#ifndef MAP_H
#define MAP_H

#include <stdbool.h>

#include "Bitmap.h"

// Map use the default data for window from WindowData.h




// run_main_while will seted true
void MapInit(Bitmap* bitmap, bool* run_main_while);

// clear whatever create
void MapDestroy();

// update bitmap and present it
void MapUpdateBitmap(Bitmap* bitmap);


#endif // MAP_H