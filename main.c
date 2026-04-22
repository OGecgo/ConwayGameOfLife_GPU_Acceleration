/*
 * Test program for Renderer2D
 * Creates a red/black split pattern to test GPU rendering
 */

#include <SDL3/SDL_main.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>


#include "Window.h"
#include "Bitmap.h"
#include "GoF.h"

int main (int argc, char *argv[]){

    bool run;
    // set up game of life 
    GoF* gof = GoFInit();
    // buffer
    Bitmap* bitmap = GoFGetBitmap(gof);
    //create window
    WindowInit(bitmap, &run);

	printf("START GAME OF LIFE\n");

    // set values
    bool* map = bitmap->map;
    for(int i = 0; i < bitmap->width * bitmap->height; i++){
        if (i%5 == 0)
            map[i] = true;
        // else if (i%5 == 0)
        //     map[i] = true;
    }
    // main loop
	while (run){
        GoFUpdateBitmap(gof);
        WindowUpdateBitmap(bitmap);
        break;
	}


    // clear memmory
    BitmapDestroy(bitmap);
    WindowDestroy();

	printf("END GAME OF LIFE\n");

    return 0;
}
