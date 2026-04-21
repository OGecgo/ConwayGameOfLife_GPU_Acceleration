/*
 * Test program for Renderer2D
 * Creates a red/black split pattern to test GPU rendering
 */

#include <SDL3/SDL_main.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

// metadata
#include "WindowData.h"

#include "Map.h"
#include "Bitmap.h"

int main (int argc, char *argv[]){

    bool run;
    // buffer
    Bitmap* bitmap = BitmapInit(10, 10);
    //create window
    MapInit(bitmap, &run);

	printf("START GAME OF LIFE\n");

    // set values
    bool* map = bitmap->map;
    for(int i = 0; i < bitmap->width * bitmap->height; i++){
        if (i%4 == 0)
            map[i] = true;
        // else if (i%5 == 0)
        //     map[i] = true;
    }
    // main loop
	while (run){
        MapUpdateBitmap(bitmap);
	}


    // clear memmory
    BitmapDestroy(bitmap);
    MapDestroy();

	printf("END GAME OF LIFE\n");

    return 0;
}
