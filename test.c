#include <SDL3/SDL_main.h>

#include <stdlib.h>
#include <stdio.h>

// metadata
#include "WindowData.h"

#include "Renderer2D.h"

int main (int argc, char *argv[]){

	// metadata
	Renderer2DMetada* metadata = malloc(sizeof(Renderer2DMetada));
	metadata->size_x = SIZE_X;
	metadata->size_y = SIZE_Y;
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
	Renderer2DInit(metadata, &run, 0, 0, 0, 1);


	printf("START GAME OF LIFE\n");

	while (run){
        Renderer2DCheckEvents();
        if (!Renderer2DClear()) printf("Error rendering clear\n");
        if (!Renderer2DPresent()) printf("Error rendering present\n");

	}
	printf("END GAME OF LIFE\n");

    Renderer2DDestroy();

    return 0;
}
