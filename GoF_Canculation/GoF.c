#include "GoF.h"
#include "GoFConfig.h"

#include <stdlib.h>
#include <string.h>

#include <stdio.h>


struct GoF{
    Bitmap* bitmap;
    bool* copy_map;
    int live;
    int deaths;
};


GoF* GoFInit(){
    GoF* gof = malloc(sizeof(GoF));
    gof->bitmap = BitmapInit(MAP_HEIGHT, MAP_WIDTH);
    gof->copy_map = malloc(sizeof(bool) * gof->bitmap->size);
    memcpy(gof->copy_map, gof->bitmap->map, sizeof(bool) * gof->bitmap->size);
    return gof;
}

void GoFDestroy(GoF* gof){
    if (gof != NULL)
        if (gof->copy_map != NULL)
            free(gof->copy_map);
        free(gof);
}




void GoFUpdate(GoF* gof){
    // setup copy map
    memcpy(gof->copy_map, gof->bitmap->map, sizeof(bool) * gof->bitmap->size);
    gof->live = 0;
    gof->deaths = 0;

    for (int pos = 0; pos < gof->bitmap->size; pos++){
        int lifes = 0;

        // count of life
        // -n , ... , 0 , ... , n
        for (int h = -INTERACT_BLOCKS_AROUND; h < INTERACT_BLOCKS_AROUND + 1; h++){
            for (int w = -INTERACT_BLOCKS_AROUND; w < INTERACT_BLOCKS_AROUND + 1; w++){
                int check_pos = pos + h * MAP_WIDTH + w;
                // do not go out of buffer
                if (check_pos >= 0 && check_pos < MAP_HEIGHT * MAP_WIDTH) {
                    // dont change row 
                    int row_test = check_pos % MAP_WIDTH - w;
                    if (row_test < MAP_WIDTH && row_test >= 0) {
                        lifes += gof->copy_map[check_pos];
                    }
                }
            }
        }
        

        // birth - death block
        bool current_state = gof->bitmap->map[pos];
        bool next_state = current_state;
        if (current_state == true){
            lifes --;
            next_state = (lifes >= MIN_LIFES_FOR_SURVIVE && lifes <= MAX_LIFES_FOR_SURVIVE);
        }else{
            next_state = (lifes >= MIN_LIFES_FOR_BIRTH && lifes <= MAX_LIFES_FOR_BIRTH);
        }
        gof->bitmap->map[pos] = next_state;
        // if next_state true add live
        // if next_state false add deaths
        gof->live = next_state;
        gof->deaths = !next_state;
    
    }
    
}

Bitmap* GoFGetBitmap(GoF* gof){
    return gof->bitmap;
}

void GoFUpdateBitmap(GoF* gof){
    /* GoF in c use one bitmap */
}

int GoFGetLive(GoF* gof){
    return gof->live;
}
int GoFGetDeaths(GoF* gof){
    return gof->deaths;
}