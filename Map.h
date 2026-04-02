#ifndef MAP_H
#define MAP_H

#include <stdbool.h>

// bitmap use the default data for window from WindowData.h


// you draw new values to bitmap
typedef struct
{
    bool* map_of_blocks;
    int width_bitmap;
    int height_bitmap;
} Bitmap;

// run_main_while will seted true
Bitmap* MapInit(int width_bitmap, int height_bitmap, bool* run_main_while);
// destory bitmap
void MapBitmapDestroy(Bitmap* bitmap);
// clear whatever create
void MapDestroy();

// every frame need to clean bitmap
void MapCleanBitmap(Bitmap* bitmap);

// set bitmap and present it
void MapSetNewBitmap(Bitmap* bitmap);


#endif // MAP_H