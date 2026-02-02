/*
 * Test program for Renderer2D
 * Creates a red/black split pattern to test GPU rendering
 */

#include <SDL3/SDL_main.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// metadata
#include "WindowData.h"

#include "Renderer2D.h"

int main (int argc, char *argv[]){

	// metadata
	Renderer2DMetada* metadata = malloc(sizeof(Renderer2DMetada));
	metadata->window_size_x = WINDOW_SIZE_X;
	metadata->window_size_y = WINDOW_SIZE_Y;
	metadata->name_aplication = NAME_APLICATION;
	metadata->version = VERSION;
	metadata->reverse_domain = REVERSE_DOMAIN;
	metadata->creator = CREATOR;
	metadata->copyright = COPYRIGHT;
	metadata->url = GITHUB_REPOSITORY;
	metadata->type_aplication = TYPE_APLICATION;

	// run main while loop
	bool run;
    // black clear color
	if(!Renderer2DInit(metadata, &run, 0, 0, 0, 1, 100, 100)) printf("Error renderer2dinit\n");
    free(metadata);
    // set by dfault
    uint32_t* rgba = malloc(100 * 100 * sizeof(uint32_t));
    memset(rgba, 0x000000FF, 100 * 100 * sizeof(uint32_t)); // by default color
    for (int x = 0; x < 100; x++){
        for (int y = 0; y < 100; y++){
            if (y > 49)
            {
                rgba[y * 100 + x] = 0x000000FF;
                continue;
            }
            rgba[y * 100 + x] = 0xFF0000FF;

        }
    }

	printf("START GAME OF LIFE\n");

	while (run){
        Renderer2DCheckEvents();
        
        if (!Renderer2DSetBufferCollor(rgba)) printf("Error Buffer color\n");

        if (!Renderer2DPresent()) printf("Error rendering present\n");

 

	}


    free(rgba);
	printf("END GAME OF LIFE\n");
    Renderer2DDestroy();

    return 0;
}
