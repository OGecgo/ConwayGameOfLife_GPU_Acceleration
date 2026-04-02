#ifndef LIFE_H
#define LIFE_H

#include "Map.h"
typedef struct
{
    Bitmap old_map;

}Life;


Life* LifeInit();
void LifeDestroy();

// update old_map to new_map
void LifeUpdate(Bitmap* old_map);

#endif //LIFE_H