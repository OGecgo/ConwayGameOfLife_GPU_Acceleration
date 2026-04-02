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

#include "GameOfLife.h"

int main (int argc, char *argv[]){

    bool run;
    //set map
    GameOfLife* b = GameOfLifeInit(10, 10, &run);
    GameOfLifeGetPresetMapValue(b);

	printf("START GAME OF LIFE\n");

	while (run){
        GameOfLifeUpdate(b);
	}

    GameOfLifeDestroy(b);
	printf("END GAME OF LIFE\n");

    return 0;
}
