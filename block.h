#ifndef BLOCK_OBJ
#define BLOCK_OBJ

#include "mygbalib.h"
#include "leveldata.h"

typedef struct {

    OBJECT obj;

} BLOCK;

extern int numBlocks;
extern BLOCK blocks[BLOCKCOUNT];

void initBlocks(LEVEL);
void updateBlocks();
void moveBlocks();
void drawBlocks();

#endif