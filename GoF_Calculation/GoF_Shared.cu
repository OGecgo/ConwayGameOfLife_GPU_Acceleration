#include <cuda_runtime.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <cuda/cmath>



#define BLOCK_WIDTH (INTERACT_BLOCKS_AROUND + INTERACT_BLOCKS_AROUND + 1)
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
    int shared_mem;

};


// copy global data to shared memory
// copy only row need each thread into block
// warning! they copy data without take mind the change of row. only buffer
// if value out of buffer. they automaticly will be false
__forceinline__ __device__ void readData(int pos, bool* copy_map){
    extern __shared__ bool tile[];

    // LEFT
    int left_pos = pos - INTERACT_BLOCKS_AROUND;
    // do not go out of buffer
    if (left_pos >= 0 && left_pos < MAP_SIZE){
        // copy first data
        tile[threadIdx.x] = copy_map[left_pos];
    }
    else{
        tile[threadIdx.x] = false;
    }

    // LAST RIGHT
    // only for last BLOCK_WIDTH-1 positions
    if(threadIdx.x > blockDim.x - BLOCK_WIDTH){
        int right_pos = pos + INTERACT_BLOCKS_AROUND;
        // do not go out of buffer
        if(right_pos >= 0 && right_pos < MAP_SIZE){
            // copy last data
            tile[threadIdx.x + BLOCK_WIDTH - 1] = copy_map[right_pos];
        }
        else{
            tile[threadIdx.x + BLOCK_WIDTH - 1] = false;
        }
    }

    // wait when all threads end with transfer data to shared memory
    __syncthreads(); 
}


// kernel 
__global__ void Update(bool* map, bool* copy_map, int* data){
    // data[0] = live
    // data[1] = deaths
    int workIndex = threadIdx.x + blockDim.x * blockIdx.x;
    extern __shared__ bool tile[];

    // each thread count of life
    int lifes = 0;
    // -n , ... , 0 , ... , n
    for (int h = -INTERACT_BLOCKS_AROUND; h < INTERACT_BLOCKS_AROUND + 1; h++){
        // move pos h steps up/down
        int pos = workIndex + h * MAP_WIDTH;
        readData(pos, copy_map);
        
        // add values from shared memory
        int row = workIndex % MAP_WIDTH;
        for (int w = -INTERACT_BLOCKS_AROUND; w < INTERACT_BLOCKS_AROUND + 1; w++){
            // do not change row when position in edge
            if (row + w >= 0 && row + w < MAP_WIDTH){
                lifes += tile[threadIdx.x + w + INTERACT_BLOCKS_AROUND]; 
            }
        }
        // wait when all threads end with add lifes
        // reason: start write to tile wrap when another wrap dont end read the tile
        //         threads write and read similars position into tile
        __syncthreads();
    }

    if (workIndex < MAP_SIZE) {
        // survive or birth
        bool current_state = copy_map[workIndex];
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
    printf("GPU Characteristics:\n");
    printf("\tMax SM::%d\n", prop.multiProcessorCount);
    printf("\tMax Threads per SM::%d\n", prop.maxThreadsPerMultiProcessor);
    printf("\tMax Registers per SM::%d\n", prop.regsPerMultiprocessor);
    printf("\tMax Shared memory per SM::%d\n", prop.sharedMemPerMultiprocessor);
    printf("\t---\n");
    printf("\tMax Blocks per SM::%d\n", prop.maxBlocksPerMultiProcessor);
    printf("\tMax Threads per Block::%d\n", prop.maxThreadsPerBlock);
    printf("\tMax Registers per Block::%d\n", prop.regsPerBlock); 
    printf("\tMax Shared memory per Block::%d\n", prop.sharedMemPerBlock);


    // for maximize usage of multiprocessor
    gof->threads = prop.maxThreadsPerMultiProcessor / prop.maxBlocksPerMultiProcessor;
    if (gof->threads > prop.maxThreadsPerBlock) 
        gof->threads = prop.maxThreadsPerBlock;

    gof->blocks = cuda::ceil_div(MAP_SIZE, gof->threads);

    // count size of shared memory need the kernel
    //
    // shared memory have the row of pixels for counting 
    // new row new read
    gof->shared_mem = (gof->threads + BLOCK_WIDTH - 1) * sizeof(bool);
    if (gof->shared_mem > prop.sharedMemPerBlock){
        gof->shared_mem = prop.sharedMemPerBlock;
    }

    printf("Game of Life\n");
    printf("Need threads::%d\n", MAP_SIZE);
    printf("Threads::%d, Blocks::%d, Total Threads::%d\n", gof->threads, gof->blocks, gof->threads * gof->blocks);
    printf("Shared Memory::%d\n", gof->shared_mem);
    printf("\n");

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
    CUDA_CHECK( cudaMemset(gof->device_data, 0, sizeof(int)*2) );
    // kernel
    Update<<<gof->blocks, gof->threads, gof->shared_mem>>>(gof->device_map, gof->device_map_copy, gof->device_data);
    
    // wait update perform
    CUDA_CHECK( cudaDeviceSynchronize() );
    // copy output
    // part 6 (for test speeds without it just commented)
    CUDA_CHECK( cudaMemcpy(gof->bitmap->map, gof->device_map, sizeof(bool) * MAP_SIZE, cudaMemcpyDeviceToHost) );
    CUDA_CHECK( cudaMemcpy(gof->data, gof->device_data, sizeof(int)*2, cudaMemcpyDeviceToHost));

    // change pointers for next Update
    bool* temp = gof->device_map_copy;
    gof->device_map_copy = gof->device_map;
    gof->device_map = temp;
}



Bitmap* GoFGetBitmap(GoF* gof){
    return gof->bitmap;
}

void GoFUpdateBitmap(GoF* gof){
    // copy data to global memmory
    CUDA_CHECK( cudaMemcpy(gof->device_map_copy, gof->bitmap->map, sizeof(bool)*MAP_SIZE, cudaMemcpyHostToDevice) );
}

int GoFGetLive(GoF* gof){
    return gof->data[0];
}
int GoFGetDeaths(GoF* gof){
    return gof->data[1];
}


} // extern C
