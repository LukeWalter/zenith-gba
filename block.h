#ifndef BLOCK_OBJ
#define BLOCK_OBJ

#include "mygbalib.h"
#include "leveldata.h"
#include "game.h"

#include "player.h"
#include "plate.h"

#define BLOCKCOUNT 1

typedef struct {

    OBJECT obj;

} BLOCK;

extern BLOCK blocks[BLOCKCOUNT];

void initBlocks(COORDINATE*);
void updateBlocks();
void moveBlocks();
void drawBlocks();

#endif