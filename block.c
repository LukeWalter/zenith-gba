#include "block.h"

#include "game.h"
#include "player.h"
#include "plate.h"

enum {HORIZONTAL, VERTICAL};

int numBlocks;

void initBlocks(LEVEL level) {

    numBlocks = level.numBlocks;

    for (int i = 0; i < numBlocks; i++) {

        blocks[i].obj.xPos = level.blockLocs[i].col;
        blocks[i].obj.yPos = level.blockLocs[i].row;
        blocks[i].obj.xTarget = blocks[i].obj.xPos;
        blocks[i].obj.yTarget = blocks[i].obj.yPos;
        blocks[i].obj.baseSpeed = 4;
        blocks[i].obj.idle = 1;
        blocks[i].obj.active = 1;

        blocks[i].obj.sprite.worldRow = 8 * mapYOffset + blocks[i].obj.yPos * 16;
        blocks[i].obj.sprite.worldCol = 8 * mapXOffset + blocks[i].obj.xPos * 16;

        blocks[i].obj.sprite.encodeFactor = 8;
        blocks[i].obj.sprite.encodeWorldRow = blocks[i].obj.sprite.worldRow * blocks[i].obj.sprite.encodeFactor;
        blocks[i].obj.sprite.encodeWorldCol = blocks[i].obj.sprite.worldCol * blocks[i].obj.sprite.encodeFactor;

        blocks[i].obj.sprite.rdel = blocks[i].obj.baseSpeed;
        blocks[i].obj.sprite.cdel = blocks[i].obj.baseSpeed;

        blocks[i].obj.sprite.width = 16;
        blocks[i].obj.sprite.height = 16;

        blocks[i].obj.sprite.aniCounter = 0;
        blocks[i].obj.sprite.aniState = HORIZONTAL;
        blocks[i].obj.sprite.curFrame = 0;
        blocks[i].obj.sprite.numFrames = 4;
        blocks[i].obj.sprite.hide = 0;

        blocks[i].obj.sprite.attributes = &shadowOAM[8 + i];

    } // for

    for (int i = numBlocks; i < BLOCKCOUNT; i++) {
        blocks[i].obj.sprite.hide = 1;
        blocks[i].obj.active = 0;

    } // for

} // initBlock

void updateBlocks() {
    moveBlocks();

} // updateBlock

void moveBlocks() {
    
    for (int i = 0; i < numBlocks; i++) {
        
        blocks[i].obj.sprite.aniCounter++;

        if (blocks[i].obj.yTarget < blocks[i].obj.yPos) {
            moveUp(&blocks[i].obj);
            blocks[i].obj.sprite.aniState = VERTICAL;
            if (blocks[i].obj.sprite.aniCounter % 10 == 0) 
                blocks[i].obj.sprite.curFrame = (blocks[i].obj.sprite.curFrame - 1 + blocks[i].obj.sprite.numFrames) % blocks[i].obj.sprite.numFrames;

        } else if (blocks[i].obj.yTarget > blocks[i].obj.yPos) {
            moveDown(&blocks[i].obj);
            blocks[i].obj.sprite.aniState = VERTICAL;
            if (blocks[i].obj.sprite.aniCounter % 10 == 0) 
                blocks[i].obj.sprite.curFrame = (blocks[i].obj.sprite.curFrame + 1 + blocks[i].obj.sprite.numFrames) % blocks[i].obj.sprite.numFrames;

        } else if (blocks[i].obj.xTarget < blocks[i].obj.xPos) {
            moveLeft(&blocks[i].obj);
            blocks[i].obj.sprite.aniState = HORIZONTAL;
            if (blocks[i].obj.sprite.aniCounter % 10 == 0) 
                blocks[i].obj.sprite.curFrame = (blocks[i].obj.sprite.curFrame - 1 + blocks[i].obj.sprite.numFrames) % blocks[i].obj.sprite.numFrames;

        } else if (blocks[i].obj.xTarget > blocks[i].obj.xPos) {
            moveRight(&blocks[i].obj);
            blocks[i].obj.sprite.aniState = HORIZONTAL;
            if (blocks[i].obj.sprite.aniCounter % 10 == 0)
                blocks[i].obj.sprite.curFrame = (blocks[i].obj.sprite.curFrame + 1 + blocks[i].obj.sprite.numFrames) % blocks[i].obj.sprite.numFrames;

        } else {
            blocks[i].obj.sprite.curFrame = 0;
             
        } // if

    } // for

} // moveBlock

void drawBlocks() {

    for (int i = 0; i < numBlocks; i++) {

        if (blocks[i].obj.sprite.hide) {
            blocks[i].obj.sprite.attributes->attr0 |= ATTR0_HIDE;

        } else {
            blocks[i].obj.sprite.attributes->attr0 = ((blocks[i].obj.sprite.worldRow - vOff) & 0x00FF) | ATTR0_SQUARE;
            blocks[i].obj.sprite.attributes->attr1 = ((blocks[i].obj.sprite.worldCol - hOff) & 0x01FF) | ATTR1_SMALL;
            blocks[i].obj.sprite.attributes->attr2 = ATTR2_PALROW(0) | ATTR2_TILEID(10 + blocks[i].obj.sprite.aniState * 2, blocks[i].obj.sprite.curFrame * 2);
        
        } // if

    } // for

} // drawBlock