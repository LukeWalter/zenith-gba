#include "leveldata.h"

#include "mygbalib.h"
#include "game.h"

#include "spritesheet.h"
#include "cave.h"
#include "house.h"

#include "sound.h"
#include "door.h"

void buildRm1();
void buildRm2();

void setupMapOne();

void buildRooms() {

    buildRm1();
    buildRm2();

} // buildLevels

LEVEL* initLevel(int level) {

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

    for (int c = 0; c < levels[level - 1].mapWidth; c++) {

        for (int r = 0; r < levels[level - 1].mapHeight; r++) {
            
            int tileposition = levels[level - 1].mapTiles[OFFSET(c, r, mapWidth)];
            drawTile(c * 2, r * 2, tileposition);

        }  // for

    } // for

    DMANow(3, spritesheetPal, SPRITEPALETTE, spritesheetPalLen / 2);
    DMANow(3, spritesheetTiles, &CHARBLOCK[4], spritesheetTilesLen / 2);
    hideSprites();
    updateOAM();

    initZenith(levels[level - 1]);
    initBlocks(levels[level - 1]);
    initPlates(levels[level - 1]);
    initTools(levels[level - 1]);

    return &levels[level - 1];

} // initLevel

void buildRm1() {

    levels[0].mapWidth = 15;
    levels[0].mapHeight = 10;
    levels[0].mapXOffset = 0;
    levels[0].mapYOffset = 0;

    levels[0].hOff = 0;
    levels[0].vOff = 0;

    levels[0].zenithOrientation = RIGHTWALK;

    levels[0].numBlocks = 1;
    levels[0].numPlates = 1;
    levels[0].numTools = 0;

    COORDINATE zLoc = {1, 6};
    levels[0].zenithLoc = zLoc;

    COORDINATE bLoc = {11, 7};
    levels[0].blockLocs[0] = bLoc;

    COORDINATE pLoc = {7, 5};
    levels[0].plateLocs[0] = pLoc;

    levels[0].plateInitStates[0] = 0;
    levels[0].onFuncs[0] = &openDoor;
    levels[0].offFuncs[0] = &closeDoor;

    levels[0].palLen = cavePalLen;
    levels[0].tileLen = caveTilesLen;
    levels[0].mapLen = caveMapLen;

    levels[0].pal = cavePal;
    levels[0].tiles = caveTiles;
    levels[0].map = caveMap;

    const unsigned short tileData[15 * 10] = {

        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        5,6,5,6,5,6,5,6,5,6,5,6,5,6,5,
        5,6,5,6,5,6,5,18,5,6,5,6,5,6,5,
        5,6,5,6,5,6,5,33,5,6,5,6,5,13,1,
        0,0,0,0,0,0,0,0,0,0,0,0,0,7,1,
        0,0,0,0,0,0,0,0,0,0,0,0,0,7,1,
        0,0,0,0,0,0,0,0,0,0,0,0,0,7,1,
        0,0,0,0,0,0,0,0,0,0,0,0,0,7,1,
        0,0,0,0,0,0,0,0,0,0,0,0,0,7,1,
        2,2,2,2,2,2,2,2,2,2,2,2,2,2,1

    };

    DMANow(3, tileData, levels[0].mapTiles, 15 * 10);

    levels[0].setup = &setupMapOne;

} // buildLv1

