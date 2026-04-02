#include "GameOfLife.h"

#include <stdlib.h>
#include <stdbool.h>

// helped value


GameOfLife* GameOfLifeInit(int width_game, int height_game, bool* run_main_loop){
    Bitmap* b = MapInit(width_game, height_game, run_main_loop);
    GameOfLife* gof = malloc(sizeof(GameOfLife));
    gof->map = *b;
    free(b);
    return gof;
}
void GameOfLifeDestroy(GameOfLife *gof){
    if(gof != NULL){
        MapBitmapDestroy(&gof->map);
    }
    MapDestroy();
}

// for now only one preseted
void GameOfLifeGetPresetMapValue(GameOfLife* gof){
    
    Bitmap b = gof->map;
    bool* map_of_blocks = b.map_of_blocks;
    for(int i = 0; i < b.width_bitmap * b.height_bitmap; i++){
        if (i%4 == 0)
            map_of_blocks[i] = true;
        // else if (i%5 == 0)
        //     map_of_blocks[i] = true;
    }
}

void GameOfLifeSetPositionLife(GameOfLife* gof, int pos_x, int pos_y){
    Bitmap b = gof->map;
    b.map_of_blocks[pos_y * b.width_bitmap + pos_x] = true;
}

void GameOfLifeSetPositionDeath(GameOfLife* gof, int pos_x, int pos_y){
    Bitmap b = gof->map;
    b.map_of_blocks[pos_y * b.width_bitmap + pos_x] = false;
}

// update every frame of game
void GameOfLifeUpdate(GameOfLife* gof){

    // MapCleanBitmap(&gof->map);
    // for now not worked life
    // bitmap
    MapSetNewBitmap(&gof->map);
}