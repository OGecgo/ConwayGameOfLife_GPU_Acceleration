/*
 * Simple 2D renderer abstraction layer for SDL3
 * Used for Conway's Game of Life GPU acceleration
 */

#ifndef RENDERER2D_H
#define RENDERER2D_H

// SDL3 library - include <SDL3/SDL_main.h> in main for proper initialization
#include <stdbool.h>
#include <stdint.h>

// Application metadata for SDL3 setup
typedef struct
{
    int window_size_x, window_size_y;    // Window size
    char *name_aplication; // App name
    char *version;         // Version string
    char *reverse_domain;  // Domain ID
    char *creator;         // Author
    char *copyright;       // Copyright info
    char *url;             // Project URL
    char *type_aplication; // App type
} Renderer2DMetada;


// Functions
// Initialize renderer with window settings and clear color
// run_true: controls main loop, set by events and SIGINT signal
// metadata can be freed after this call
bool Renderer2DInit
(
    Renderer2DMetada *metadata, bool *run_true, 
    uint8_t r, uint8_t g, uint8_t b, uint8_t a, 
    int buffer_size_x, int buffer_size_y
);

// Clean up SDL resources
void Renderer2DDestroy();

// Update color buffer (clear buffer before new data)
bool Renderer2DSetBufferCollor(void *new_buffer);

// Show current frame on screen
bool Renderer2DPresent();

// Process events (handles quit events)
// Note: may block during window drag/resize on some platforms
void Renderer2DCheckEvents();

#endif