void buildRm2() {

    levels[1].mapWidth = 15;
    levels[1].mapHeight = 10;
    levels[1].mapXOffset = 0;
    levels[1].mapYOffset = 0;

    levels[1].hOff = 0;
    levels[1].vOff = 0;

    levels[1].zenithOrientation = BACKWALK;

    levels[1].numBlocks = 1;
    levels[1].numPlates = 0;
    levels[1].numTools = 1;

    COORDINATE zLoc2 = {7, 8};
    levels[1].zenithLoc = zLoc2;

    COORDINATE bLoc2 = {7, 4};
    levels[1].blockLocs[0] = bLoc2;

    
    COORDINATE tLoc2 = {4, 4};
    levels[1].toolLocs[0] = tLoc2;

    levels[1].toolAbilities[0] = &doNothing;

    levels[1].palLen = cavePalLen;
    levels[1].tileLen = caveTilesLen;
    levels[1].mapLen = caveMapLen;

    levels[1].pal = cavePal;
    levels[1].tiles = caveTiles;
    levels[1].map = caveMap;

    const unsigned short tileData[15 * 10] = {

        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        5,6,5,6,5,6,5,6,5,6,5,6,5,13,1,
        5,6,5,6,5,6,5,15,5,6,5,6,13,7,1,
        5,6,5,6,5,6,5,30,5,6,5,13,7,7,1,
        5,6,5,13,0,0,3,0,4,0,0,7,7,7,1,
        5,6,13,7,0,0,0,0,0,0,0,7,7,7,1,
        5,13,7,7,0,0,0,0,0,0,0,7,7,7,1,
        1,7,7,7,0,0,0,0,0,0,0,7,7,7,1,
        1,7,7,7,0,0,0,0,0,0,0,7,7,7,1,
        2,2,2,2,2,2,2,2,2,2,2,2,2,2,1

    };

    DMANow(3, tileData, levels[1].mapTiles, 16 * 16);

    levels[1].setup = &doNothing;

} // buildLv2

void drawTile(int c, int r, int tileposition) {

    int tilecol = tileposition % 15;
    int tilerow = tileposition / 15;

    SCREENBLOCK[28].tilemap[OFFSET(c + 0, r + 0, 32)] = OFFSET(2 * tilecol + 0, 2 * tilerow + 0, 32);
    SCREENBLOCK[28].tilemap[OFFSET(c + 1, r + 0, 32)] = OFFSET(2 * tilecol + 1, 2 * tilerow + 0, 32);
    SCREENBLOCK[28].tilemap[OFFSET(c + 0, r + 1, 32)] = OFFSET(2 * tilecol + 0, 2 * tilerow + 1, 32);
    SCREENBLOCK[28].tilemap[OFFSET(c + 1, r + 1, 32)] = OFFSET(2 * tilecol + 1, 2 * tilerow + 1, 32);

} // drawTile

void openDoor() {
    
    playSoundB(door_data, door_length, 0);

    int tileId = getTileId(7, 3);

    if (tileId == 33) {

        levelData->mapTiles[OFFSET(7, 2, levelData->mapWidth)] -= 3;
        levelData->mapTiles[OFFSET(7, 3, levelData->mapWidth)] -= 3;

        drawTile(7 * 2, 2 * 2, tileId - 15 - 3);
        drawTile(7 * 2, 3 * 2, tileId - 3);

    } // if
    
} // openDoor

void closeDoor() {
    
    playSoundB(door_data, door_length, 0);

    int tileId = getTileId(7, 3);

    if (tileId == 30) {
        
        levelData->mapTiles[OFFSET(7, 2, levelData->mapWidth)] += 3;
        levelData->mapTiles[OFFSET(7, 3, levelData->mapWidth)] += 3;

        drawTile(7 * 2, 2 * 2, tileId - 15 + 3);
        drawTile(7 * 2, 3 * 2, tileId + 3);

    } // if


} // closeDoor

void setupMapOne() {

    const unsigned short tileData[15 * 10] = {

        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        5,6,5,6,5,6,5,6,5,6,5,6,5,6,5,
        5,6,5,6,5,6,5,18,5,6,5,6,5,6,5,
        5,6,5,6,5,6,5,33,5,6,5,6,5,13,1,
        0,0,0,0,0,0,0,0,0,0,0,0,0,7,1,
        0,0,0,0,0,0,0,0,0,0,0,0,0,7,1,
        0,0,0,0,0,0,0,0,0,0,0,0,0,7,1,
        0,0,0,0,0,0,0,0,0,0,0,0,0,7,1,
        0,0,0,0,0,0,0,0,0,0,0,0,0,7,1,
        2,2,2,2,2,2,2,2,2,2,2,2,2,2,1

    };

    DMANow(3, tileData, levels[0].mapTiles, 15 * 10);

} // setupMapOne