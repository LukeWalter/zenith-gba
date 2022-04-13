#ifndef GAME
#define GAME

#include "mygbalib.h"
#include "leveldata.h"
#include "player.h"
#include "block.h"
#include "plate.h"

enum { FRONTWALK, BACKWALK, RIGHTWALK, LEFTWALK, IDLE };

extern int gameOver;
extern int gameWon;

void initGame();
void updateGame();
void drawGame();

void initLevel(int);

int canMoveUp(OBJECT*);
int canMoveDown(OBJECT*);
int canMoveLeft(OBJECT*);
int canMoveRight(OBJECT*);

void moveUp(OBJECT*);
void moveDown(OBJECT*);
void moveLeft(OBJECT*);
void moveRight(OBJECT*);

void readInput(OBJECT*);

#endif