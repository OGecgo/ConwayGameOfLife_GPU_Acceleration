#ifndef GOF_H
#define GOF_H

#include "../utils/Bitmap.h"

typedef struct GoF GoF;

// Initialize the Game of Life engine and create Bitmap
GoF* GoFInit();
// Free all allocated memory exept bitmap
void GoFDestroy(GoF* gof);


// Compute one generation: apply Conway's rules and update internal state
// Updates the internal bitmap that can be retrieved with GoFGetBitmap()
void GoFUpdate(GoF* gof);
// Get the current game state bitmap (automatically updated by GoFUpdate)
Bitmap* GoFGetBitmap(GoF* gof);
// Sync external bitmap changes back into the GoF engine
// Call this after modifying the bitmap from outside 
void GoFUpdateBitmap(GoF* gof);

int GoFGetLive(GoF* gof);
int GoFGetDeaths(GoF* gof);


#endif // GOF_H