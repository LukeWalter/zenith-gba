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
    int baseSpeed;
    int idle;
    int active;
    ANISPRITE sprite;

} OBJECT;

OBJECT zenith;
OBJECT blocks[BLOCKCOUNT];

int gameOver;
int gameWon;

int mapWidth;
int mapHeight;
int mapXOffset;
int mapYOffset;

int hOff;
int vOff;

void initLevel(int);

void initZenith();
void updateZenith();
void moveZenith();
void drawZenith();

void initBlocks();
void updateBlocks();
void moveBlocks();
void drawBlocks();

int canMoveUp(OBJECT*);
int canMoveDown(OBJECT*);
int canMoveLeft(OBJECT*);
int canMoveRight(OBJECT*);

void moveUp(OBJECT*);
void moveDown(OBJECT*);
void moveLeft(OBJECT*);
void moveRight(OBJECT*);

void readInput(OBJECT*);

void initGame() {

    initLevel(1);

} // initGame

void updateGame() {
    updateZenith();

} // updateGame

void drawGame() {

    drawZenith();
    drawBlocks();

    waitForVBlank();
    updateOAM();

    REG_BG1VOFF = vOff;
    REG_BG1HOFF = hOff;

} // drawGame

void initLevel(int level) {

    switch (level) {
        
        case 1: {

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
            initBlocks();

            break;

        } // 1

        case 2: {

            gameOver = 0;
            gameWon = 0;

            // Measured in double tiles
            mapWidth = 16;
            mapHeight = 16;

            // Measured in single tiles
            mapXOffset = 0;
            mapYOffset = 0;

            hOff = 0;
            vOff = 0;

            DMANow(3, housePal, PALETTE, 256);
            DMANow(3, houseTiles, &CHARBLOCK[0], houseTilesLen / 2);
            DMANow(3, houseMap, &SCREENBLOCK[28], houseMapLen / 2);
            REG_BG1VOFF = vOff;
            REG_BG1HOFF = hOff;

            DMANow(3, spritesheetPal, SPRITEPALETTE, spritesheetPalLen / 2);
            DMANow(3, spritesheetTiles, &CHARBLOCK[4], spritesheetTilesLen / 2);
            hideSprites();
            updateOAM();

            initZenith();
            initBlocks();

            break;

        } // 2

        default: { break; }

    } // switch

} // initLevel

