#ifndef PLAYER_OBJ
#define PLAYER_OBJ

#include "mygbalib.h"
#include "leveldata.h"
#include "game.h"

#include "tool.h"
#include "block.h"
#include "plate.h"

typedef struct {

    OBJECT obj;
    // TOOL equippedTool;

} PLAYER;

void initZenith(LEVEL);
void updateZenith();
void moveZenith();
void drawZenith();

extern PLAYER zenith;

#endif