#ifndef PLATE_OBJ
#define PLATE_OBJ

#include "mygbalib.h"
#include "leveldata.h"

#include "game.h"
#include "player.h"
#include "block.h"

typedef struct {

    OBJECT obj;
    int pressed;
    void (*onFunc)(void);
    void (*offFunc)(void);

} PLATE;

extern PLATE plates[PLATECOUNT];

void initPlates(LEVEL);
void updatePlates();
void movePlates();
void drawPlates();

#endif