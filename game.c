#include <stdlib.h>

#include "game.h"
#include "mygbalib.h"

#include "spritesheet.h"

typedef struct {

    int encodeFactor;
    int encodeWorldRow;
    int encodeWorldCol;
    int worldRow;
    int worldCol;
    int rdel;
    int cdel;
    int width;
    int height;
    int aniCounter;
    int aniState;
    int prevAniState;
    int curFrame;
    int numFrames;
    int hide;
    OBJ_ATTR* attributes;

} ANISPRITE;

typedef struct {
    
    int xPos;
    int yPos;
    int xTarget;
    int yTarget;
    int active;
    ANISPRITE sprite;

} PLAYER;

PLAYER zenith;

int hOff;
int vOff;

int gameOver;
int gameWon;

void initZenith();
void updateZenith();
void animateZenith();
void drawZenith();

void initGame() {
    
    hOff = 0;
    vOff = 0;

    gameOver = 0;
    gameWon = 0;

    DMANow(3, spritesheetPal, SPRITEPALETTE, spritesheetPalLen / 2);
    DMANow(3, spritesheetTiles, &CHARBLOCK[4], spritesheetTilesLen / 2);
    hideSprites();
    updateOAM();

    initZenith();

} // initGame

void updateGame() {
    updateZenith();

} // updateGame

void drawGame() {

    drawZenith();

    waitForVBlank();
    updateOAM();

    REG_BG1HOFF = hOff;
    REG_BG1VOFF = vOff;

} // drawGame

void initZenith() {

    zenith.sprite.encodeFactor = 8;
    zenith.sprite.encodeWorldRow = zenith.sprite.worldRow * zenith.sprite.encodeFactor;
    zenith.sprite.encodeWorldCol = zenith.sprite.worldCol * zenith.sprite.encodeFactor;

    zenith.sprite.worldRow = SCREENHEIGHT / 2 - zenith.sprite.width / 2 + vOff;
    zenith.sprite.worldCol = SCREENWIDTH / 2 - zenith.sprite.height / 2 + hOff;

    zenith.sprite.rdel = 6;
    zenith.sprite.cdel = 6;

    zenith.sprite.width = 16;
    zenith.sprite.height = 16;

    zenith.sprite.aniCounter = 0;
    zenith.sprite.aniState = FRONTWALK;
    zenith.sprite.curFrame = 0;
    zenith.sprite.numFrames = 3;
    zenith.sprite.hide = 0;

    zenith.sprite.attributes = &shadowOAM[0];

    zenith.active = 1;

} // initZenith

void updateZenith() {

    animateZenith();

} // updateZenith

void animateZenith() {

    zenith.sprite.prevAniState = zenith.sprite.aniState;
    zenith.sprite.aniState = IDLE;

    if(zenith.sprite.aniCounter % 20 == 0) {
        zenith.sprite.curFrame = (zenith.sprite.curFrame + 1) % zenith.sprite.numFrames;
    
    } // if

    if(BUTTON_HELD(BUTTON_UP))    zenith.sprite.aniState = BACKWALK;
    if(BUTTON_HELD(BUTTON_DOWN))  zenith.sprite.aniState = FRONTWALK;
    if(BUTTON_HELD(BUTTON_LEFT))  zenith.sprite.aniState = LEFTWALK;
    if(BUTTON_HELD(BUTTON_RIGHT)) zenith.sprite.aniState = RIGHTWALK;


    if (zenith.sprite.aniState == IDLE) {
        zenith.sprite.curFrame = 0;
        zenith.sprite.aniCounter = 0;
        zenith.sprite.aniState = zenith.sprite.prevAniState;

    } else {
        zenith.sprite.aniCounter++;

    } // if

} // animateZenith

void drawZenith() {
    
    if (zenith.sprite.hide) {
        zenith.sprite.attributes->attr0 |= ATTR0_HIDE;

    } else {
        zenith.sprite.attributes->attr0 = (zenith.sprite.worldRow - vOff) | ATTR0_SQUARE;
        zenith.sprite.attributes->attr1 = (zenith.sprite.worldCol - hOff) | ATTR1_SMALL;
        zenith.sprite.attributes->attr2 = ATTR2_PALROW(0) | ATTR2_TILEID(zenith.sprite.aniState * 2, zenith.sprite.curFrame * 2);
    
    } // if

} // drawZenith