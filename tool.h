#ifndef TOOL_OBJ
#define TOOL_OBJ

#include "mygbalib.h"
#include "leveldata.h"

typedef struct {

    OBJECT obj;
    void (*ability)(void);

} TOOL;

extern int numTools;
extern TOOL tools[TOOLCOUNT];

void initTools(LEVEL);
void drawTools();

#endif