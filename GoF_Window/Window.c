#include "Window.h"

#include "WindowData.h"
#include "Renderer2D.h"
#include "../utils/Bitmap.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <time.h>

// Global pixel buffer for the screen.
// Reused every frame to avoid calling malloc/free in a loop.
static uint32_t *rgba8888;

static void ConverBitmapToRGBA8888(Bitmap *bitmap)
{

    // Convert boolean game grid to actual colors: 
    // True = White (Alive), False = Black (Dead)
    for (int i = 0; i < bitmap->size; i++)
    {
        rgba8888[i] = bitmap->map[i] ? 0xFFFFFFFF : 0x000000FF;
    }
}

void WindowInit(Bitmap* bitmap, bool *run_main_while)
{
    // Prepare SDL window properties
    Renderer2DMetada *metadata = malloc(sizeof(Renderer2DMetada));
    metadata->window_size_x = WINDOW_SIZE_X;
    metadata->window_size_y = WINDOW_SIZE_Y;
    metadata->name_aplication = NAME_APLICATION;
    metadata->version = VERSION;
    metadata->reverse_domain = REVERSE_DOMAIN;
    metadata->creator = CREATOR;
    metadata->copyright = COPYRIGHT;
    metadata->url = GITHUB_REPOSITORY;
    metadata->type_aplication = TYPE_APLICATION;

    // Create the window with a black background (R:0, G:0, B:0, A:1)
    if (!Renderer2DInit(metadata, run_main_while, 0, 0, 0, 1, bitmap->width, bitmap->height))
        printf("\nError Renderer2DInit\n");
    // Metadata is passed to SDL, so we don't need it anymore
    free(metadata);


    // Allocate memory for our pixel buffer and fill it with black
    rgba8888 = malloc(sizeof(uint32_t) * bitmap->size);
    memset(rgba8888, 0x000000FF, bitmap->size);
}


void WindowDestroy()
{
    Renderer2DDestroy();
    if (rgba8888 != NULL) {
        free(rgba8888);
        rgba8888 = NULL;
    }
}

void WindowUpdateBitmap(Bitmap *bitmap)
{
    ConverBitmapToRGBA8888(bitmap);

    Renderer2DCheckEvents();
    
    if (!Renderer2DSetBufferCollor(rgba8888))
        printf("Error Buffer color\n");

    if (!Renderer2DPresent())
        printf("Error rendering present\n");
}