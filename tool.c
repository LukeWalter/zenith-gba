#include "tool.h"

#include "game.h"
#include "plate.h"
#include "block.h"

int numTools;

void initTools(LEVEL level) {

    numTools = level.numTools;

    for (int i = 0; i < numTools; i++) {

        tools[i].obj.xPos = level.toolLocs[i].col;
        tools[i].obj.yPos = level.toolLocs[i].row;
        tools[i].obj.xTarget = tools[i].obj.xPos;
        tools[i].obj.yTarget = tools[i].obj.yPos;
        tools[i].obj.baseSpeed = 0;
        tools[i].obj.idle = 1;
        tools[i].obj.active = 1;

        tools[i].obj.sprite.worldRow = 8 * mapYOffset + tools[i].obj.yPos * 16;
        tools[i].obj.sprite.worldCol = 8 * mapXOffset + tools[i].obj.xPos * 16;

        tools[i].obj.sprite.encodeFactor = 8;
        tools[i].obj.sprite.encodeWorldRow = tools[i].obj.sprite.worldRow * tools[i].obj.sprite.encodeFactor;
        tools[i].obj.sprite.encodeWorldCol = tools[i].obj.sprite.worldCol * tools[i].obj.sprite.encodeFactor;

        tools[i].obj.sprite.rdel = tools[i].obj.baseSpeed;
        tools[i].obj.sprite.cdel = tools[i].obj.baseSpeed;

        tools[i].obj.sprite.width = 16;
        tools[i].obj.sprite.height = 16;

        tools[i].obj.sprite.aniCounter = 0;
        tools[i].obj.sprite.aniState = LEFTWALK;
        tools[i].obj.sprite.curFrame = 0;
        tools[i].obj.sprite.numFrames = 2;
        tools[i].obj.sprite.hide = 0;

        tools[i].obj.sprite.attributes = &shadowOAM[8 + BLOCKCOUNT + i];

        tools[i].ability = level.toolAbilities[i];

    } // for

    for (int i = numTools; i < TOOLCOUNT; i++) {
        tools[i].obj.sprite.hide = 1;
        tools[i].obj.active = 0;

    } // for
    
} // initTools

void drawTools() {

    for (int i = 0; i < numTools; i++) {

        if (tools[i].obj.sprite.hide) {
            tools[i].obj.sprite.attributes->attr0 |= ATTR0_HIDE;

        } else {
            tools[i].obj.sprite.attributes->attr0 = (tools[i].obj.sprite.worldRow - vOff) | ATTR0_SQUARE;
            tools[i].obj.sprite.attributes->attr1 = (tools[i].obj.sprite.worldCol - hOff) | ATTR1_SMALL;
            tools[i].obj.sprite.attributes->attr2 = ATTR2_PALROW(0) | ATTR2_TILEID(12, tools[i].obj.sprite.curFrame * 2);
        
        } // if

    } // for

} // drawTools