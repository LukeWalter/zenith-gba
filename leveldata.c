#include "leveldata.h"

#include "mygbalib.h"
#include "game.h"

#include "cave.h"
#include "house.h"

void buildLevels() {

    // Level 1

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


    // Level 2

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

} // buildLevels