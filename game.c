#include <stdlib.h>

#include "game.h"
#include "mygbalib.h"

#include "spritesheet.h"

typedef struct {

    int worldRow;
    int worldCol;
    int encodeFactor;
    int encodeWorldRow;
    int encodeWorldCol;
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
    int idle;
    int active;
    ANISPRITE sprite;

} OBJECT;

OBJECT zenith;

int hOff;
int vOff;

int gameOver;
int gameWon;

int mapWidth;
int mapHeight;

void initZenith();
void updateZenith();
void animateZenith();
void drawZenith();

void moveUp(OBJECT* obj);
void moveDown(OBJECT* obj);
void moveLeft(OBJECT* obj);
void moveRight(OBJECT* obj);

void initGame() {
    
    hOff = 0;
    vOff = 0;

    gameOver = 0;
    gameWon = 0;

    mapWidth = 11;
    mapHeight = 7;

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

    zenith.xPos = 0;
    zenith.yPos = 0;
    zenith.xTarget = zenith.xPos;
    zenith.yTarget = zenith.yPos;
    zenith.idle = 1;
    zenith.active = 1;

    zenith.sprite.worldRow = 24 + zenith.yPos * 16 + vOff;
    zenith.sprite.worldCol = 32 + zenith.xPos * 16 + hOff;

    zenith.sprite.encodeFactor = 8;
    zenith.sprite.encodeWorldRow = zenith.sprite.worldRow * zenith.sprite.encodeFactor;
    zenith.sprite.encodeWorldCol = zenith.sprite.worldCol * zenith.sprite.encodeFactor;

    zenith.sprite.rdel = 8;
    zenith.sprite.cdel = 8;

    zenith.sprite.width = 16;
    zenith.sprite.height = 16;

    zenith.sprite.aniCounter = 0;
    zenith.sprite.aniState = FRONTWALK;
    zenith.sprite.curFrame = 0;
    zenith.sprite.numFrames = 3;
    zenith.sprite.hide = 0;

    zenith.sprite.attributes = &shadowOAM[0];

} // initZenith

void updateZenith() {

    if (zenith.idle) {

        if (BUTTON_PRESSED(BUTTON_UP) && zenith.yTarget - 1 > -1)                zenith.yTarget--;
        else if (BUTTON_PRESSED(BUTTON_DOWN) && zenith.yTarget + 1 < mapHeight)  zenith.yTarget++;
        else if (BUTTON_PRESSED(BUTTON_LEFT) && zenith.xTarget - 1 > -1)         zenith.xTarget--;
        else if (BUTTON_PRESSED(BUTTON_RIGHT) && zenith.xTarget + 1 < mapWidth)  zenith.xTarget++;

    } // if

    if (zenith.yTarget < zenith.yPos)      { zenith.idle = 0; moveUp(&zenith); }
    else if (zenith.yTarget > zenith.yPos) { zenith.idle = 0; moveDown(&zenith); }
    else if (zenith.xTarget < zenith.xPos) { zenith.idle = 0; moveLeft(&zenith); }
    else if (zenith.xTarget > zenith.xPos) { zenith.idle = 0; moveRight(&zenith); }

    animateZenith();

} // updateZenith

