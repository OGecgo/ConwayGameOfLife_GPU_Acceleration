#include "Map.h"

#include "WindowData.h"
#include "Renderer2D.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <time.h>

// bitmap translated to array of colors
// static array of rgba colors
static uint32_t *rgba8888;

static void ConverBitmapToRGBA8888(Bitmap *bitmap)
{
    int size_b = bitmap->height_bitmap * bitmap->width_bitmap;

    for (int i = 0; i < size_b; i++)
    {
        rgba8888[i] = bitmap->map_of_blocks[i] ? 0xFFFFFFFF : 0x000000FF;
    }
}

Bitmap *MapInit(int width_bitmap, int height_bitmap, bool *run_main_while)
{
    // set metadata
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

    // black backsreen
    if (!Renderer2DInit(metadata, run_main_while, 0, 0, 0, 1, width_bitmap, height_bitmap))
        printf("\nError Renderer2DInit\n");
    // do not need again
    free(metadata);

    int size_map = width_bitmap * height_bitmap;

    // set pixels rendering black
    rgba8888 = malloc(sizeof(uint32_t) * size_map);
    memset(rgba8888, 0x000000FF, size_map);

    // set bitmap and retunr
    Bitmap *bitmap = malloc(sizeof(Bitmap));
    bitmap->height_bitmap = height_bitmap;
    bitmap->width_bitmap = width_bitmap;
    bitmap->map_of_blocks = malloc(sizeof(bool) * size_map);
    memset(bitmap->map_of_blocks, false, size_map);
    return bitmap;
}

void MapBitmapDestroy(Bitmap *bitmap)
{
    free(bitmap->map_of_blocks);
    free(bitmap);
}

void MapDestroy()
{
    Renderer2DDestroy();
}

// every frame need to clean bitmap
void MapCleanBitmap(Bitmap *bitmap)
{
    memset(bitmap->map_of_blocks, false, bitmap->height_bitmap * bitmap->width_bitmap);
}

// set bitmap and present it
void MapSetNewBitmap(Bitmap *bitmap)
{
    ConverBitmapToRGBA8888(bitmap);

    Renderer2DCheckEvents();

    if (!Renderer2DSetBufferCollor(rgba8888))
        printf("Error Buffer color\n");

    if (!Renderer2DPresent())
        printf("Error rendering present\n");
}