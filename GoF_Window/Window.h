#ifndef WINDOW_H
#define WINDOW_H

#include <stdbool.h>

#include "../utils/Bitmap.h"

// Window use the default data for window from WindowData.h




// run_main_while will seted true
void WindowInit(Bitmap* bitmap, bool* run_main_while);

// clear whatever create
void WindowDestroy();

// update bitmap and present it
void WindowUpdateBitmap(Bitmap* bitmap);


#endif // WINDOW_H