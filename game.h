#ifndef GAME_LOOP
#define GAME_LOOP

#include "mygbalib.h"
#include "leveldata.h"
#include "player.h"
#include "block.h"
#include "plate.h"
#include "tool.h"

enum { FRONTWALK, BACKWALK, RIGHTWALK, LEFTWALK, IDLE };

extern int gamePaused;
extern int gameWon;

extern int level;
extern LEVEL* levelData;

void initGame();
void updateGame();
void drawGame();

void updateLevel();

int canMoveUp(OBJECT*);
int canMoveDown(OBJECT*);
int canMoveLeft(OBJECT*);
int canMoveRight(OBJECT*);

void moveUp(OBJECT*);
void moveDown(OBJECT*);
void moveLeft(OBJECT*);
void moveRight(OBJECT*);

void readInput(OBJECT*);

void pickaxeFunction();

#endif