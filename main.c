

#include <SDL3/SDL_main.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include "GoF_Window/Window.h"
#include "utils/Bitmap.h"

#include "GoF_Canculation/GoF.h"


#define RANDOM_SEED 0


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

void testNonRandomValue(bool* map, int size){
    for(int i = 0; i < size; i++){
        if (i%2 == 0)
            map[i] = true;

    }
}

// melidika: prosthetw dio metrites. ta apotelesmata tha ta pernw apo to GoF
int main (int argc, char *argv[]){

    bool run;
    // set up game of life 
    GoF* gof = GoFInit();
    // get bitmap
    Bitmap* bitmap = GoFGetBitmap(gof);
    //create window
    WindowInit(bitmap, &run);

    srand(RANDOM_SEED); 
    testNonRandomValue(bitmap->map, bitmap->size);
    GoFUpdateBitmap(gof);

    printf("--START GAME OF LIFE--\n");
    // main loop
	while (run){
        WindowUpdateBitmap(bitmap);

        // count time
        clock_t start = clock();
        GoFUpdate(gof);
        clock_t end = clock();
        int live = GoFGetLive(gof);
        int deaths = GoFGetDeaths(gof);
        double time = (double)(end - start)/CLOCKS_PER_SEC;
        printf("Deaths::%d Live::%d Time::%f_sec\n", deaths, live, time);

        // break;
        // sleepFor(0.1);
	}


    // clear memmory
    GoFDestroy(gof);
    BitmapDestroy(bitmap);
    WindowDestroy();

	printf("--END GAME OF LIFE--\n");

    return 0;
}
