#ifndef GAMEOFLIFE_H
#define GAMEOFLIFE_H

#include "Map.h"

typedef struct
{
    Bitmap map;
} GameOfLife;

GameOfLife *GameOfLifeInit(int width_game, int height_game, bool* run_main_loop);
void GameOfLifeDestroy(GameOfLife *gof);

// for now only one preseted
void GameOfLifeGetPresetMapValue(GameOfLife *gof);

void GameOfLifeSetPositionLife(GameOfLife *gof, int pos_x, int pos_y);
void GameOfLifeSetPositionDeath(GameOfLife *gof, int pos_x, int pos_y);
// update every frame of game
// todo here seted if compile for nvidia gpu or not
// gcc -> linear execution of gameoflife
// nvcc -> paralel execution gameoflife (only for the logic of game)
// the lybrary what used for rendering axelerate the gpu
void GameOfLifeUpdate(GameOfLife *gof);

#endif // GAMEOFLIFE_H