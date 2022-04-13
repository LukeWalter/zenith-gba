#include "player.h"

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