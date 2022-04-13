#ifndef PLATE_OBJ
#define PLATE_OBJ

#include "mygbalib.h"
#include "leveldata.h"

#include "game.h"
#include "player.h"
#include "block.h"

#define PLATECOUNT 1

typedef struct {

    OBJECT obj;

} PLATE;

extern OBJECT plates[PLATECOUNT];

void initPlates(COORDINATE*);
void updatePlates();
void movePlates();
void drawPlates();

#endif