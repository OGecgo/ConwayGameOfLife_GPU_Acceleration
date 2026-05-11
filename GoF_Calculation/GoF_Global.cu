#include <cuda_runtime.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <cuda/cmath>



#define MAP_SIZE MAP_HEIGHT * MAP_WIDTH
// Check CUDA errors and print diagnostic information
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
    int data[2];            // [0] = live, [1] = deaths
    bool* device_map;       // next generation (GPU memory)
    bool* device_map_copy;  // current generation (GPU memory)
    int* device_data;       // [0] = live, [1] = deaths (GPU memory)
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
        // Loop order optimized for memory coalescing:
        // Inner loop (w) over columns, outer loop (h) over rows.
        // This ensures consecutive threads access consecutive memory locations.
        for (int h = -INTERACT_BLOCKS_AROUND; h < INTERACT_BLOCKS_AROUND + 1; h++){
            for (int w = -INTERACT_BLOCKS_AROUND; w < INTERACT_BLOCKS_AROUND + 1; w++){
                int check_pos = workIndex + h * MAP_WIDTH + w;
                // Bounds check: stay within buffer
                bool check = (check_pos >= 0 && check_pos < MAP_SIZE);
                // Bounds check: don't wrap to adjacent rows
                int row_test = check_pos % MAP_WIDTH - w;
                check = (row_test < MAP_WIDTH && row_test >= 0) && check;
                if (check) lifes += copy_map[check_pos];
            }
        }    

       // survive, birth or death
        bool current_state = copy_map[workIndex];
        bool next_state = current_state;
        if (current_state == true){
            lifes --; // Don't count itself
            next_state = (lifes >= MIN_LIFES_FOR_SURVIVE && lifes <= MAX_LIFES_FOR_SURVIVE);
        }else{
            next_state = (lifes >= MIN_LIFES_FOR_BIRTH && lifes <= MAX_LIFES_FOR_BIRTH);
        }
        map[workIndex] = next_state;
        // Count statistics for this generation
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
    // set false values
    CUDA_CHECK( cudaMemset(gof->device_map     , false, sizeof(bool)*MAP_SIZE) );
    CUDA_CHECK( cudaMemset(gof->device_map_copy, false, sizeof(bool)*MAP_SIZE) );
    CUDA_CHECK( cudaMemset(gof->device_data    , false, sizeof(int)*2));

    // GPU properties
    int device; 
    CUDA_CHECK( cudaGetDevice(&device) );
    cudaDeviceProp prop;
    CUDA_CHECK( cudaGetDeviceProperties(&prop, device) );

    printf("GPU Characteristics:\n");
    printf("\tMax SM::%d\n", prop.multiProcessorCount);
    printf("\tMax Threads per SM::%d\n", prop.maxThreadsPerMultiProcessor);
    printf("\tMax Registers per SM::%d\n", prop.regsPerMultiprocessor);
    printf("\t---\n");
    printf("\tMax Blocks per SM::%d\n", prop.maxBlocksPerMultiProcessor);
    printf("\tMax Threads per Block::%d\n", prop.maxThreadsPerBlock);
    printf("\tMax Registers per Block::%d\n", prop.regsPerBlock); 

    // Calculate optimal thread/block configuration for load balancing
    gof->threads = prop.maxThreadsPerMultiProcessor / prop.maxBlocksPerMultiProcessor;
    if (gof->threads > prop.maxThreadsPerBlock) 
        gof->threads = prop.maxThreadsPerBlock;
    //(vectorLength + threads-1)/threads;
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




void GoFUpdate(GoF* gof){
    // clear statistics
    CUDA_CHECK( cudaMemset(gof->device_data, 0, sizeof(int)*2) );
    
    // launch kernel
    Update<<<gof->blocks, gof->threads>>>(gof->device_map, gof->device_map_copy, gof->device_data);
    
    // Wait for GPU to finish (kernel + memory operations)
    CUDA_CHECK( cudaDeviceSynchronize() );
    // copy results from GPU to host CPU
    CUDA_CHECK( cudaMemcpy(gof->bitmap->map, gof->device_map, sizeof(bool) * MAP_SIZE, cudaMemcpyDeviceToHost) );
    CUDA_CHECK( cudaMemcpy(gof->data, gof->device_data, sizeof(int)*2, cudaMemcpyDeviceToHost));

    // swap buffers for next generation
    bool* temp = gof->device_map_copy;
    gof->device_map_copy = gof->device_map;
    gof->device_map = temp;
}



Bitmap* GoFGetBitmap(GoF* gof){
    return gof->bitmap;
}

void GoFUpdateBitmap(GoF* gof){
    // copy CPU host data to GPU global memmory
    CUDA_CHECK( cudaMemcpy(gof->device_map_copy, gof->bitmap->map, sizeof(bool)*MAP_SIZE, cudaMemcpyHostToDevice) );
}

int GoFGetLive(GoF* gof){
    return gof->data[0];
}
int GoFGetDeaths(GoF* gof){
    return gof->data[1];
}


} // extern C