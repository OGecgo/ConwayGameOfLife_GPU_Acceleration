/*
 * Test program for Renderer2D
 * Creates a red/black split pattern to test GPU rendering
 */

#include <SDL3/SDL_main.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#include "Window.h"
#include "Bitmap.h"

#include "GoF.h"

void sleepFor(double time){
    clock_t start = clock();
    clock_t end = clock();
    while ((double)(end - start)/CLOCKS_PER_SEC < time)
    {
        end = clock();
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

	printf("START GAME OF LIFE\n");

    // set values
    bool* map = bitmap->map;
    for(int i = 0; i < bitmap->size; i++){
        if (i%2 == 0)
            map[i] = true;

    }
    // main loop
	while (run){
        WindowUpdateBitmap(bitmap);

        // count time
        clock_t start = clock();
        GoFUpdateBitmap(gof);
        clock_t end = clock();
        printf("Time to update bitmap is: %f\n", (double)(end - start));
        // for show
        // sleepFor(0.05);
	}


    // clear memmory
    GoFDestroy(gof);
    BitmapDestroy(bitmap);
    WindowDestroy();

	printf("END GAME OF LIFE\n");

    return 0;
}
