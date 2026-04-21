#ifndef BITMAP_H
#define BITMAP_H

#include <stdbool.h>

typedef struct
{
    bool* map;
    int width;
    int height;
} Bitmap;

Bitmap* BitmapInit(int height, int width);
void BitmapDestroy(Bitmap* bitmap);

void BitmapClean(Bitmap* bitmap);

#endif // BITMAP_H