#ifndef BLOCK_OBJ
#define BLOCK_OBJ

#include "mygbalib.h"
#include "leveldata.h"
#include "game.h"

#include "player.h"
#include "plate.h"

typedef struct {

    OBJECT obj;

} BLOCK;

extern BLOCK blocks[BLOCKCOUNT];

void initBlocks(LEVEL);
void updateBlocks();
void moveBlocks();
void drawBlocks();

#endif