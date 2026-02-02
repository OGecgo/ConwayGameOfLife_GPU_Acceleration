#ifndef RENDERER2D_H
#define RENDERER2D_H


// Renderer2D need for simple with abstractions usage of sdl lib

/*
    lib using is SDL3
    in main you should include include <SDL3/SDL_main.h> for
    properly work SDL
*/



#include <stdbool.h>
#include <stdint.h>


// metadata
typedef struct
{
    int size_x;
    int size_y;
    char* name_aplication;
    char* version;
    char* reverse_domain;
    char* creator;
    char* copyright;
    char* url;
    char* type_aplication;
}Renderer2DMetada;

// buffer collor


/* 
    run_true is value what used for main while
    run_true added to events for sdl and to signal SIGINT
    by default run_true seted by initRenderer with true
*/
// if return false is error
// r, g, b, a is clear color
bool Renderer2DInit(Renderer2DMetada* metadata, bool* run_true,  uint8_t r, uint8_t g, uint8_t b, uint8_t a);
void Renderer2DDestroy();

bool Renderer2DClear();
bool Renderer2DSetBufferCollor(); // to do ftiahnw tin dinatotita na doulevw me buffer. 
bool Renderer2DPresent();

// NODE:
/* 
    Note that Windows (and possibly other platforms) has a quirk 
    about how it handles events while dragging/resizing a window, 
    which can cause this function to block for significant amounts of time. 
    Technical explanations and solutions are discussed on the wiki:
*/
// Check only quite event
void Renderer2DCheckEvents();
#endif