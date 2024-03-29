#include "player.h"

#include "game.h"

#include "block.h"
#include "plate.h"

#include "sound.h"
#include "step.h"
#include "door.h"

void initZenith(LEVEL level) {

    zenith.obj.xPos = level.zenithLoc.col;
    zenith.obj.yPos = level.zenithLoc.row;
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
    zenith.obj.sprite.aniState = level.zenithOrientation;
    zenith.obj.sprite.curFrame = 0;
    zenith.obj.sprite.numFrames = 4;
    zenith.obj.sprite.hide = 0;

    zenith.obj.sprite.attributes = &shadowOAM[7];

    zenith.equippedTool = NULL;

} // initZenith

void updateZenith() {
    moveZenith();

} // updateZenith

void moveZenith() {

    zenith.obj.sprite.prevAniState = zenith.obj.sprite.aniState;
    if (zenith.obj.idle) zenith.obj.sprite.aniState = IDLE;

    if (zenith.obj.sprite.aniCounter % 12 == 0) {
        zenith.obj.sprite.curFrame = (zenith.obj.sprite.curFrame + 1) % zenith.obj.sprite.numFrames;
    
    } // if
    
    if (zenith.obj.idle) {
        
        int pickedUp = 0;

        if (BUTTON_PRESSED(BUTTON_R)) {
            spawnCheaterShovel();

        } else if (BUTTON_HELD(BUTTON_B)) {

            for (int i = 0; i < numBlocks; i++) {

                if (BUTTON_HELD(BUTTON_UP) && zenith.obj.yTarget - 1 == blocks[i].obj.yPos && zenith.obj.xPos == blocks[i].obj.xPos && canMoveUp(&blocks[i].obj)) {
                    blocks[i].obj.yTarget--;
                    zenith.obj.sprite.aniState = BACKWALK;
                    zenith.obj.yTarget--;
                    zenith.obj.sprite.rdel = blocks[i].obj.baseSpeed;
                    playSoundB(door_data, door_length, 0);
                    
                } else if (BUTTON_HELD(BUTTON_DOWN) && zenith.obj.yTarget + 1 == blocks[i].obj.yPos && zenith.obj.xPos == blocks[i].obj.xPos && canMoveDown(&blocks[i].obj)) {
                    blocks[i].obj.yTarget++;
                    zenith.obj.sprite.aniState = FRONTWALK;
                    zenith.obj.yTarget++;
                    zenith.obj.sprite.rdel = blocks[i].obj.baseSpeed;
                    playSoundB(door_data, door_length, 0);
                    
                } else if (BUTTON_HELD(BUTTON_LEFT) && zenith.obj.xTarget - 1 == blocks[i].obj.xPos && zenith.obj.yPos == blocks[i].obj.yPos && canMoveLeft(&blocks[i].obj)) {
                    blocks[i].obj.xTarget--;
                    zenith.obj.sprite.aniState = LEFTWALK;
                    zenith.obj.xTarget--;
                    zenith.obj.sprite.cdel = blocks[i].obj.baseSpeed;
                    playSoundB(door_data, door_length, 0);

                } else if (BUTTON_HELD(BUTTON_RIGHT) && zenith.obj.xTarget + 1 == blocks[i].obj.xPos && zenith.obj.yPos == blocks[i].obj.yPos && canMoveRight(&blocks[i].obj)) {
                    blocks[i].obj.xTarget++;
                    zenith.obj.sprite.aniState = RIGHTWALK;
                    zenith.obj.xTarget++;
                    zenith.obj.sprite.cdel = blocks[i].obj.baseSpeed;
                    playSoundB(door_data, door_length, 0);

                } // if

            } // for

        } else if (BUTTON_PRESSED(BUTTON_A)) {
            
            if (cheaterShovel.obj.active && cheaterShovel.obj.xPos == zenith.obj.xPos && cheaterShovel.obj.yPos == zenith.obj.yPos) {

                cheaterShovel.obj.active = 0;
                cheaterShovel.obj.sprite.hide = 1;
                    
                if (zenith.equippedTool != NULL) {
                        
                    zenith.equippedTool->obj.xPos = zenith.obj.xPos;
                    zenith.equippedTool->obj.yPos = zenith.obj.yPos;
                    zenith.equippedTool->obj.sprite.worldRow = 8 * mapYOffset + cheaterShovel.obj.yPos * 16;
                    zenith.equippedTool->obj.sprite.worldCol = 8 * mapXOffset + cheaterShovel.obj.xPos * 16;

                    zenith.equippedTool->obj.active = 1;
                    zenith.equippedTool->obj.sprite.hide = 0;

                } // if

                zenith.equippedTool = &cheaterShovel;
                pickedUp = 1;

            } else {

                for (int i = 0; i < numTools; i++) {

                    if (tools[i].obj.active && tools[i].obj.xPos == zenith.obj.xPos && tools[i].obj.yPos == zenith.obj.yPos) {

                        tools[i].obj.active = 0;
                        tools[i].obj.sprite.hide = 1;
                        
                        if (zenith.equippedTool != NULL) {
                            
                            zenith.equippedTool->obj.xPos = zenith.obj.xPos;
                            zenith.equippedTool->obj.yPos = zenith.obj.yPos;
                            zenith.equippedTool->obj.sprite.worldRow = 8 * mapYOffset + tools[i].obj.yPos * 16;
                            zenith.equippedTool->obj.sprite.worldCol = 8 * mapXOffset + tools[i].obj.xPos * 16;

                            zenith.equippedTool->obj.active = 1;
                            zenith.equippedTool->obj.sprite.hide = 0;

                        } // if

                        zenith.equippedTool = &tools[i];
                        pickedUp = 1;
                        break;

                    } // if

                } // for

            } // if

        } else {

            if (BUTTON_HELD(BUTTON_UP) && canMoveUp(&zenith.obj)) {
                zenith.obj.sprite.aniState = BACKWALK;
                zenith.obj.yTarget--;
                playSoundB(step_data, step_length, 0);
                
            } else if (BUTTON_HELD(BUTTON_DOWN) && canMoveDown(&zenith.obj)) {
                zenith.obj.sprite.aniState = FRONTWALK;
                zenith.obj.yTarget++;
                playSoundB(step_data, step_length, 0);
                
            } else if (BUTTON_HELD(BUTTON_LEFT) && canMoveLeft(&zenith.obj)) {
                zenith.obj.sprite.aniState = LEFTWALK;
                zenith.obj.xTarget--;
                playSoundB(step_data, step_length, 0);

            } else if (BUTTON_HELD(BUTTON_RIGHT) && canMoveRight(&zenith.obj)) {
                zenith.obj.sprite.aniState = RIGHTWALK;
                zenith.obj.xTarget++;
                playSoundB(step_data, step_length, 0);

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

        if (!pickedUp && zenith.equippedTool != NULL) {
            zenith.equippedTool->ability();

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
        zenith.obj.sprite.attributes->attr0 = ((zenith.obj.sprite.worldRow - 5 - vOff) & 0x00FF) | ATTR0_TALL;
        zenith.obj.sprite.attributes->attr1 = ((zenith.obj.sprite.worldCol - hOff) & 0x01FF) | ATTR1_MEDIUM;
        zenith.obj.sprite.attributes->attr2 = ATTR2_PALROW(0) | ATTR2_TILEID(zenith.obj.sprite.aniState * 2, zenith.obj.sprite.curFrame * 4);
    
    } // if

} // drawZenith