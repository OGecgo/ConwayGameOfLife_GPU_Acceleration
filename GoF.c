#include "GoF.h"
#include "GoFConfig.h"

#include <stdlib.h>
#include <string.h>

#include <stdio.h>

GoF* GoFInit(){
    GoF* gof = malloc(sizeof(GoF));
    gof->bitmap = BitmapInit(MAP_HEIGHT, MAP_WIDTH);
    gof->copy_map = malloc(sizeof(bool) * gof->bitmap->size);
    memcpy(gof->copy_map, gof->bitmap->map, sizeof(bool) * gof->bitmap->size);
    return gof;
}

GoF* GoFDestroy(GoF* gof){
    if (gof != NULL)
        if (gof->copy_map != NULL)
            free(gof->copy_map);
        free(gof);
}





void GoFUpdateBitmap(GoF* gof){
    // setup copy map
    memcpy(gof->copy_map, gof->bitmap->map, sizeof(bool) * gof->bitmap->size);


    for (int pos = 0; pos < gof->bitmap->size; pos++){
        int lifes = 0;

        // count of life
        // -n , ... , 0 , ... , n
        for (int h = -INTERACT_BLOCKS_AROUND; h < INTERACT_BLOCKS_AROUND + 1; h++){
            for (int w = -INTERACT_BLOCKS_AROUND; w < INTERACT_BLOCKS_AROUND + 1; w++){
                int check_pos;
                check_pos = pos + h * MAP_WIDTH + w;
                // height check (do not go out of buffer)
                if (check_pos < 0 || check_pos > gof->bitmap->size) continue;
                // width check (do not chaing layer)
                if (check_pos / MAP_WIDTH != (check_pos - w) / MAP_WIDTH) continue;

                // printf("%d\n", check_pos);

                if (gof->copy_map[check_pos] == true) lifes++;
            }
        }
        printf("%d\n", lifes);
        break;
    }
    
}

Bitmap* GoFGetBitmap(GoF* gof){
    return gof->bitmap;
}