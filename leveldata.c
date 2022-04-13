#include "leveldata.h"

#include "mygbalib.h"
#include "game.h"

#include "spritesheet.h"
#include "cave.h"
#include "house.h"

void buildRm1();
void buildRm2();

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

    DMANow(3, spritesheetPal, SPRITEPALETTE, spritesheetPalLen / 2);
    DMANow(3, spritesheetTiles, &CHARBLOCK[4], spritesheetTilesLen / 2);
    hideSprites();
    updateOAM();

    initZenith(levels[level - 1].zenithOrientation, levels[level - 1].zenithLoc);
    initBlocks(levels[level - 1].blockLocs);
    initPlates(levels[level - 1].plateLocs);

} // initLevel

void buildRm1() {

    levels[0].mapWidth = 11;
    levels[0].mapHeight = 7;
    levels[0].mapXOffset = 4;
    levels[0].mapYOffset = 3;

    levels[0].hOff = 0;
    levels[0].vOff = 0;

    levels[0].zenithOrientation = FRONTWALK;

    COORDINATE zLoc = {0, 0};
    levels[0].zenithLoc = zLoc;

    COORDINATE bLoc = {3, 3};
    levels[0].blockLocs[0] = bLoc;

    COORDINATE pLoc = {4, 4};
    levels[0].plateLocs[0] = pLoc;

    levels[0].palLen = cavePalLen;
    levels[0].tileLen = caveTilesLen;
    levels[0].mapLen = caveMapLen;

    DMANow(3, cavePal, (volatile void*) levels[0].pal, 256);
    DMANow(3, caveTiles, (volatile void*) levels[0].tiles, houseTilesLen / 2);
    DMANow(3, caveMap, (volatile void*) levels[0].map, houseMapLen / 2);

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

    COORDINATE pLoc2 = {5, 5};
    levels[1].plateLocs[0] = pLoc2;

    levels[1].palLen = housePalLen;
    levels[1].tileLen = houseTilesLen;
    levels[1].mapLen = houseMapLen;

    DMANow(3, housePal, (volatile void*) levels[1].pal, 256);
    DMANow(3, houseTiles, (volatile void*) levels[1].tiles, houseTilesLen / 2);
    DMANow(3, houseMap, (volatile void*) levels[1].map, houseMapLen / 2);

} // buildLv2