#include <cuda_runtime.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <cuda/cmath>



#define WIDTH_BLOCK (INTERACT_BLOCKS_AROUND + INTERACT_BLOCKS_AROUND + 1)
#define SIZE_BLOCK WIDTH_BLOCK * WIDTH_BLOCK
#define MAP_SIZE MAP_HEIGHT * MAP_WIDTH

#define CUDA_CHECK(expr_to_check){                     \
    cudaError_t result  = expr_to_check;               \
    if(result != cudaSuccess)                          \
    {                                                  \
        fprintf(stderr,                                \
                "CUDA Runtime Error: %s:%i:%d = %s\n", \
                __FILE__,                              \
                __LINE__,                              \
                result,                                \
                cudaGetErrorString(result));           \
    }                                                  \
} 


extern "C"{
    #include "GoF.h"
    #include "GoFConfig.h"
}


struct GoF
{
    Bitmap* bitmap;
    int data[2]; // live deaths
    bool* device_map;
    bool* device_map_copy;
    int* device_data;
    int threads;
    int blocks;

};





// kernel 
__global__ void Update(bool* map, bool* copy_map, int* data){
    // data[0] = live
    // data[1] = deaths
    int workIndex = threadIdx.x + blockDim.x * blockIdx.x;

    if (workIndex < MAP_SIZE ){
        // count of life
        int lifes = 0;

        // -n , ... , 0 , ... , n
        // wrap = 32 threads. bandwidth transfer data 32 bytes 
        // 1 step take 32 byte data. for first count lifes(all 32 threads have data )
        // 2 step take again 32 byte data for secodn  counts life add (last threads need data)
        // 3 step not need to take data 
        // (if INTERACT_BLOCKS_AROUND is big. maybe needs anothers steps with takes data)
        // next linen
        // repeat
        for (int h = -INTERACT_BLOCKS_AROUND; h < INTERACT_BLOCKS_AROUND + 1; h++){
            for (int w = -INTERACT_BLOCKS_AROUND; w < INTERACT_BLOCKS_AROUND + 1; w++){
                int check_pos = workIndex + h * MAP_WIDTH + w;
                // height check (do not go out of buffer)
                bool check = (check_pos >= 0 && check_pos < MAP_SIZE);
                // width check (do not chaing layer)
                check = (check_pos / MAP_WIDTH == (check_pos - w) / MAP_WIDTH);
                if (check) lifes += copy_map[check_pos];
            }
        }    

        // survive or birth
        bool current_state = map[workIndex];
        bool next_state = current_state;
        if (current_state == true){
            lifes --;
            next_state = (lifes >= MIN_LIFES_FOR_SURVIVE && lifes <= MAX_LIFES_FOR_SURVIVE);
        }else{
            next_state = (lifes >= MIN_LIFES_FOR_BIRTH && lifes <= MAX_LIFES_FOR_BIRTH);
        }
        map[workIndex] = next_state;
        // if next_state true add live
        // if next_state false add deaths
        atomicAdd(&data[0], next_state);
        atomicAdd(&data[1], !next_state);
    
    }
}






extern "C"{

GoF* GoFInit(){
    // host malloc
    GoF* gof = (GoF*)malloc(sizeof(GoF));
    gof->bitmap = BitmapInit(MAP_HEIGHT, MAP_WIDTH);

    // global malloc
    CUDA_CHECK( cudaMalloc(&(gof->device_map)     , sizeof(bool)*MAP_SIZE) );
    CUDA_CHECK( cudaMalloc(&(gof->device_map_copy), sizeof(bool)*MAP_SIZE) );
    CUDA_CHECK( cudaMalloc(&(gof->device_data)    , sizeof(int)*2));

    // data about gpu
    int device; 
    CUDA_CHECK( cudaGetDevice(&device) );
    cudaDeviceProp prop;
    CUDA_CHECK( cudaGetDeviceProperties(&prop, device) );

    // stats output
    printf("GPU stats:\n");
    printf("\tSM::%d\n", prop.multiProcessorCount);
    printf("\tThreads per SM::%d\n", prop.maxThreadsPerMultiProcessor);
    printf("\tRegisters per SM::%d\n", prop.regsPerMultiprocessor);
    printf("\t---\n");
    printf("\tBlocks per SM::%d\n", prop.maxBlocksPerMultiProcessor);
    printf("\tThreads per Blocks::%d\n", prop.maxThreadsPerBlock);
    printf("\tRegisters per Blocks::%d\n", prop.regsPerBlock); 

    // for maximize usage of multiprocessor
    gof->threads = prop.maxThreadsPerMultiProcessor / prop.maxBlocksPerMultiProcessor;
    if (gof->threads > prop.maxThreadsPerBlock) 
        gof->threads = prop.maxThreadsPerBlock;

    gof->blocks = cuda::ceil_div(MAP_SIZE, gof->threads);

    printf("Game of Life requer threads::%d \n", MAP_SIZE);
    printf("\n");
    printf("Will be used Threads:::%d and Blocks:::%d\n",gof->threads, gof->blocks);
    printf("Total threads::%d will be called\n", gof->threads * gof->blocks);

    return gof;
}

void GoFDestroy(GoF* gof){
    if (gof != NULL)
        if (gof->device_map != NULL)
            CUDA_CHECK( cudaFree(gof->device_map) );
        if (gof->device_map_copy != NULL)
            CUDA_CHECK( cudaFree(gof->device_map_copy) );
        free(gof);
}


// top 1: memory copy make aplication very slow
// top 2: cuda device synchronize  make aplication litle bit more slower

void GoFUpdate(GoF* gof){
    CUDA_CHECK( cudaMemcpy(gof->device_map, gof->bitmap->map, sizeof(bool)*MAP_SIZE, cudaMemcpyHostToDevice) );
    CUDA_CHECK( cudaMemcpy(gof->device_map_copy, gof->bitmap->map, sizeof(bool)*MAP_SIZE, cudaMemcpyHostToDevice) );

    // CUDA_CHECK( cudaMemcpy(gof->device_map_copy, gof->device_map, sizeof(bool) * MAP_SIZE, cudaMemcpyDeviceToDevice));
    CUDA_CHECK( cudaMemset(gof->device_data, 0, sizeof(int)*2));
    // kernel
    Update<<<gof->blocks, gof->threads>>>(gof->device_map, gof->device_map_copy, gof->device_data);
    
    // wait update perform
    CUDA_CHECK( cudaDeviceSynchronize() ); // make aplication multiple times slower
    // copy output
    CUDA_CHECK( cudaMemcpy(gof->bitmap->map, gof->device_map, sizeof(bool) * MAP_SIZE, cudaMemcpyDeviceToHost) );
    CUDA_CHECK( cudaMemcpy(gof->data, gof->device_data, sizeof(int)*2, cudaMemcpyDeviceToHost));

}


Bitmap* GoFGetBitmap(GoF* gof){
    return gof->bitmap;
}

void GoFUpdateBitmap(GoF* gof){
    // copy data to global memmory
    CUDA_CHECK( cudaMemcpy(gof->device_map, gof->bitmap->map, sizeof(bool)*MAP_SIZE, cudaMemcpyHostToDevice) );
    CUDA_CHECK( cudaMemcpy(gof->device_map_copy, gof->bitmap->map, sizeof(bool)*MAP_SIZE, cudaMemcpyHostToDevice) );
}

int GoFGetLive(GoF* gof){
    return gof->data[0];
}
int GoFGetDeaths(GoF* gof){
    return gof->data[1];
}


} // extern C