void animateZenith() {

    zenith.sprite.prevAniState = zenith.sprite.aniState;
    zenith.sprite.aniState = IDLE;

    if(zenith.sprite.aniCounter % 20 == 0) {
        zenith.sprite.curFrame = (zenith.sprite.curFrame + 1) % zenith.sprite.numFrames;
    
    } // if

    if (BUTTON_HELD(BUTTON_UP))    zenith.sprite.aniState = BACKWALK;
    if (BUTTON_HELD(BUTTON_DOWN))  zenith.sprite.aniState = FRONTWALK;
    if (BUTTON_HELD(BUTTON_LEFT))  zenith.sprite.aniState = LEFTWALK;
    if (BUTTON_HELD(BUTTON_RIGHT)) zenith.sprite.aniState = RIGHTWALK;


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

void moveUp(OBJECT* obj) {

    if (obj->sprite.worldRow <= 24 + obj->yTarget * 16 + vOff) {

        obj->yPos = obj->yTarget;
        obj->sprite.worldRow = 24 + obj->yTarget * 16 + vOff;
        obj->sprite.encodeWorldRow = zenith.sprite.worldRow * zenith.sprite.encodeFactor;
        
        if (BUTTON_HELD(BUTTON_UP) && zenith.yTarget - 1 > -1) {
            obj->yTarget--;
            moveUp(obj);

        } else {
            obj->idle = 1;

        } // if

    } else {

        obj->sprite.encodeWorldRow -= obj->sprite.rdel;
        obj->sprite.worldRow = obj->sprite.encodeWorldRow / obj->sprite.encodeFactor;

        if ((obj->sprite.worldRow - vOff < SCREENHEIGHT / 2 - obj->sprite.height / 2) && (vOff > 0)) {
            vOff--;
    
        } // if

    } // if

} // moveUp

void moveDown(OBJECT* obj) {
    
    if (zenith.sprite.worldRow >= 24 + obj->yTarget * 16 + vOff) {

        obj->yPos = obj->yTarget;
        obj->sprite.worldRow = 24 + obj->yPos * 16 + vOff;
        obj->sprite.encodeWorldRow = 8 * obj->sprite.worldRow;
        
        if (BUTTON_HELD(BUTTON_DOWN) && zenith.yTarget + 1 < mapHeight) {
            obj->yTarget++;
            moveDown(obj);

        } else {
            obj->idle = 1;

        } // if

    } else {

        obj->sprite.encodeWorldRow += obj->sprite.rdel;
        obj->sprite.worldRow = obj->sprite.encodeWorldRow / obj->sprite.encodeFactor;

        if ((obj->sprite.worldRow - vOff > SCREENHEIGHT / 2 - obj->sprite.height / 2) && (vOff < mapHeight - SCREENHEIGHT)) { 
            vOff++;
        
        } // if

    } // if

} // moveDown

void moveLeft(OBJECT* obj) {

    if (obj->sprite.worldCol <= 32 + obj->xTarget * 16 + hOff) {

        obj->xPos = obj->xTarget;
        obj->sprite.worldCol = 32 + obj->xTarget * 16 + hOff;
        obj->sprite.encodeWorldCol = zenith.sprite.worldCol * zenith.sprite.encodeFactor;
        
        if (BUTTON_HELD(BUTTON_LEFT) && zenith.xTarget - 1 > -1) {
            obj->xTarget--;
            moveLeft(obj);

        } else {
            obj->idle = 1;

        } // if

    } else {

        obj->sprite.encodeWorldCol -= obj->sprite.cdel;
        obj->sprite.worldCol = obj->sprite.encodeWorldCol / obj->sprite.encodeFactor;

        if ((obj->sprite.worldCol - hOff < SCREENWIDTH / 2 - obj->sprite.width / 2) && (hOff > 0)) {
            vOff--;
    
        } // if

    } // if

} // moveLeft

void moveRight(OBJECT* obj) {

    if (obj->sprite.worldCol >= 32 + obj->xTarget * 16 + hOff) {

        obj->xPos = obj->xTarget;
        obj->sprite.worldCol = 32 + obj->xPos * 16 + hOff;
        obj->sprite.encodeWorldCol = 8 * obj->sprite.worldCol;
        
        if (BUTTON_HELD(BUTTON_RIGHT) && zenith.xTarget + 1 < mapWidth) {
            obj->xTarget++;
            moveRight(obj);

        } else {
            obj->idle = 1;

        } // if

    } else {

        obj->sprite.encodeWorldCol += obj->sprite.cdel;
        obj->sprite.worldCol = obj->sprite.encodeWorldCol / obj->sprite.encodeFactor;

        if ((obj->sprite.worldCol - hOff > SCREENWIDTH / 2 - obj->sprite.width / 2) && (hOff < mapWidth - SCREENWIDTH)) {
            vOff--;
    
        } // if

    } // if

} // moveRight