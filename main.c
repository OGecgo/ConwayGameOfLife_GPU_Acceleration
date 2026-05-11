

#include <SDL3/SDL_main.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#ifndef NO_SDL
    #include "GoF_Window/Window.h"
#endif

#include "utils/Bitmap.h"
#include "GoF_Calculation/GoF.h"
#include "mainConfig.h"


void sleepFor(double time){
    clock_t start = clock();
    clock_t end = clock();
    while ((double)(end - start)/CLOCKS_PER_SEC < time)
    {
        end = clock();
    }
}



void randomizeValues(bool* map, int size){
    for(int i = 0; i < size; i++){
        map[i] = rand()%2;
    }
}


int main (int argc, char *argv[]){

    bool run;
    // Initialize game of life 
    GoF* gof = GoFInit();
    if (PRINT_GOF_GPU_STATS) return 0;
    // get bitmap
    Bitmap* bitmap = GoFGetBitmap(gof);

    #ifndef NO_SDL
        //create window SDL
        WindowInit(bitmap, &run);
    #else
        run = true;
    #endif

    // set bitmap with random values
    srand(RANDOM_SEED); 
    randomizeValues(bitmap->map, bitmap->size);
    GoFUpdateBitmap(gof);

    printf("--START GAME OF LIFE--\n");
    int gen = 0;
    double time = 0;
    // main loop
	while (run && (OFF_MAXGEN || gen < MAXGEN)){
        
        #ifndef NO_SDL
            WindowUpdateBitmap(bitmap);
        #endif

        // count time
        clock_t start = clock();
        GoFUpdate(gof);
        clock_t end = clock();
        // get counters
        int live = GoFGetLive(gof);
        int deaths = GoFGetDeaths(gof);
        time += (double)(end - start)/CLOCKS_PER_SEC;
        // print statistics
        printf("Gen::%d, Deaths::%d Live::%d Time::%f_sec\n", gen, deaths, live, (double)(end - start)/CLOCKS_PER_SEC);

        // break;
        // sleepFor(0.1);
        gen++;
	}

    // print time statistics
    printf("Times statistics for bitmap update::\n");
    printf("\tAvarage Time::%f\n", time/gen);
    printf("\tTime to end %d generations::%f\n", gen, time);

    // clear memmory
    GoFDestroy(gof);
    BitmapDestroy(bitmap);
    #ifndef NO_SDL
        WindowDestroy();
    #endif

	printf("--END GAME OF LIFE--\n");

    return 0;
}
