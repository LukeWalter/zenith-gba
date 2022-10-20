#include "leveldata.h"

#include "mygbalib.h"
#include "game.h"

#include "spritesheet.h"
#include "cave.h"
#include "house.h"

#include "sound.h"
#include "door.h"

LEVEL* level;

void buildRm1();
void buildRm2();

void setupMapOne();
void setupMapTwo();

void allocate() {
    level = malloc(sizeof(LEVEL));
    if (level == NULL) {
        mgba_printf("Too much memory >_<");

    } // if

} // allocate

void initMemory() {
    allocate();
    buildLevel[0] = &buildRm1;
    buildLevel[1] = &buildRm2;

} // buildLevels

LEVEL* initLevel(int lv) {

    buildLevel[lv - 1]();

    hideSprites();
    cheaterShovel.obj.active = 0;
    cheaterShovel.obj.sprite.hide = 1;

    // Measured in double tiles
    mapWidth = level->mapWidth;
    mapHeight = level->mapHeight;

    // Measured in single tiles
    mapXOffset = level->mapXOffset;
    mapYOffset = level->mapYOffset;

    hOff = level->hOff;
    vOff = level->vOff;

    level->setup();

    DMANow(3, level->pal, PALETTE, 256);
    DMANow(3, level->tiles, &CHARBLOCK[0], level->tileLen / 2);
    DMANow(3, level->map, &SCREENBLOCK[28], level->mapLen / 2);
    REG_BG1VOFF = vOff;
    REG_BG1HOFF = hOff;

    for (int c = 0; c < level->mapWidth; c++) {

        for (int r = 0; r < level->mapHeight; r++) {
            
            int tileposition = level->mapTiles[OFFSET(c, r, mapWidth)];
            drawTile(c * 2, r * 2, tileposition);

        }  // for

    } // for

    DMANow(3, spritesheetPal, SPRITEPALETTE, spritesheetPalLen / 2);
    DMANow(3, spritesheetTiles, &CHARBLOCK[4], spritesheetTilesLen / 2);
    hideSprites();
    updateOAM();

    initZenith(*level);
    initBlocks(*level);
    initPlates(*level);
    initTools(*level);

    return level;

} // initLevel

void buildRm1() {

    level->mapWidth = 15;
    level->mapHeight = 10;
    level->mapXOffset = 0;
    level->mapYOffset = 0;

    level->hOff = 0;
    level->vOff = 0;

    level->zenithOrientation = RIGHTWALK;

    level->numBlocks = 1;
    level->numPlates = 1;
    level->numTools = 0;

    COORDINATE zLoc = {1, 6};
    level->zenithLoc = zLoc;

    COORDINATE bLoc = {11, 7};
    level->blockLocs[0] = bLoc;

    COORDINATE pLoc = {7, 5};
    level->plateLocs[0] = pLoc;

    level->plateInitStates[0] = 0;
    level->onFuncs[0] = &openDoor;
    level->offFuncs[0] = &closeDoor;

    level->palLen = cavePalLen;
    level->tileLen = caveTilesLen;
    level->mapLen = caveMapLen;

    level->pal = cavePal;
    level->tiles = caveTiles;
    level->map = caveMap;

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

    DMANow(3, tileData, level->mapTiles, 15 * 10);

    level->setup = &setupMapOne;

} // buildLv1

void buildRm2() {

    level->mapWidth = 15;
    level->mapHeight = 10;
    level->mapXOffset = 0;
    level->mapYOffset = 0;

    level->hOff = 0;
    level->vOff = 0;

    level->zenithOrientation = BACKWALK;

    level->numBlocks = 1;
    level->numPlates = 0;
    level->numTools = 1;

    COORDINATE zLoc2 = {7, 8};
    level->zenithLoc = zLoc2;

    COORDINATE bLoc2 = {7, 4};
    level->blockLocs[0] = bLoc2;

    
    COORDINATE tLoc2 = {4, 4};
    level->toolLocs[0] = tLoc2;

    level->toolAbilities[0] = &pickaxeFunction;

    level->palLen = cavePalLen;
    level->tileLen = caveTilesLen;
    level->mapLen = caveMapLen;

    level->pal = cavePal;
    level->tiles = caveTiles;
    level->map = caveMap;

    const unsigned short tileData[15 * 10] = {

        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        5,6,5,6,5,6,5,6,5,6,5,6,5,13,1,
        5,6,5,6,5,6,5,15,5,6,5,6,13,7,1,
        5,6,5,6,5,6,5,30,5,6,5,13,7,7,1,
        5,6,5,13,0,0,3,0,4,0,0,7,7,7,1,
        5,6,13,7,0,4,0,0,0,0,0,7,7,7,1,
        5,13,7,7,0,0,0,4,0,3,0,7,7,7,1,
        1,7,7,7,0,0,0,0,0,0,0,7,7,7,1,
        1,7,7,7,3,0,0,0,0,0,0,7,7,7,1,
        2,2,2,2,2,2,2,2,2,2,2,2,2,2,1

    };

    DMANow(3, tileData, level->mapTiles, 15 * 10);

    level->setup = &setupMapTwo;

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

    DMANow(3, tileData, level->mapTiles, 15 * 10);

} // setupMapOne

void setupMapTwo() {

    const unsigned short tileData[15 * 10] = {

        1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
        5,6,5,6,5,6,5,6,5,6,5,6,5,13,1,
        5,6,5,6,5,6,5,15,5,6,5,6,13,7,1,
        5,6,5,6,5,6,5,30,5,6,5,13,7,7,1,
        5,6,5,13,0,0,3,0,4,0,0,7,7,7,1,
        5,6,13,7,0,4,0,0,0,0,0,7,7,7,1,
        5,13,7,7,0,0,0,4,0,3,0,7,7,7,1,
        1,7,7,7,0,0,0,0,0,0,0,7,7,7,1,
        1,7,7,7,3,0,0,0,0,0,0,7,7,7,1,
        2,2,2,2,2,2,2,2,2,2,2,2,2,2,1

    };

    DMANow(3, tileData, level->mapTiles, 15 * 10);

} // setupMapTwo