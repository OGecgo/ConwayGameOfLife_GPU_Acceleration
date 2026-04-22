#include "Bitmap.h"

#include <stdlib.h>
#include <string.h>

Bitmap* BitmapInit(int height, int width){
    Bitmap* bitmap = malloc(sizeof(Bitmap));
    bitmap->height = height;
    bitmap->width  = width;
    bitmap->size = height * width;
    bitmap->map = malloc(sizeof(bool) * bitmap->size);
    memset(bitmap->map, false, bitmap->size);
    return bitmap;
}

void BitmapDestroy(Bitmap* bitmap){
    free(bitmap->map);
    free(bitmap);
}

void BitmapClean(Bitmap* bitmap){
    memset(bitmap->map, false, bitmap->height * bitmap->width);
}