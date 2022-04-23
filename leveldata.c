#include "leveldata.h"

#include "mygbalib.h"
#include "game.h"

#include "spritesheet.h"
#include "cave.h"
#include "house.h"

void buildRm1();
void buildRm2();

void drawTile(int, int, int);

void buildRooms() {

    buildRm1();
    buildRm2();

} // buildLevels

void initLevel(int level) {

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

    for (int c = 0; c < levels[level - 1].mapWidth * 2; c += 2) {

        for (int r = 0; r < levels[level - 1].mapHeight * 2; r += 2) {
            
            int tileposition = levels[level - 1].mapTiles[OFFSET(c / 2, r / 2, mapWidth)];
            drawTile(c, r, tileposition);

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

} // initLevel

void buildRm1() {

    levels[0].mapWidth = 15;
    levels[0].mapHeight = 10;
    levels[0].mapXOffset = 0;
    levels[0].mapYOffset = 0;

    levels[0].hOff = 0;
    levels[0].vOff = 0;

    levels[0].zenithOrientation = FRONTWALK;

    COORDINATE zLoc = {1, 1};
    levels[0].zenithLoc = zLoc;

    COORDINATE bLoc = {3, 3};
    levels[0].blockLocs[0] = bLoc;

    COORDINATE pLoc = {4, 4};
    levels[0].plateLocs[0] = pLoc;

    COORDINATE tLoc = {0, 3};
    levels[0].toolLocs[0] = tLoc;

    levels[0].plateInitStates[0] = 0;
    levels[0].onFuncs[0] = &updateLevel;
    levels[0].offFuncs[0] = &doNothing;

    levels[0].toolAbilities[0] = &doNothing;

    levels[0].palLen = cavePalLen;
    levels[0].tileLen = caveTilesLen;
    levels[0].mapLen = caveMapLen;

    levels[0].pal = cavePal;
    levels[0].tiles = caveTiles;
    levels[0].map = caveMap;

    const unsigned short tileData[15 * 10] = {

        10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,
        11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,
        12,12,12,12,12,12,12,12,12,12,12,12,12,12,12,
        13,13,13,13,13,13,13,13,13,13,13,13,13,13,13,
        14,14,14,14,14,14,14,14,14,14,14,14,14,14,14,
        15,15,15,15,15,15,15,15,15,15,15,15,15,15,15,
        16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,
        17,17,17,17,17,17,17,17,17,17,17,17,17,17,17,
        18,18,18,18,18,18,18,18,18,18,18,18,18,18,18,
        19,19,19,19,19,19,19,19,19,19,19,19,19,19,19

    };

    DMANow(3, tileData, levels[0].mapTiles, 15 * 10);

} // buildLv1

void buildRm2() {

    levels[1].mapWidth = 16;
    levels[1].mapHeight = 16;
    levels[1].mapXOffset = 0;
    levels[1].mapYOffset = 0;

    levels[1].hOff = 0;
    levels[1].vOff = 0;

    levels[1].zenithOrientation = RIGHTWALK;

    COORDINATE zLoc2 = {1, 1};
    levels[1].zenithLoc = zLoc2;

    COORDINATE bLoc2 = {4, 4};
    levels[1].blockLocs[0] = bLoc2;

    COORDINATE pLoc2 = {4, 4};
    levels[1].plateLocs[0] = pLoc2;
    
    COORDINATE tLoc2 = {10, 2};
    levels[1].toolLocs[0] = tLoc2;

    levels[1].plateInitStates[0] = 1;
    levels[1].onFuncs[0] = &doNothing;
    levels[1].offFuncs[0] = &updateLevel;

    levels[1].toolAbilities[0] = &doNothing;

    levels[1].palLen = cavePalLen;
    levels[1].tileLen = caveTilesLen;
    levels[1].mapLen = caveMapLen;

    levels[1].pal = cavePal;
    levels[1].tiles = caveTiles;
    levels[1].map = caveMap;

    const unsigned short tileData[16 * 16] = {

        5,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1

    };

    DMANow(3, tileData, levels[1].mapTiles, 16 * 16);

} // buildLv2

void drawTile(int c, int r, int tileposition) {

    int tilecol = tileposition % 15;
    int tilerow = tileposition / 15;

    SCREENBLOCK[28].tilemap[OFFSET(c + 0, r + 0, 32)] = OFFSET(2 * tilecol + 0, 2 * tilerow + 0, 32);
    SCREENBLOCK[28].tilemap[OFFSET(c + 1, r + 0, 32)] = OFFSET(2 * tilecol + 1, 2 * tilerow + 0, 32);
    SCREENBLOCK[28].tilemap[OFFSET(c + 0, r + 1, 32)] = OFFSET(2 * tilecol + 0, 2 * tilerow + 1, 32);
    SCREENBLOCK[28].tilemap[OFFSET(c + 1, r + 1, 32)] = OFFSET(2 * tilecol + 1, 2 * tilerow + 1, 32);

} // drawTile