void initZenith() {

    zenith.xPos = 0;
    zenith.yPos = 0;
    zenith.xTarget = zenith.xPos;
    zenith.yTarget = zenith.yPos;
    zenith.baseSpeed = 8;
    zenith.idle = 1;
    zenith.active = 1;

    zenith.sprite.worldRow = 8 * mapYOffset + zenith.yPos * 16;
    zenith.sprite.worldCol = 8 * mapXOffset + zenith.xPos * 16;

    zenith.sprite.encodeFactor = 8;
    zenith.sprite.encodeWorldRow = zenith.sprite.worldRow * zenith.sprite.encodeFactor;
    zenith.sprite.encodeWorldCol = zenith.sprite.worldCol * zenith.sprite.encodeFactor;

    zenith.sprite.rdel = zenith.baseSpeed;
    zenith.sprite.cdel = zenith.baseSpeed;

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
    
    if (zenith.idle) {
        
        if (BUTTON_HELD(BUTTON_B)) {

            for (int i = 0; i < BLOCKCOUNT; i++) {

                if (BUTTON_HELD(BUTTON_UP) && zenith.yTarget - 1 == blocks[i].yPos && zenith.xPos == blocks[i].xPos && canMoveUp(&blocks[i])) {
                    blocks[i].yTarget--;
                    zenith.sprite.aniState = BACKWALK;
                    zenith.yTarget--;
                    zenith.sprite.rdel = blocks[i].baseSpeed;
                    
                } else if (BUTTON_HELD(BUTTON_DOWN) && zenith.yTarget + 1 == blocks[i].yPos && zenith.xPos == blocks[i].xPos && canMoveDown(&blocks[i])) {
                    blocks[i].yTarget++;
                    zenith.sprite.aniState = FRONTWALK;
                    zenith.yTarget++;
                    zenith.sprite.rdel = blocks[i].baseSpeed;
                    
                } else if (BUTTON_HELD(BUTTON_LEFT) && zenith.xTarget - 1 == blocks[i].xPos && zenith.yPos == blocks[i].yPos && canMoveLeft(&blocks[i])) {
                    blocks[i].xTarget--;
                    zenith.sprite.aniState = LEFTWALK;
                    zenith.xTarget--;
                    zenith.sprite.cdel = blocks[i].baseSpeed;

                } else if (BUTTON_HELD(BUTTON_RIGHT) && zenith.xTarget + 1 == blocks[i].xPos && zenith.yPos == blocks[i].yPos && canMoveRight(&blocks[i])) {
                    blocks[i].xTarget++;
                    zenith.sprite.aniState = RIGHTWALK;
                    zenith.xTarget++;
                    zenith.sprite.cdel = blocks[i].baseSpeed;

                } // if

            } // for

        } else {

            if (BUTTON_HELD(BUTTON_UP) && canMoveUp(&zenith)) {
                zenith.sprite.aniState = BACKWALK;
                zenith.yTarget--;
                
            } else if (BUTTON_HELD(BUTTON_DOWN) && canMoveDown(&zenith)) {
                zenith.sprite.aniState = FRONTWALK;
                zenith.yTarget++;
                
            } else if (BUTTON_HELD(BUTTON_LEFT) && canMoveLeft(&zenith)) {
                zenith.sprite.aniState = LEFTWALK;
                zenith.xTarget--;

            } else if (BUTTON_HELD(BUTTON_RIGHT) && canMoveRight(&zenith)) {
                zenith.sprite.aniState = RIGHTWALK;
                zenith.xTarget++;

            } else if (BUTTON_HELD(BUTTON_UP)) {
                zenith.sprite.aniState = BACKWALK;
                
            } else if (BUTTON_HELD(BUTTON_DOWN)) {
                zenith.sprite.aniState = FRONTWALK;
                
            } else if (BUTTON_HELD(BUTTON_LEFT)) {
                zenith.sprite.aniState = LEFTWALK;

            } else if (BUTTON_HELD(BUTTON_RIGHT)) {
                zenith.sprite.aniState = RIGHTWALK;

            } // if

        } // if

    } // if

    updateBlocks();

    if (zenith.yTarget < zenith.yPos)      moveUp(&zenith);
    else if (zenith.yTarget > zenith.yPos) moveDown(&zenith);
    else if (zenith.xTarget < zenith.xPos) moveLeft(&zenith);
    else if (zenith.xTarget > zenith.xPos) moveRight(&zenith);


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

void initBlocks() {

    for (int i = 0; i < BLOCKCOUNT; i++) {

        blocks[i].xPos = 3;
        blocks[i].yPos = 3;
        blocks[i].xTarget = blocks[i].xPos;
        blocks[i].yTarget = blocks[i].yPos;
        blocks[i].baseSpeed = 4;
        blocks[i].idle = 1;
        blocks[i].active = 1;

        blocks[i].sprite.worldRow = 8 * mapYOffset + blocks[i].yPos * 16;
        blocks[i].sprite.worldCol = 8 * mapXOffset + blocks[i].xPos * 16;

        blocks[i].sprite.encodeFactor = 8;
        blocks[i].sprite.encodeWorldRow = blocks[i].sprite.worldRow * blocks[i].sprite.encodeFactor;
        blocks[i].sprite.encodeWorldCol = blocks[i].sprite.worldCol * blocks[i].sprite.encodeFactor;

        blocks[i].sprite.rdel = blocks[i].baseSpeed;
        blocks[i].sprite.cdel = blocks[i].baseSpeed;

        blocks[i].sprite.width = 16;
        blocks[i].sprite.height = 16;

        blocks[i].sprite.aniCounter = 0;
        blocks[i].sprite.aniState = BACKWALK;
        blocks[i].sprite.curFrame = 0;
        blocks[i].sprite.numFrames = 1;
        blocks[i].sprite.hide = 0;

        blocks[i].sprite.attributes = &shadowOAM[1];

    } // for

} // initBlock

void updateBlocks() {
    moveBlocks();

} // updateBlock

void moveBlocks() {
    
    for (int i = 0; i < BLOCKCOUNT; i++) {

        if (blocks[i].yTarget < blocks[i].yPos)      moveUp(&blocks[i]);
        else if (blocks[i].yTarget > blocks[i].yPos) moveDown(&blocks[i]);
        else if (blocks[i].xTarget < blocks[i].xPos) moveLeft(&blocks[i]);
        else if (blocks[i].xTarget > blocks[i].xPos) moveRight(&blocks[i]);
        
    } // for

} // moveBlock

void drawBlocks() {

    for (int i = 0; i < BLOCKCOUNT; i++) {

        if (blocks[i].sprite.hide) {
            blocks[i].sprite.attributes->attr0 |= ATTR0_HIDE;

        } else {
            blocks[i].sprite.attributes->attr0 = (blocks[i].sprite.worldRow - vOff) | ATTR0_SQUARE;
            blocks[i].sprite.attributes->attr1 = (blocks[i].sprite.worldCol - hOff) | ATTR1_SMALL;
            blocks[i].sprite.attributes->attr2 = ATTR2_PALROW(0) | ATTR2_TILEID(blocks[i].sprite.aniState * 2, 0);
        
        } // if

    } // for

} // drawBlock

int canMoveUp(OBJECT* obj) {

    if (obj->yTarget - 1 > -1) {
        
        for (int i = 0; i < BLOCKCOUNT; i++) {
            
            if (obj->yTarget - 1 == blocks[i].yPos && obj->xPos == blocks[i].xPos) {
                return 0;
        
            } // if

        } // for

        return 1;

    } // if

    return 0;

} // canMoveUp

int canMoveDown(OBJECT* obj) {

    if (obj->yTarget + 1 < mapHeight) {
        
        for (int i = 0; i < BLOCKCOUNT; i++) {
            
            if (obj->yTarget + 1 == blocks[i].yPos && obj->xPos == blocks[i].xPos) {
                return 0;
        
            } // if

        } // for

        return 1;

    } // if

    return 0;

} // canMoveDown

int canMoveLeft(OBJECT* obj) {

    if (obj->xTarget - 1 > -1) {
        
        for (int i = 0; i < BLOCKCOUNT; i++) {
            
            if (obj->xTarget - 1 == blocks[i].xPos && obj->yPos == blocks[i].yPos) {
                return 0;
        
            } // if

        } // for

        return 1;

    } // if

    return 0;

} // canMoveLeft

int canMoveRight(OBJECT* obj) {

    if (obj->xTarget + 1 < mapWidth) {
        
        for (int i = 0; i < BLOCKCOUNT; i++) {
            
            if (obj->xTarget + 1 == blocks[i].xPos && obj->yPos == blocks[i].yPos) {
                return 0;
        
            } // if

        } // for

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
        

        obj->sprite.rdel = obj->baseSpeed;
        obj->idle = 1;

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

        obj->sprite.rdel = obj->baseSpeed;
        obj->idle = 1;

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

        obj->sprite.cdel = obj->baseSpeed;
        obj->idle = 1;

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

        obj->sprite.cdel = obj->baseSpeed;
        obj->idle = 1;

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