#include <stdlib.h>

#include "game.h"
#include "mygbalib.h"
#include "print.h"

#include "spritesheet.h"
#include "cave.h"
#include "house.h"

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
OBJECT block;

int gameOver;
int gameWon;

int mapWidth;
int mapHeight;
int mapXOffset;
int mapYOffset;

int hOff;
int vOff;

void initZenith();
void updateZenith();
void moveZenith();
void drawZenith();

void initBlock();
void updateBlock();
void moveBlock();
void drawBlock();

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

    gameOver = 0;
    gameWon = 0;

    // Measured in double tiles
    mapWidth = 11;
    mapHeight = 7;

    // Measured in single tiles
    mapXOffset = 4;
    mapYOffset = 3;

    hOff = 0;
    vOff = 0;

    DMANow(3, cavePal, PALETTE, 256);
    DMANow(3, caveTiles, &CHARBLOCK[0], caveTilesLen / 2);
    DMANow(3, caveMap, &SCREENBLOCK[28], caveMapLen / 2);
    REG_BG1VOFF = vOff;
    REG_BG1HOFF = hOff;

    DMANow(3, spritesheetPal, SPRITEPALETTE, spritesheetPalLen / 2);
    DMANow(3, spritesheetTiles, &CHARBLOCK[4], spritesheetTilesLen / 2);
    hideSprites();
    updateOAM();

    initZenith();
    initBlock();

} // initGame

void updateGame() {
    updateZenith();

} // updateGame

void drawGame() {

    drawZenith();
    drawBlock();

    waitForVBlank();
    updateOAM();

    REG_BG1VOFF = vOff;
    REG_BG1HOFF = hOff;

} // drawGame

void initZenith() {

    block.xPos = 0;
    zenith.yPos = 0;
    zenith.xTarget = zenith.xPos;
    zenith.yTarget = zenith.yPos;
    zenith.idle = 1;
    zenith.active = 1;

    zenith.sprite.worldRow = 8 * mapYOffset + zenith.yPos * 16;
    zenith.sprite.worldCol = 8 * mapXOffset + zenith.xPos * 16;

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

void initBlock() {

    block.xPos = 3;
    block.yPos = 3;
    block.xTarget = block.xPos;
    block.yTarget = block.yPos;
    block.idle = 1;
    block.active = 1;

    block.sprite.worldRow = 8 * mapYOffset + block.yPos * 16;
    block.sprite.worldCol = 8 * mapXOffset + block.xPos * 16;

    block.sprite.encodeFactor = 8;
    block.sprite.encodeWorldRow = block.sprite.worldRow * block.sprite.encodeFactor;
    block.sprite.encodeWorldCol = block.sprite.worldCol * block.sprite.encodeFactor;

    block.sprite.rdel = 4;
    block.sprite.cdel = 4;

    block.sprite.width = 16;
    block.sprite.height = 16;

    block.sprite.aniCounter = 0;
    block.sprite.aniState = BACKWALK;
    block.sprite.curFrame = 0;
    block.sprite.numFrames = 1;
    block.sprite.hide = 0;

    block.sprite.attributes = &shadowOAM[1];

} // initBlock

void updateBlock() {


} // updateBlock

void moveBlock() {


} // moveBlock

void drawBlock() {

    if (block.sprite.hide) {
        block.sprite.attributes->attr0 |= ATTR0_HIDE;

    } else {
        block.sprite.attributes->attr0 = (block.sprite.worldRow - vOff) | ATTR0_SQUARE;
        block.sprite.attributes->attr1 = (block.sprite.worldCol - hOff) | ATTR1_SMALL;
        block.sprite.attributes->attr2 = ATTR2_PALROW(0) | ATTR2_TILEID(block.sprite.aniState * 2, 0);
    
    } // if

} // drawBlock

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

    if (obj->sprite.worldRow <= 8 * mapYOffset + obj->yTarget * 16) {

        obj->yPos = obj->yTarget;
        obj->sprite.worldRow = 8 * mapYOffset + obj->yTarget * 16;
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

    if (obj->sprite.worldRow >= 8 * mapYOffset + obj->yTarget * 16) {

        obj->yPos = obj->yTarget;
        obj->sprite.worldRow = 8 * mapYOffset + obj->yPos * 16;
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

        if ((obj->sprite.worldRow - vOff > SCREENHEIGHT / 2 - obj->sprite.height / 2) && (vOff < (16 * mapHeight + 8 * mapYOffset) - SCREENHEIGHT)) {
            vOff++;
        
        } // if

    } // if

} // moveDown

void moveLeft(OBJECT* obj) {

    obj->idle = 0;

    if (obj->sprite.worldCol <= 8 * mapXOffset + obj->xTarget * 16) {

        obj->xPos = obj->xTarget;
        obj->sprite.worldCol = 8 * mapXOffset + obj->xTarget * 16;
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
            hOff--;
    
        } // if

    } // if

} // moveLeft

void moveRight(OBJECT* obj) {

    obj->idle = 0;

    if (obj->sprite.worldCol >= 8 * mapXOffset + obj->xTarget * 16) {

        obj->xPos = obj->xTarget;
        obj->sprite.worldCol = 8 * mapXOffset + obj->xPos * 16;
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

        if ((obj->sprite.worldCol - hOff > SCREENWIDTH / 2 - obj->sprite.width / 2) && (hOff < (16 * mapWidth + 8 * mapYOffset) - SCREENWIDTH)) {
            hOff++;
    
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