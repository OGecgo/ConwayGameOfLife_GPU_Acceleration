#include "GoF.h"
#include "GoFConfig.h"

#include <stdlib.h>
#include <string.h>

#include <stdio.h>


struct GoF{
    Bitmap* bitmap;
    bool* copy_map; // current generation
    bool* map;      // next generation
    int live;
    int deaths;
};


GoF* GoFInit(){
    GoF* gof = malloc(sizeof(GoF));
    gof->bitmap = BitmapInit(MAP_HEIGHT, MAP_WIDTH);
    gof->copy_map = malloc(sizeof(bool) * gof->bitmap->size);
    gof->map      = malloc(sizeof(bool) * gof->bitmap->size);
    // set by default values (false) 
    memset(gof->copy_map, false, sizeof(bool) * gof->bitmap->size);
    memset(gof->map, false, sizeof(bool) * gof->bitmap->size);

    return gof;
}

void GoFDestroy(GoF* gof){
    if (gof != NULL){
        if (gof->copy_map != NULL){
            free(gof->copy_map);
        }
        if (gof->map != NULL){
            free(gof->map);
        }
        free(gof);
    }
}




void GoFUpdate(GoF* gof){
    gof->live = 0;
    gof->deaths = 0;

    for (int pos = 0; pos < gof->bitmap->size; pos++){
        // count neighbors in a square around cell
        int lifes = 0;
        // -n , ... , 0 , ... , n
        for (int h = -INTERACT_BLOCKS_AROUND; h < INTERACT_BLOCKS_AROUND + 1; h++){
            for (int w = -INTERACT_BLOCKS_AROUND; w < INTERACT_BLOCKS_AROUND + 1; w++){
                int check_pos = pos + h * MAP_WIDTH + w;
                // Bounds check: stay within buffer
                if (check_pos >= 0 && check_pos < MAP_HEIGHT * MAP_WIDTH) {
                    // Bounds check: don't wrap to adjacent rows
                    int row_test = check_pos % MAP_WIDTH - w;
                    if (row_test < MAP_WIDTH && row_test >= 0) {
                        lifes += gof->copy_map[check_pos];
                    }
                }
            }
        }
        

        // survive, birth or death
        bool current_state = gof->copy_map[pos];
        bool next_state = current_state;
        if (current_state == true){
            lifes --; // Don't count itself
            next_state = (lifes >= MIN_LIFES_FOR_SURVIVE && lifes <= MAX_LIFES_FOR_SURVIVE);
        }else{
            next_state = (lifes >= MIN_LIFES_FOR_BIRTH && lifes <= MAX_LIFES_FOR_BIRTH);
        }
        gof->map[pos] = next_state;
        // Count statistics for this generation
        gof->live += next_state; 
        gof->deaths += !next_state; 
    }


    // Copy computed generation to bitmap (transfer data)
    memcpy(gof->bitmap->map, gof->map, sizeof(bool) * gof->bitmap->size);
    // Swap buffers: current becomes previous, computed becomes current
    bool* temp = gof->copy_map;
    gof->copy_map = gof->map;
    gof->map = temp; 
}

Bitmap* GoFGetBitmap(GoF* gof){
    return gof->bitmap;
}

void GoFUpdateBitmap(GoF* gof){
    memcpy(gof->copy_map, gof->bitmap->map, sizeof(bool) * gof->bitmap->size);
}

int GoFGetLive(GoF* gof){
    return gof->live;
}
int GoFGetDeaths(GoF* gof){
    return gof->deaths;
}