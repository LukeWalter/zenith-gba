#include "player.h"

void initZenith(int zenithOrientation, COORDINATE zenithLoc) {

    zenith.obj.xPos = zenithLoc.col;
    zenith.obj.yPos = zenithLoc.row;
    zenith.obj.xTarget = zenith.obj.xPos;
    zenith.obj.yTarget = zenith.obj.yPos;
    zenith.obj.baseSpeed = 8;
    zenith.obj.idle = 1;
    zenith.obj.active = 1;

    zenith.obj.sprite.worldRow = 8 * mapYOffset + zenith.obj.yPos * 16;
    zenith.obj.sprite.worldCol = 8 * mapXOffset + zenith.obj.xPos * 16;

    zenith.obj.sprite.encodeFactor = 8;
    zenith.obj.sprite.encodeWorldRow = zenith.obj.sprite.worldRow * zenith.obj.sprite.encodeFactor;
    zenith.obj.sprite.encodeWorldCol = zenith.obj.sprite.worldCol * zenith.obj.sprite.encodeFactor;

    zenith.obj.sprite.rdel = zenith.obj.baseSpeed;
    zenith.obj.sprite.cdel = zenith.obj.baseSpeed;

    zenith.obj.sprite.width = 16;
    zenith.obj.sprite.height = 16;

    zenith.obj.sprite.aniCounter = 0;
    zenith.obj.sprite.aniState = zenithOrientation;
    zenith.obj.sprite.curFrame = 0;
    zenith.obj.sprite.numFrames = 3;
    zenith.obj.sprite.hide = 0;

    zenith.obj.sprite.attributes = &shadowOAM[0];

} // initZenith

void updateZenith() {
    moveZenith();

} // updateZenith

void moveZenith() {

    zenith.obj.sprite.prevAniState = zenith.obj.sprite.aniState;
    if (zenith.obj.idle) zenith.obj.sprite.aniState = IDLE;

    if (zenith.obj.sprite.aniCounter % 15 == 0) {
        zenith.obj.sprite.curFrame = (zenith.obj.sprite.curFrame + 1) % zenith.obj.sprite.numFrames;
    
    } // if
    
    if (zenith.obj.idle) {
        
        if (BUTTON_HELD(BUTTON_B)) {

            for (int i = 0; i < BLOCKCOUNT; i++) {

                if (BUTTON_HELD(BUTTON_UP) && zenith.obj.yTarget - 1 == blocks[i].yPos && zenith.obj.xPos == blocks[i].xPos && canMoveUp(&blocks[i])) {
                    blocks[i].yTarget--;
                    zenith.obj.sprite.aniState = BACKWALK;
                    zenith.obj.yTarget--;
                    zenith.obj.sprite.rdel = blocks[i].baseSpeed;
                    
                } else if (BUTTON_HELD(BUTTON_DOWN) && zenith.obj.yTarget + 1 == blocks[i].yPos && zenith.obj.xPos == blocks[i].xPos && canMoveDown(&blocks[i])) {
                    blocks[i].yTarget++;
                    zenith.obj.sprite.aniState = FRONTWALK;
                    zenith.obj.yTarget++;
                    zenith.obj.sprite.rdel = blocks[i].baseSpeed;
                    
                } else if (BUTTON_HELD(BUTTON_LEFT) && zenith.obj.xTarget - 1 == blocks[i].xPos && zenith.obj.yPos == blocks[i].yPos && canMoveLeft(&blocks[i])) {
                    blocks[i].xTarget--;
                    zenith.obj.sprite.aniState = LEFTWALK;
                    zenith.obj.xTarget--;
                    zenith.obj.sprite.cdel = blocks[i].baseSpeed;

                } else if (BUTTON_HELD(BUTTON_RIGHT) && zenith.obj.xTarget + 1 == blocks[i].xPos && zenith.obj.yPos == blocks[i].yPos && canMoveRight(&blocks[i])) {
                    blocks[i].xTarget++;
                    zenith.obj.sprite.aniState = RIGHTWALK;
                    zenith.obj.xTarget++;
                    zenith.obj.sprite.cdel = blocks[i].baseSpeed;

                } // if

            } // for

        } else {

            if (BUTTON_HELD(BUTTON_UP) && canMoveUp(&zenith.obj)) {
                zenith.obj.sprite.aniState = BACKWALK;
                zenith.obj.yTarget--;
                
            } else if (BUTTON_HELD(BUTTON_DOWN) && canMoveDown(&zenith.obj)) {
                zenith.obj.sprite.aniState = FRONTWALK;
                zenith.obj.yTarget++;
                
            } else if (BUTTON_HELD(BUTTON_LEFT) && canMoveLeft(&zenith.obj)) {
                zenith.obj.sprite.aniState = LEFTWALK;
                zenith.obj.xTarget--;

            } else if (BUTTON_HELD(BUTTON_RIGHT) && canMoveRight(&zenith.obj)) {
                zenith.obj.sprite.aniState = RIGHTWALK;
                zenith.obj.xTarget++;

            } else if (BUTTON_HELD(BUTTON_UP)) {
                zenith.obj.sprite.aniState = BACKWALK;
                
            } else if (BUTTON_HELD(BUTTON_DOWN)) {
                zenith.obj.sprite.aniState = FRONTWALK;
                
            } else if (BUTTON_HELD(BUTTON_LEFT)) {
                zenith.obj.sprite.aniState = LEFTWALK;

            } else if (BUTTON_HELD(BUTTON_RIGHT)) {
                zenith.obj.sprite.aniState = RIGHTWALK;

            } // if

        } // if

    } // if

    updateBlocks();

    if (zenith.obj.yTarget < zenith.obj.yPos)      moveUp(&zenith.obj);
    else if (zenith.obj.yTarget > zenith.obj.yPos) moveDown(&zenith.obj);
    else if (zenith.obj.xTarget < zenith.obj.xPos) moveLeft(&zenith.obj);
    else if (zenith.obj.xTarget > zenith.obj.xPos) moveRight(&zenith.obj);


    if (zenith.obj.sprite.aniState == IDLE) {
        zenith.obj.sprite.curFrame = 0;
        zenith.obj.sprite.aniCounter = 0;
        zenith.obj.sprite.aniState = zenith.obj.sprite.prevAniState;

    } else {
        zenith.obj.sprite.aniCounter++;

    } // if

} // animateZenith

void drawZenith() {
    
    if (zenith.obj.sprite.hide) {
        zenith.obj.sprite.attributes->attr0 |= ATTR0_HIDE;

    } else {
        zenith.obj.sprite.attributes->attr0 = (zenith.obj.sprite.worldRow - vOff) | ATTR0_SQUARE;
        zenith.obj.sprite.attributes->attr1 = (zenith.obj.sprite.worldCol - hOff) | ATTR1_SMALL;
        zenith.obj.sprite.attributes->attr2 = ATTR2_PALROW(0) | ATTR2_TILEID(zenith.obj.sprite.aniState * 2, zenith.obj.sprite.curFrame * 2);
    
    } // if

} // drawZenith