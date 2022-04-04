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
int mapXOffset;
int mapYOffset;

void initZenith();
void updateZenith();
void moveZenith();
void drawZenith();

int canMoveUp(OBJECT* obj);
int canMoveDown(OBJECT* obj);
int canMoveLeft(OBJECT* obj);
int canMoveRight(OBJECT* obj);

void moveUp(OBJECT* obj);
void moveDown(OBJECT* obj);
void moveLeft(OBJECT* obj);
void moveRight(OBJECT* obj);

void readInput(OBJECT* obj);


void initGame() {
    
    hOff = 0;
    vOff = 0;

    gameOver = 0;
    gameWon = 0;

    mapWidth = 11;
    mapHeight = 7;
    mapXOffset = 32;
    mapYOffset = 24;

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

    zenith.sprite.worldRow = mapYOffset + zenith.yPos * 16 + vOff;
    zenith.sprite.worldCol = mapXOffset + zenith.xPos * 16 + hOff;

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

    moveZenith();

} // updateZenith

void moveZenith() {

    zenith.sprite.prevAniState = zenith.sprite.aniState;
    if (zenith.idle) zenith.sprite.aniState = IDLE;

    if (zenith.sprite.aniCounter % 15 == 0) {
        zenith.sprite.curFrame = (zenith.sprite.curFrame + 1) % zenith.sprite.numFrames;
    
    } // if

    readInput(&zenith);

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

int canMoveUp(OBJECT* obj) {

    if (obj->yTarget - 1 > -1) {
        return 1;

    } // if

    return 0;

} // canMoveUp

int canMoveDown(OBJECT* obj) {

    if (obj->yTarget + 1 < mapHeight) {
        return 1;

    } // if

    return 0;

} // canMoveDown

int canMoveLeft(OBJECT* obj) {

    if (obj->xTarget - 1 > -1) {
        return 1;

    } // if

    return 0;

} // canMoveLeft

int canMoveRight(OBJECT* obj) {

    if (obj->xTarget + 1 < mapWidth) {
        return 1;

    } // if

    return 0;

} // canMoveRight

void moveUp(OBJECT* obj) {

    obj->idle = 0;

    if (obj->sprite.worldRow <= mapYOffset + obj->yTarget * 16 + vOff) {

        obj->yPos = obj->yTarget;
        obj->sprite.worldRow = mapYOffset + obj->yTarget * 16 + vOff;
        obj->sprite.encodeWorldRow = obj->sprite.worldRow * obj->sprite.encodeFactor;
        
        if (BUTTON_HELD(BUTTON_UP) && canMoveUp(obj)) {
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
    
    obj->idle = 0;

    if (obj->sprite.worldRow >= mapYOffset + obj->yTarget * 16 + vOff) {

        obj->yPos = obj->yTarget;
        obj->sprite.worldRow = mapYOffset + obj->yPos * 16 + vOff;
        obj->sprite.encodeWorldRow = 8 * obj->sprite.worldRow;
        
        if (BUTTON_HELD(BUTTON_DOWN) && canMoveDown(obj)) {
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

    obj->idle = 0;

    if (obj->sprite.worldCol <= mapXOffset + obj->xTarget * 16 + hOff) {

        obj->xPos = obj->xTarget;
        obj->sprite.worldCol = mapXOffset + obj->xTarget * 16 + hOff;
        obj->sprite.encodeWorldCol = obj->sprite.worldCol * obj->sprite.encodeFactor;
        
        if (BUTTON_HELD(BUTTON_LEFT) && canMoveLeft(obj)) {
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

    obj->idle = 0;

    if (obj->sprite.worldCol >= mapXOffset + obj->xTarget * 16 + hOff) {

        obj->xPos = obj->xTarget;
        obj->sprite.worldCol = mapXOffset + obj->xPos * 16 + hOff;
        obj->sprite.encodeWorldCol = 8 * obj->sprite.worldCol;
        
        if (BUTTON_HELD(BUTTON_RIGHT) && canMoveRight(obj)) {
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

void readInput(OBJECT* obj) {

    if (obj->idle) {
        
        if (BUTTON_HELD(BUTTON_UP) && canMoveUp(obj)) {
            obj->sprite.aniState = BACKWALK;
            obj->yTarget--;
            
        } else if (BUTTON_HELD(BUTTON_DOWN) && canMoveDown(obj)) {
            obj->sprite.aniState = FRONTWALK;
            obj->yTarget++;
            
        } else if (BUTTON_HELD(BUTTON_LEFT) && canMoveLeft(obj)) {
            obj->sprite.aniState = LEFTWALK;
            obj->xTarget--;

        } else if (BUTTON_HELD(BUTTON_RIGHT) && canMoveRight(obj)) {
            obj->sprite.aniState = RIGHTWALK;
            obj->xTarget++;

        } else if (BUTTON_HELD(BUTTON_UP)) {
            obj->sprite.aniState = BACKWALK;
            
        } else if (BUTTON_HELD(BUTTON_DOWN)) {
            obj->sprite.aniState = FRONTWALK;
            
        } else if (BUTTON_HELD(BUTTON_LEFT)) {
            obj->sprite.aniState = LEFTWALK;

        } else if (BUTTON_HELD(BUTTON_RIGHT)) {
            obj->sprite.aniState = RIGHTWALK;

        } // if

    } // if

    if (obj->yTarget < obj->yPos)      moveUp(obj);
    else if (obj->yTarget > obj->yPos) moveDown(obj);
    else if (obj->xTarget < obj->xPos) moveLeft(obj);
    else if (obj->xTarget > obj->xPos) moveRight(obj);

} // readInput