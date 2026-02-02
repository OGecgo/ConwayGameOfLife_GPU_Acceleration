#include "Renderer2D.h"

#include <SDL3/SDL.h>

#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_error.h>


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>

// variables
SDL_Window* window;
SDL_Renderer* renderer;
bool* run_true;


void ErrorSDLPrint(char* error){
	printf("ERROR - %s: %s", error, SDL_GetError());
}


void ErrorCrashSDLPrint(char* error){
	printf("ERROR - %s: %s", error, SDL_GetError());
    Renderer2DDestroy();
	exit(1);
}


// signal handler for sigint
void SignalHandler(int sig){
	// _Exit(sig); 
	exit(sig);
}






bool Renderer2DInit(Renderer2DMetada* metadata, bool* run, uint8_t r, uint8_t g, uint8_t b, uint8_t a){
    bool val_return;

    // set metadata 
    if (!SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_NAME_STRING, metadata->name_aplication) ) 
    { 
        val_return = false; 
        ErrorSDLPrint("SDL_PROP_APP_METADATA_NAME_STRING");
    }
    if (!SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_VERSION_STRING, metadata->version) ) 
    { 
        val_return = false; 
        ErrorSDLPrint("SDL_PROP_APP_METADATA_VERSION_STRING"); 
    }
    if (!SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_IDENTIFIER_STRING, metadata->reverse_domain) ) 
    { 
        val_return = false; 
        ErrorSDLPrint("SDL_PROP_APP_METADATA_IDENTIFIER_STRING"); 
    }
    if (!SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_CREATOR_STRING, metadata->creator) ) 
    { 
        val_return = false; 
        ErrorSDLPrint("SDL_PROP_APP_METADATA_CREATOR_STRING"); 
    }
    if (!SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_COPYRIGHT_STRING, metadata->copyright) ) 
    { 
        val_return = false; 
        ErrorSDLPrint("SDL_PROP_APP_METADATA_COPYRIGHT_STRING"); 
    }
    if (!SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_URL_STRING, metadata->url) ) 
    { 
        val_return = false; 
        ErrorSDLPrint("SDL_PROP_APP_METADATA_URL_STRING"); 
    }
    if (!SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_TYPE_STRING, metadata->type_aplication) ) 
    { 
        val_return = false; 
        ErrorSDLPrint("SDL_PROP_APP_METADATA_TYPE_STRING"); 
    }



    

	// initialize video sdl
	if (!SDL_Init(SDL_INIT_VIDEO)) ErrorCrashSDLPrint("SDL_INIT_VIDEO"); 

    // window init
    window = SDL_CreateWindow(metadata->name_aplication, metadata->size_x, metadata->size_y, SDL_WINDOW_RESIZABLE);
	if (window == NULL) ErrorCrashSDLPrint("SDL_CREATE_WINDOW");

    // renderer init 
    // auto take driver for rendering
    renderer = SDL_CreateRenderer(window, NULL);
    if (renderer == NULL) ErrorCrashSDLPrint("SDL_CREATE_RENDERER");
    // set clear data
    if (!SDL_SetRenderDrawColor(renderer, r, g, b, a))
    {
        val_return = false;
        ErrorSDLPrint("SDL_SET_RENDER_DRAW_COLOR");
    }


    // set run_true
    run_true = run;
    *run_true = true;
    // add signint signal
    signal(SIGINT, SignalHandler);

    return val_return;
}


void Renderer2DDestroy(){
    if (renderer != NULL) SDL_DestroyRenderer(renderer);
    if (window != NULL) SDL_DestroyWindow(window);
    SDL_Quit();
}


bool Renderer2DClear(){
    return SDL_RenderClear(renderer);
}
bool Renderer2DPresent(){
    return SDL_RenderPresent(renderer);
}

void Renderer2DCheckEvents(){
    SDL_Event event;
    while (SDL_PollEvent(&event)){ 
        if (event.type == SDL_EVENT_QUIT){
            *run_true = false;
        }
    }
}