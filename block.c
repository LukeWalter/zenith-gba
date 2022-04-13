#include "block.h"

void initBlocks(COORDINATE* blockLocs) {

    for (int i = 0; i < BLOCKCOUNT; i++) {

        blocks[i].obj.xPos = blockLocs[i].col;
        blocks[i].obj.yPos = blockLocs[i].row;
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
        blocks[i].obj.sprite.aniState = BACKWALK;
        blocks[i].obj.sprite.curFrame = 0;
        blocks[i].obj.sprite.numFrames = 1;
        blocks[i].obj.sprite.hide = 0;

        blocks[i].obj.sprite.attributes = &shadowOAM[1];

    } // for

} // initBlock

void updateBlocks() {
    moveBlocks();

} // updateBlock

void moveBlocks() {
    
    for (int i = 0; i < BLOCKCOUNT; i++) {

        if (blocks[i].obj.yTarget < blocks[i].obj.yPos)      moveUp(&blocks[i].obj);
        else if (blocks[i].obj.yTarget > blocks[i].obj.yPos) moveDown(&blocks[i].obj);
        else if (blocks[i].obj.xTarget < blocks[i].obj.xPos) moveLeft(&blocks[i].obj);
        else if (blocks[i].obj.xTarget > blocks[i].obj.xPos) moveRight(&blocks[i].obj);
        
    } // for

} // moveBlock

void drawBlocks() {

    for (int i = 0; i < BLOCKCOUNT; i++) {

        if (blocks[i].obj.sprite.hide) {
            blocks[i].obj.sprite.attributes->attr0 |= ATTR0_HIDE;

        } else {
            blocks[i].obj.sprite.attributes->attr0 = (blocks[i].obj.sprite.worldRow - vOff) | ATTR0_SQUARE;
            blocks[i].obj.sprite.attributes->attr1 = (blocks[i].obj.sprite.worldCol - hOff) | ATTR1_SMALL;
            blocks[i].obj.sprite.attributes->attr2 = ATTR2_PALROW(0) | ATTR2_TILEID(blocks[i].obj.sprite.aniState * 2, 0);
        
        } // if

    } // for

} // drawBlock