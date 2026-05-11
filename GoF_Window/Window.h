#ifndef WINDOW_H
#define WINDOW_H

#include <stdbool.h>

#include "../utils/Bitmap.h"

// Note: This module automatically uses the default window settings from GoF_Window/WindowData.h 

// Initializes the graphical window and allocates pixel memory.
// It also links 'run_main_while' to the window state (turns false when window is closed).
void WindowInit(Bitmap* bitmap, bool* run_main_while);

// Cleans up SDL rendering resources and frees the pixel buffer.
void WindowDestroy();

// Translates the game grid into colors and renders the new frame to the screen
void WindowUpdateBitmap(Bitmap* bitmap);


#endif // WINDOW_H