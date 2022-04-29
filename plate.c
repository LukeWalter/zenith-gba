#include "plate.h"

#include "game.h"
#include "player.h"
#include "block.h"

void initPlates(LEVEL level) {

    for (int i = 0; i < level.numPlates; i++) {

        plates[i].obj.xPos = level.plateLocs[i].col;
        plates[i].obj.yPos = level.plateLocs[i].row;
        plates[i].obj.xTarget = plates[i].obj.xPos;
        plates[i].obj.yTarget = plates[i].obj.yPos;
        plates[i].obj.baseSpeed = 0;
        plates[i].obj.idle = 1;
        plates[i].obj.active = 1;

        plates[i].obj.sprite.worldRow = 8 * mapYOffset + plates[i].obj.yPos * 16;
        plates[i].obj.sprite.worldCol = 8 * mapXOffset + plates[i].obj.xPos * 16;

        plates[i].obj.sprite.encodeFactor = 8;
        plates[i].obj.sprite.encodeWorldRow = plates[i].obj.sprite.worldRow * plates[i].obj.sprite.encodeFactor;
        plates[i].obj.sprite.encodeWorldCol = plates[i].obj.sprite.worldCol * plates[i].obj.sprite.encodeFactor;

        plates[i].obj.sprite.rdel = plates[i].obj.baseSpeed;
        plates[i].obj.sprite.cdel = plates[i].obj.baseSpeed;

        plates[i].obj.sprite.width = 16;
        plates[i].obj.sprite.height = 16;

        plates[i].obj.sprite.aniCounter = 0;
        plates[i].obj.sprite.aniState = FRONTWALK;
        plates[i].obj.sprite.curFrame = 0;
        plates[i].obj.sprite.numFrames = 2;
        plates[i].obj.sprite.hide = 0;

        plates[i].obj.sprite.attributes = &shadowOAM[8 + BLOCKCOUNT + PLATECOUNT + i];

        plates[i].pressed = level.plateInitStates[i];
        plates[i].onFunc = level.onFuncs[i];
        plates[i].offFunc = level.offFuncs[i];

    } // for
    
} // initPlates

void updatePlates() {

    int switchState = 0;

    for (int i = 0; i < PLATECOUNT; i++) {

        switchState = 1;

        if (plates[i].pressed) {

            if (plates[i].obj.xPos == zenith.obj.xPos && plates[i].obj.yPos == zenith.obj.yPos) {
                switchState = 0;

            } else {

                for (int j = 0; j < BLOCKCOUNT; j++) {
                    
                    if (plates[i].obj.xPos == blocks[j].obj.xPos && plates[i].obj.yPos == blocks[j].obj.yPos) {
                        switchState = 0;
                    
                    } // if

                } // for

            } // if

            if (switchState) {
                plates[i].offFunc();
                plates[i].pressed = 0;
                plates[i].obj.sprite.curFrame = 0;

            } // if

        } else {

            switchState = 0;

            if (plates[i].obj.xPos == zenith.obj.xPos && plates[i].obj.yPos == zenith.obj.yPos) {
                switchState = 1;

            } else {

                for (int j = 0; j < BLOCKCOUNT; j++) {
                    
                    if (plates[i].obj.xPos == blocks[j].obj.xPos && plates[i].obj.yPos == blocks[j].obj.yPos) {
                        switchState = 1;
                    
                    } // if

                } // for

            } // if

            if (switchState) {
                plates[i].onFunc();
                plates[i].pressed = 1;
                plates[i].obj.sprite.curFrame = 1;

            } // if

        } // if

    } // for

} // updatePlates

void drawPlates() {

    for (int i = 0; i < PLATECOUNT; i++) {

        if (plates[i].obj.sprite.hide) {
            plates[i].obj.sprite.attributes->attr0 |= ATTR0_HIDE;

        } else {
            plates[i].obj.sprite.attributes->attr0 = (plates[i].obj.sprite.worldRow - vOff) | ATTR0_SQUARE;
            plates[i].obj.sprite.attributes->attr1 = (plates[i].obj.sprite.worldCol - hOff) | ATTR1_SMALL;
            plates[i].obj.sprite.attributes->attr2 = ATTR2_PALROW(0) | ATTR2_TILEID(8, plates[i].obj.sprite.curFrame * 2);
        
        } // if

    } // for

} // drawPlates
