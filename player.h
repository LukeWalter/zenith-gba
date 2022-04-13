#ifndef PLAYER
#define PLAYER

#include "mygbalib.h"
#include "leveldata.h"

#include "game.h"
#include "block.h"
#include "plate.h"

typedef struct {

    OBJECT obj;

} PLAYER;

void initZenith(int, COORDINATE);
void updateZenith();
void moveZenith();
void drawZenith();

extern OBJECT zenith;

#endif