#include "Renderer2D.h"

// SDL3 includes
#include <SDL3/SDL.h>
#include <SDL3/SDL_events.h>
#include <SDL3/SDL_init.h>
#include <SDL3/SDL_video.h>
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_pixels.h>
#include <SDL3/SDL_error.h>

// Standard library
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>

// Static SDL objects
static SDL_Window *window;
static SDL_Renderer *renderer;
static SDL_Texture *texture;

// Main loop control and buffer dimensions
static bool *run_true;
int _buffer_size_x, _buffer_size_y;

// Print non-critical SDL error and continue execution
static void ErrorSDLPrint(char *error)
{
    printf("ERROR - %s: %s\n", error, SDL_GetError());
}

// Print critical SDL error, cleanup, and exit the program
static void ErrorCrashSDLPrint(char *error)
{
    printf("CRITICAL ERROR - %s: %s\n", error, SDL_GetError());
    Renderer2DDestroy();
    exit(1);
}

// Catch Ctrl+C and gracefully exit
static void SignalHandler(int sig)
{
    exit(sig);
}



// Create SDL window with specified size and resizable flag
static void InitWindow(Renderer2DMetada *metadata)
{
    window = SDL_CreateWindow(metadata->name_aplication, metadata->window_size_x, metadata->window_size_y, SDL_WINDOW_RESIZABLE);
    if (window == NULL)
        ErrorCrashSDLPrint("SDL_CREATE_WINDOW");
}
// Create renderer, set background color, and clear the screen once
static void InitRenderer(uint8_t r, uint8_t g, uint8_t b, uint8_t a, bool *val_return)
{
    // Renderer auto-selects best SDL driver available
    renderer = SDL_CreateRenderer(window, NULL);
    if (renderer == NULL)
        ErrorCrashSDLPrint("SDL_CREATE_RENDERER");

    // Set the color used for clearing the screen each frame
    if (!SDL_SetRenderDrawColor(renderer, r, g, b, a))
    {
        *val_return = false;
        ErrorSDLPrint("SDL_SET_RENDER_DRAW_COLOR");
    }

    // Clear once to show the background color
    SDL_RenderClear(renderer);
}
// Create a texture that can be updated each frame 
static void InitTexture()
{
    // Create streaming texture for color buffer updates
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, _buffer_size_x, _buffer_size_y);
    if (texture == NULL)
        ErrorCrashSDLPrint("SDL_CREATE_TEXTURE");
}

// Main initialization function
bool Renderer2DInit(
    Renderer2DMetada *metadata, bool *run,
    uint8_t r, uint8_t g, uint8_t b, uint8_t a,
    int buffer_size_x, int buffer_size_y)
{
    _buffer_size_x = buffer_size_x;
    _buffer_size_y = buffer_size_y;
    bool val_return = true;

    // Set SDL app metadata
    if (!SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_NAME_STRING, metadata->name_aplication))
    {
        val_return = false;
        ErrorSDLPrint("SDL_PROP_APP_METADATA_NAME_STRING");
    }
    if (!SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_VERSION_STRING, metadata->version))
    {
        val_return = false;
        ErrorSDLPrint("SDL_PROP_APP_METADATA_VERSION_STRING");
    }
    if (!SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_IDENTIFIER_STRING, metadata->reverse_domain))
    {
        val_return = false;
        ErrorSDLPrint("SDL_PROP_APP_METADATA_IDENTIFIER_STRING");
    }
    if (!SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_CREATOR_STRING, metadata->creator))
    {
        val_return = false;
        ErrorSDLPrint("SDL_PROP_APP_METADATA_CREATOR_STRING");
    }
    if (!SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_COPYRIGHT_STRING, metadata->copyright))
    {
        val_return = false;
        ErrorSDLPrint("SDL_PROP_APP_METADATA_COPYRIGHT_STRING");
    }
    if (!SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_URL_STRING, metadata->url))
    {
        val_return = false;
        ErrorSDLPrint("SDL_PROP_APP_METADATA_URL_STRING");
    }
    if (!SDL_SetAppMetadataProperty(SDL_PROP_APP_METADATA_TYPE_STRING, metadata->type_aplication))
    {
        val_return = false;
        ErrorSDLPrint("SDL_PROP_APP_METADATA_TYPE_STRING");
    }

    // Initialize SDL3 video subsystem
    if (!SDL_Init(SDL_INIT_VIDEO))
        ErrorCrashSDLPrint("SDL_INIT_VIDEO");

    // Create window, renderer, and textur
    InitWindow(metadata);
    InitRenderer(r, g, b, a, &val_return);
    InitTexture();

    // Use nearest-neighbor scaling (no blurring when window is resized)
    SDL_SetTextureScaleMode(texture, SDL_SCALEMODE_NEAREST);

    // Connect main loop to window state and setup handler
    run_true = run;
    *run_true = true;
    signal(SIGINT, SignalHandler);

    return val_return;
}

// Free all SDL resources
void Renderer2DDestroy()
{
    if (texture != NULL)
        SDL_DestroyTexture(texture);
    if (renderer != NULL)
        SDL_DestroyRenderer(renderer);
    if (window != NULL)
        SDL_DestroyWindow(window);
    SDL_Quit();
}

// Copy pixel data to texture and render it to the screen
bool Renderer2DSetBufferCollor(void *new_buffer)
{
    bool val_return = true;
    int pitch;
    void *old_buffer;

    // Lock texture for writing
    if (!SDL_LockTexture(texture, NULL, &old_buffer, &pitch))
    {
        val_return = false;
        ErrorSDLPrint("SDL_LOCK_TEXTURE");
    }

    // Copy the new pixel data into the locked texture
    memcpy(old_buffer, new_buffer, pitch * _buffer_size_y);

    // Unlock texture and queue it for rendering
    SDL_UnlockTexture(texture);
    SDL_RenderTexture(renderer, texture, NULL, NULL);
    return val_return;
}

// Send the rendered frame to the display
bool Renderer2DPresent()
{
    return SDL_RenderPresent(renderer);
}

// Handle SDL events
void Renderer2DCheckEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_EVENT_QUIT)
        {
            *run_true = false;
        }
    }
}