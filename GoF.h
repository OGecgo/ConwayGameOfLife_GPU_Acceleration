#ifndef GAMEOFLIFE_H
#define GAMEOFLIFE_H

#include "Bitmap.h"

typedef struct
{
    Bitmap* bitmap;
    bool* copy_map;
} GoF;

// create the bitmap
GoF* GoFInit();
// do not destroy the bitmap
GoF* GoFDestroy(GoF* gof);

// GoFUpdateBitmap (automaticly update the bitmap memmory what given on GoFInit)
void GoFUpdateBitmap(GoF* gof);

Bitmap* GoFGetBitmap(GoF* gof);

#endif // GAMEOFLIFE_H