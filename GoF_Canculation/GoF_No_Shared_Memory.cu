#include <cuda_runtime.h>

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <cuda/cmath>


// for now not use<-----
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
    #include "../GoF.h"
    #include "../GoFConfig.h"
}


struct GoF
{
    Bitmap* bitmap;
    bool* device_map;
    bool* device_map_copy;
    int threads;
    int blocks;
};




// kernel 
// with no use shared memmory
__global__ void Update(bool* map, bool* copy_map, int size_map, int size_block, int width_block){
    

    int workIndex = threadIdx.x + blockDim.x * blockIdx.x;


// doulevw les kai pada ta threads tha einai > apo to block_size
    
// o sohos mou twra einai na prosthw 2 dimation wste na min metraw egw me kathe thread me poio block na doulepse
// afto pou tha ginete. ena block tha apofasizei an ena life einai life
//
// an ta blocks ftasoun tha einai arketa veltisto. fasi tha einai ana block vector add
// an einai ligotera ta blocks is ta threads ana block. tha to skeftw meta
    

    int lifes = 0;
    // do step back
    int x = threadIdx.x%(width_block - INTERACT_BLOCKS_AROUND);
    int y = threadIdx.x/(width_block - INTERACT_BLOCKS_AROUND);
    int pos_block = x + MAP_WIDTH*y;
    // chekc if threads will be more than size_block
    if (threadIdx.x < size_block){
        // height check if out of buffer or not
        if (pos_block >= 0 && pos_block < MAP_WIDTH){
            // width check if out of proper layer or not
            if (pos_block / MAP_WIDTH == (pos_block - x) / MAP_WIDTH){
                
            }
        }


    }






    if (workIndex < size){
        // count of life
        // -n , ... , 0 , ... , n
        int check_pos = 0;


        for (int h = -INTERACT_BLOCKS_AROUND; h < INTERACT_BLOCKS_AROUND + 1; h++){
            for (int w = -INTERACT_BLOCKS_AROUND; w < INTERACT_BLOCKS_AROUND + 1; w++){
                int check_pos;
                check_pos = workIndex + h * MAP_WIDTH + w;
                // height check (do not go out of buffer)
                if (check_pos < 0 || check_pos >= size) continue;
                // width check (do not chaing layer)
                if (check_pos / MAP_WIDTH != (check_pos - w) / MAP_WIDTH) continue;
                // add if true
                lifes += copy_map[check_pos];
            }
        }    

        // birth - death block
        if (map[workIndex] == true){
            lifes --;
            if (lifes < MIN_LIFES_FOR_SURVIVE || lifes > MAX_LIFES_FOR_SURVIVE) 
                map[workIndex] = false;
        }else{
            if (lifes >= MIN_LIFES_FOR_BIRTH && lifes <= MAX_LIFES_FOR_BIRTH) 
                map[workIndex] = true;
        }
    }


    
    // cudaError_t a = cudaSetDevice(0);
    // do check. how much data trasfer to thread adn make it be paralel 2.2.4.1
}





extern "C"{

GoF* GoFInit(){
    // host malloc
    GoF* gof = (GoF*)malloc(sizeof(GoF));
    gof->bitmap = BitmapInit(MAP_HEIGHT, MAP_WIDTH);

    // global malloc
    int size = MAP_HEIGHT * MAP_WIDTH;
    CUDA_CHECK( cudaMalloc(&(gof->device_map)     , sizeof(bool)*size) );
    CUDA_CHECK( cudaMalloc(&(gof->device_map_copy), sizeof(bool)*size) );

    // data about gpu
    int device; // gpu
    CUDA_CHECK( cudaGetDevice(&device) );
    cudaDeviceProp prop;
    CUDA_CHECK( cudaGetDeviceProperties(&prop, device) );

    // one block for one canculate
    gof->threads = prop.maxThreadsPerBlock;
    int size_block = (INTERACT_BLOCKS_AROUND + INTERACT_BLOCKS_AROUND + 1);
    size_block *= size_block;
    if (gof->threads > size_block)
        // wrap is 32 
        gof->threads = 32 * (size_block/32 + 1);

    // (vectorLength + threads-1)/threads;
    gof->blocks = cuda::ceil_div(size, gof->threads);

    //------------> for not not use <-----------
    size_t shared_mem = prop.sharedMemPerBlock;
    size_t shared_mem_sm = prop.sharedMemPerMultiprocessor;
    size_t reg = prop.regsPerBlock;
    size_t reg_sm = prop.regsPerMultiprocessor;




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


// bad idea to malloc and free every frame <!!!!!!!! dont forget 

void GoFUpdateBitmap(GoF* gof){

    Bitmap* bitmap = gof->bitmap;
    bool* map = bitmap->map;
    int size = bitmap->size;


    // copy data to global memmory
    CUDA_CHECK( cudaMemcpy(gof->device_map, map, sizeof(bool)*size, cudaMemcpyHostToDevice) );
    CUDA_CHECK( cudaMemcpy(gof->device_map_copy, map, sizeof(bool)*size, cudaMemcpyHostToDevice) );

    int size_block = (INTERACT_BLOCKS_AROUND + INTERACT_BLOCKS_AROUND + 1);
    // that used for ease canculate postiion on block for each thread
    int pos_block = -( INTERACT_BLOCKS_AROUND + INTERACT_BLOCKS_AROUND * size_block);
    Update<<<gof->blocks, gof->threads>>>(gof->device_map, gof->device_map_copy, size, size_block, pos_block);

    // wait update perform
    CUDA_CHECK( cudaDeviceSynchronize() );

    // copy output
    CUDA_CHECK( cudaMemcpy(map, gof->device_map, sizeof(bool) * size, cudaMemcpyDeviceToHost) );


}


Bitmap* GoFGetBitmap(GoF* gof){
    return gof->bitmap;
}

} // extern C