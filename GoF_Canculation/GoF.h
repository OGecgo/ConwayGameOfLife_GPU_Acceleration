#ifndef GOF_H
#define GOF_H
// compiler nvcc with extern c link name like a c 
// main will compile like a c and they will waite liking like a c


#include "../utils/Bitmap.h"

typedef struct GoF GoF;



// create the bitmap
GoF* GoFInit();
// do not destroy the bitmap
void GoFDestroy(GoF* gof);


// GoFUpdateBitmap (automaticly update the bitmap memmory what given on GoFInit)
void GoFUpdateBitmap(GoF* gof);

Bitmap* GoFGetBitmap(GoF* gof);
// return the value of last step
int GoFGetLive(GoF* gof);
int GoFGetDeaths(GoF* gof);


#endif // GOF_H