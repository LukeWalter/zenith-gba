#include "plate.h"

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

        if (plates[i].xPos == zenith.obj.xPos && plates[i].yPos == zenith.obj.yPos) {
            initLevel(1);

        } else {

            for (int j = 0; j < BLOCKCOUNT; j++) {
                
                if (plates[i].xPos == blocks[j].obj.xPos && plates[i].yPos == blocks[j].obj.yPos) {
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
