#ifndef BLOCK
#define BLOCK

#include "mygbalib.h"
#include "leveldata.h"
#include "game.h"

#include "player.h"
#include "plate.h"

#define BLOCKCOUNT 1

typedef struct {

    OBJECT obj;

} BLOCK;

extern OBJECT blocks[BLOCKCOUNT];

void initBlocks(COORDINATE*);
void updateBlocks();
void moveBlocks();
void drawBlocks();

#endif