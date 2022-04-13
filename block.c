#include "block.h"

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