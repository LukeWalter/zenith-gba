#ifndef TOOL_OBJ
#define TOOL_OBJ

#include "mygbalib.h"
#include "leveldata.h"
#include "game.h"

#include "player.h"
#include "plate.h"
#include "block.h"

typedef struct {

    OBJECT obj;
    void (*ability)(void);

} TOOL;

extern TOOL tools[TOOLCOUNT];

void initTools(LEVEL);
void drawTools();

#endif