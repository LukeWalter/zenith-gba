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
OBJECT plates[PLATECOUNT];

LEVEL levels[LEVELCOUNT];

int gameOver;
int gameWon;

int mapWidth;
int mapHeight;
int mapXOffset;
int mapYOffset;

int hOff;
int vOff;

void buildLevels();

void initLevel(int);

void initZenith(int, COORDINATE);
void updateZenith();
void moveZenith();
void drawZenith();

void initBlocks(COORDINATE*);
void updateBlocks();
void moveBlocks();
void drawBlocks();

void initPlates(COORDINATE*);
void updatePlates();
void movePlates();
void drawPlates();

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

    gameOver = 0;
    gameWon = 0;

    buildLevels();
    initLevel(1);

} // initGame

void updateGame() {
    updateZenith();
    updatePlates();

} // updateGame

void drawGame() {

    drawZenith();
    drawBlocks();
    drawPlates();

    waitForVBlank();
    updateOAM();

    REG_BG1VOFF = vOff;
    REG_BG1HOFF = hOff;

} // drawGame

void buildLevels() {

    // Level 1

    levels[0].mapWidth = 11;
    levels[0].mapHeight = 7;
    levels[0].mapXOffset = 4;
    levels[0].mapYOffset = 3;

    levels[0].hOff = 0;
    levels[0].vOff = 0;

    levels[0].zenithOrientation = FRONTWALK;

    COORDINATE zLoc = {0, 0};
    levels[0].zenithLoc = zLoc;

    COORDINATE bLoc = {3, 3};
    levels[0].blockLocs[0] = bLoc;

    COORDINATE pLoc = {4, 4};
    levels[0].plateLocs[0] = pLoc;

    levels[0].palLen = cavePalLen;
    levels[0].tileLen = caveTilesLen;
    levels[0].mapLen = caveMapLen;

    DMANow(3, cavePal, (volatile void*) levels[0].pal, 256);
    DMANow(3, caveTiles, (volatile void*) levels[0].tiles, houseTilesLen / 2);
    DMANow(3, caveMap, (volatile void*) levels[0].map, houseMapLen / 2);


    // Level 2

    levels[1].mapWidth = 16;
    levels[1].mapHeight = 16;
    levels[1].mapXOffset = 0;
    levels[1].mapYOffset = 0;

    levels[1].hOff = 0;
    levels[1].vOff = 0;

    levels[1].zenithOrientation = RIGHTWALK;

    COORDINATE zLoc2 = {1, 1};
    levels[1].zenithLoc = zLoc2;

    COORDINATE bLoc2 = {4, 4};
    levels[1].blockLocs[0] = bLoc2;

    COORDINATE pLoc2 = {5, 5};
    levels[1].plateLocs[0] = pLoc2;

    levels[1].palLen = housePalLen;
    levels[1].tileLen = houseTilesLen;
    levels[1].mapLen = houseMapLen;

    DMANow(3, housePal, (volatile void*) levels[1].pal, 256);
    DMANow(3, houseTiles, (volatile void*) levels[1].tiles, houseTilesLen / 2);
    DMANow(3, houseMap, (volatile void*) levels[1].map, houseMapLen / 2);

} // buildLevels

void initLevel(int level) {

    // Measured in double tiles
    mapWidth = levels[level - 1].mapWidth;
    mapHeight = levels[level - 1].mapHeight;

    // Measured in single tiles
    mapXOffset = levels[level - 1].mapXOffset;
    mapYOffset = levels[level - 1].mapYOffset;

    hOff = levels[level - 1].hOff;
    vOff = levels[level - 1].vOff;

    DMANow(3, levels[level - 1].pal, PALETTE, 256);
    DMANow(3, levels[level - 1].tiles, &CHARBLOCK[0], levels[level - 1].tileLen / 2);
    DMANow(3, levels[level - 1].map, &SCREENBLOCK[28], levels[level - 1].mapLen / 2);
    REG_BG1VOFF = vOff;
    REG_BG1HOFF = hOff;

    DMANow(3, spritesheetPal, SPRITEPALETTE, spritesheetPalLen / 2);
    DMANow(3, spritesheetTiles, &CHARBLOCK[4], spritesheetTilesLen / 2);
    hideSprites();
    updateOAM();

    initZenith(levels[level - 1].zenithOrientation, levels[level - 1].zenithLoc);
    initBlocks(levels[level - 1].blockLocs);
    initPlates(levels[level - 1].plateLocs);

} // initLevel

void initZenith(int zenithOrientation, COORDINATE zenithLoc) {

    zenith.xPos = zenithLoc.col;
    zenith.yPos = zenithLoc.row;
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
    zenith.sprite.aniState = zenithOrientation;
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

void initBlocks(COORDINATE* blockLocs) {

    for (int i = 0; i < BLOCKCOUNT; i++) {

        blocks[i].xPos = blockLocs[i].col;
        blocks[i].yPos = blockLocs[i].row;
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

void initPlates(COORDINATE* plateLocs) {

    for (int i = 0; i < PLATECOUNT; i++) {

        plates[i].xPos = plateLocs[i].col;
        plates[i].yPos = plateLocs[i].row;
        plates[i].xTarget = plates[i].xPos;
        plates[i].yTarget = plates[i].yPos;
        plates[i].baseSpeed = 0;
        plates[i].idle = 1;
        plates[i].active = 1;

        plates[i].sprite.worldRow = 8 * mapYOffset + plates[i].yPos * 16;
        plates[i].sprite.worldCol = 8 * mapXOffset + plates[i].xPos * 16;

        plates[i].sprite.encodeFactor = 8;
        plates[i].sprite.encodeWorldRow = plates[i].sprite.worldRow * plates[i].sprite.encodeFactor;
        plates[i].sprite.encodeWorldCol = plates[i].sprite.worldCol * plates[i].sprite.encodeFactor;

        plates[i].sprite.rdel = plates[i].baseSpeed;
        plates[i].sprite.cdel = plates[i].baseSpeed;

        plates[i].sprite.width = 16;
        plates[i].sprite.height = 16;

        plates[i].sprite.aniCounter = 0;
        plates[i].sprite.aniState = FRONTWALK;
        plates[i].sprite.curFrame = 0;
        plates[i].sprite.numFrames = 2;
        plates[i].sprite.hide = 0;

        plates[i].sprite.attributes = &shadowOAM[2];

    } // for
    
} // initPlates

void updatePlates() {

    for (int i = 0; i < PLATECOUNT; i++) {

        if (plates[i].xPos == zenith.xPos && plates[i].yPos == zenith.yPos) {
            initLevel(1);

        } else {

            for (int j = 0; j < BLOCKCOUNT; j++) {
                
                if (plates[i].xPos == blocks[j].xPos && plates[i].yPos == blocks[j].yPos) {
                    initLevel(2);
                
                } // if

            } // for

        } // if

    } // for

} // updatePlates

void drawPlates() {

    for (int i = 0; i < PLATECOUNT; i++) {

        if (plates[i].sprite.hide) {
            plates[i].sprite.attributes->attr0 |= ATTR0_HIDE;

        } else {
            plates[i].sprite.attributes->attr0 = (plates[i].sprite.worldRow - vOff) | ATTR0_SQUARE;
            plates[i].sprite.attributes->attr1 = (plates[i].sprite.worldCol - hOff) | ATTR1_SMALL;
            plates[i].sprite.attributes->attr2 = ATTR2_PALROW(0) | ATTR2_TILEID(plates[i].sprite.aniState * 2, plates[i].sprite.curFrame * 2);
        
        } // if

    } // for

} // drawPlates

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