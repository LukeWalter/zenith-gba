#include "game.h"

#include "sound.h"
#include "break.h"
#include "despawn.h"

PLAYER zenith;
BLOCK blocks[BLOCKCOUNT];
PLATE plates[PLATECOUNT];
TOOL tools[TOOLCOUNT];

TOOL cheaterShovel;

int gamePaused;
int gameWon;

int mapWidth;
int mapHeight;
int mapXOffset;
int mapYOffset;

int hOff;
int vOff;

int lv;
LEVEL* levelData;

void initGame() {

    initMemory();

    lv = 1;
    levelData = initLevel(lv);

    gamePaused = 0;
    gameWon = 0;

} // initGame

void updateLevel() {

    lv++;
    
    if (lv > LEVELCOUNT) gameWon = 1;
    else levelData = initLevel(lv);

} // updateLevel

void updateGame() {
    
    gamePaused = 0;
    if (BUTTON_PRESSED(BUTTON_START) && zenith.obj.idle) gamePaused = 1;
    if (BUTTON_PRESSED(BUTTON_L) && zenith.obj.idle) levelData = initLevel(lv);
    
    updateZenith();
    updatePlates();

    if (getTileId(zenith.obj.xPos, zenith.obj.yPos) == 30) updateLevel();

} // updateGame

void drawGame() {

    drawZenith();
    drawBlocks();
    drawPlates();
    drawTools();
    drawCheaterShovel();

    waitForVBlank();
    updateOAM();

    REG_BG1VOFF = vOff;
    REG_BG1HOFF = hOff;

} // drawGame
















int canMoveUp(OBJECT* obj) {

    if (obj->yTarget - 1 > -1) {
        
        int tileId = getTileId(obj->xTarget, obj->yTarget - 1);
        if (tileId != 0 && tileId != 30) return 0;

        for (int i = 0; i < numBlocks; i++) {
            
            if (blocks[i].obj.active && obj->yTarget - 1 == blocks[i].obj.yPos && obj->xPos == blocks[i].obj.xPos) {
                return 0;
        
            } // if

        } // for

        return 1;

    } // if

    return 0;

} // canMoveUp

int canMoveDown(OBJECT* obj) {

    if (obj->yTarget + 1 < mapHeight) {
        
        int tileId = getTileId(obj->xTarget, obj->yTarget + 1);
        if (tileId != 0 && tileId != 30) return 0;

        for (int i = 0; i < numBlocks; i++) {
            
            if (blocks[i].obj.active && obj->yTarget + 1 == blocks[i].obj.yPos && obj->xPos == blocks[i].obj.xPos) {
                return 0;
        
            } // if

        } // for

        return 1;

    } // if

    return 0;

} // canMoveDown

int canMoveLeft(OBJECT* obj) {

    if (obj->xTarget - 1 > -1) {
        
        int tileId = getTileId(obj->xTarget - 1, obj->yTarget);
        if (tileId != 0 && tileId != 30) return 0;

        for (int i = 0; i < numBlocks; i++) {
            
            if (blocks[i].obj.active && obj->xTarget - 1 == blocks[i].obj.xPos && obj->yPos == blocks[i].obj.yPos) {
                return 0;
        
            } // if

        } // for

        return 1;

    } // if

    return 0;

} // canMoveLeft

int canMoveRight(OBJECT* obj) {

    if (obj->xTarget + 1 < mapWidth) {
        
        int tileId = getTileId(obj->xTarget + 1, obj->yTarget);
        if (tileId != 0 && tileId != 30) return 0;

        for (int i = 0; i < numBlocks; i++) {
            
            if (blocks[i].obj.active && obj->xTarget + 1 == blocks[i].obj.xPos && obj->yPos == blocks[i].obj.yPos) {
                return 0;
        
            } // if

        } // for

        return 1;

    } // if

    return 0;

} // canMoveRight




void moveUp(OBJECT* obj) {

    obj->idle = 0;

    if (obj->sprite.worldRow <= 8 * mapYOffset + obj->yTarget * 16) {

        obj->yPos = obj->yTarget;
        obj->sprite.worldRow = 8 * mapYOffset + obj->yTarget * 16;
        obj->sprite.encodeWorldRow = obj->sprite.worldRow * obj->sprite.encodeFactor;
        

        obj->sprite.rdel = obj->baseSpeed;
        obj->idle = 1;

    } else {

        obj->sprite.encodeWorldRow -= obj->sprite.rdel;
        obj->sprite.worldRow = obj->sprite.encodeWorldRow / obj->sprite.encodeFactor;

        if ((obj->sprite.worldRow - vOff < SCREENHEIGHT / 2 - obj->sprite.height / 2) && (vOff > 0)) {
            vOff--;
        
        } // if

    } // if

} // moveUp

void moveDown(OBJECT* obj) {
    
    obj->idle = 0;

    if (obj->sprite.worldRow >= 8 * mapYOffset + obj->yTarget * 16) {

        obj->yPos = obj->yTarget;
        obj->sprite.worldRow = 8 * mapYOffset + obj->yPos * 16;
        obj->sprite.encodeWorldRow = 8 * obj->sprite.worldRow;

        obj->sprite.rdel = obj->baseSpeed;
        obj->idle = 1;

    } else {

        obj->sprite.encodeWorldRow += obj->sprite.rdel;
        obj->sprite.worldRow = obj->sprite.encodeWorldRow / obj->sprite.encodeFactor;

        if ((obj->sprite.worldRow - vOff > SCREENHEIGHT / 2 - obj->sprite.height / 2) && (vOff < (16 * mapHeight + 8 * mapYOffset) - SCREENHEIGHT)) {
            vOff++;
        
        } // if

    } // if

} // moveDown

void moveLeft(OBJECT* obj) {

    obj->idle = 0;

    if (obj->sprite.worldCol <= 8 * mapXOffset + obj->xTarget * 16) {

        obj->xPos = obj->xTarget;
        obj->sprite.worldCol = 8 * mapXOffset + obj->xTarget * 16;
        obj->sprite.encodeWorldCol = obj->sprite.worldCol * obj->sprite.encodeFactor;

        obj->sprite.cdel = obj->baseSpeed;
        obj->idle = 1;

    } else {

        obj->sprite.encodeWorldCol -= obj->sprite.cdel;
        obj->sprite.worldCol = obj->sprite.encodeWorldCol / obj->sprite.encodeFactor;

        if ((obj->sprite.worldCol - hOff < SCREENWIDTH / 2 - obj->sprite.width / 2) && (hOff > 0)) {
            hOff--;
    
        } // if

    } // if

} // moveLeft

void moveRight(OBJECT* obj) {

    obj->idle = 0;

    if (obj->sprite.worldCol >= 8 * mapXOffset + obj->xTarget * 16) {

        obj->xPos = obj->xTarget;
        obj->sprite.worldCol = 8 * mapXOffset + obj->xPos * 16;
        obj->sprite.encodeWorldCol = 8 * obj->sprite.worldCol;

        obj->sprite.cdel = obj->baseSpeed;
        obj->idle = 1;

    } else {

        obj->sprite.encodeWorldCol += obj->sprite.cdel;
        obj->sprite.worldCol = obj->sprite.encodeWorldCol / obj->sprite.encodeFactor;

        if ((obj->sprite.worldCol - hOff > SCREENWIDTH / 2 - obj->sprite.width / 2) && (hOff < (16 * mapWidth + 8 * mapYOffset) - SCREENWIDTH)) {
            hOff++;
    
        } // if

    } // if

} // moveRight




void readInput(OBJECT* obj) {

    if (obj->idle) {
        
        if (BUTTON_HELD(BUTTON_UP) && canMoveUp(obj)) {
            obj->sprite.aniState = BACKWALK;
            obj->yTarget--;
            
        } else if (BUTTON_HELD(BUTTON_DOWN) && canMoveDown(obj)) {
            obj->sprite.aniState = FRONTWALK;
            obj->yTarget++;
            
        } else if (BUTTON_HELD(BUTTON_LEFT) && canMoveLeft(obj)) {
            obj->sprite.aniState = LEFTWALK;
            obj->xTarget--;

        } else if (BUTTON_HELD(BUTTON_RIGHT) && canMoveRight(obj)) {
            obj->sprite.aniState = RIGHTWALK;
            obj->xTarget++;

        } else if (BUTTON_HELD(BUTTON_UP)) {
            obj->sprite.aniState = BACKWALK;
            
        } else if (BUTTON_HELD(BUTTON_DOWN)) {
            obj->sprite.aniState = FRONTWALK;
            
        } else if (BUTTON_HELD(BUTTON_LEFT)) {
            obj->sprite.aniState = LEFTWALK;

        } else if (BUTTON_HELD(BUTTON_RIGHT)) {
            obj->sprite.aniState = RIGHTWALK;

        } // if

    } // if

    if (obj->yTarget < obj->yPos)      moveUp(obj);
    else if (obj->yTarget > obj->yPos) moveDown(obj);
    else if (obj->xTarget < obj->xPos) moveLeft(obj);
    else if (obj->xTarget > obj->xPos) moveRight(obj);

} // readInput

int getTileId(int x, int y) {
    return levelData->mapTiles[OFFSET(x, y, mapWidth)];

} // getTileId


void pickaxeFunction() {

    if (BUTTON_PRESSED(BUTTON_A)) {

        int tileId;

        switch (zenith.obj.sprite.prevAniState) {
        
        case FRONTWALK:
            
            if (zenith.obj.yPos < mapHeight - 1) {
                
                tileId = getTileId(zenith.obj.xPos, zenith.obj.yPos + 1);

                if (tileId == 3 || tileId == 4) {
                    playSoundB(break_data, break_length, 0);
                    levelData->mapTiles[OFFSET(zenith.obj.xPos, zenith.obj.yPos + 1, mapWidth)] = 0;
                    drawTile(zenith.obj.xPos * 2, (zenith.obj.yPos + 1) * 2, 1);

                } // if
            
            } // if
            
            break;
        
        case BACKWALK:
            
            if (zenith.obj.yPos > 0) {
                
                tileId = getTileId(zenith.obj.xPos, zenith.obj.yPos - 1);

                if (tileId == 3 || tileId == 4) {
                    playSoundB(break_data, break_length, 0);
                    levelData->mapTiles[OFFSET(zenith.obj.xPos, zenith.obj.yPos - 1, mapWidth)] = 0;
                    drawTile(zenith.obj.xPos * 2, (zenith.obj.yPos - 1) * 2, 1);

                } // if
            
            } // if
            
            break;
        
        case LEFTWALK:
            
            if (zenith.obj.xPos > 0) {
                
                tileId = getTileId(zenith.obj.xPos - 1, zenith.obj.yPos);

                if (tileId == 3 || tileId == 4) {
                    playSoundB(break_data, break_length, 0);
                    levelData->mapTiles[OFFSET(zenith.obj.xPos - 1, zenith.obj.yPos, mapWidth)] = 0;
                    drawTile((zenith.obj.xPos - 1) * 2, zenith.obj.yPos * 2, 1);

                } // if
            
            } // if
            
            break;
        
        case RIGHTWALK:
            
            if (zenith.obj.xPos < mapWidth - 1) {
                
                tileId = getTileId(zenith.obj.xPos + 1, zenith.obj.yPos);

                if (tileId == 3 || tileId == 4) {
                    playSoundB(break_data, break_length, 0);
                    levelData->mapTiles[OFFSET(zenith.obj.xPos + 1, zenith.obj.yPos, mapWidth)] = 0;
                    drawTile((zenith.obj.xPos + 1) * 2, zenith.obj.yPos * 2, 1);

                } // if
            
            } // if
            
            break;

        default:
            break;

        } // switch

    } // if
    
} // pickaxeFunction


void spawnCheaterShovel() {

    int spawnable = 1;

    for (int i = 0; i < numTools; i++) {

        if (tools[i].obj.active && tools[i].obj.xPos == zenith.obj.xPos && tools[i].obj.yPos == zenith.obj.yPos) {
            spawnable = 0;
            break;

        } // if

    } // for

    for (int i = 0; i < numPlates; i++) {

        if (plates[i].obj.active && plates[i].obj.xPos == zenith.obj.xPos && plates[i].obj.yPos == zenith.obj.yPos) {
            spawnable = 0;
            break;

        } // if

    } // for

    if (spawnable) {

        cheaterShovel.obj.xPos = zenith.obj.xPos;
        cheaterShovel.obj.yPos = zenith.obj.yPos;
        cheaterShovel.obj.xTarget = cheaterShovel.obj.xPos;
        cheaterShovel.obj.yTarget = cheaterShovel.obj.yPos;
        cheaterShovel.obj.baseSpeed = 0;
        cheaterShovel.obj.idle = 1;
        cheaterShovel.obj.active = 1;

        cheaterShovel.obj.sprite.worldRow = 8 * mapYOffset + cheaterShovel.obj.yPos * 16;
        cheaterShovel.obj.sprite.worldCol = 8 * mapXOffset + cheaterShovel.obj.xPos * 16;

        cheaterShovel.obj.sprite.encodeFactor = 8;
        cheaterShovel.obj.sprite.encodeWorldRow = cheaterShovel.obj.sprite.worldRow * cheaterShovel.obj.sprite.encodeFactor;
        cheaterShovel.obj.sprite.encodeWorldCol = cheaterShovel.obj.sprite.worldCol * cheaterShovel.obj.sprite.encodeFactor;

        cheaterShovel.obj.sprite.rdel = cheaterShovel.obj.baseSpeed;
        cheaterShovel.obj.sprite.cdel = cheaterShovel.obj.baseSpeed;

        cheaterShovel.obj.sprite.width = 16;
        cheaterShovel.obj.sprite.height = 16;

        cheaterShovel.obj.sprite.aniCounter = 0;
        cheaterShovel.obj.sprite.aniState = LEFTWALK;
        cheaterShovel.obj.sprite.curFrame = 0;
        cheaterShovel.obj.sprite.numFrames = 2;
        cheaterShovel.obj.sprite.hide = 0;

        cheaterShovel.obj.sprite.attributes = &shadowOAM[100];

        cheaterShovel.ability = &cheaterShovelFunction;

    } // if

} // spawnCheaterShovel

void cheaterShovelFunction() {

    int tileId;

    switch (zenith.obj.sprite.prevAniState) {
        
    case FRONTWALK:
            
        if (zenith.obj.yPos < mapHeight - 1) {
                
            tileId = getTileId(zenith.obj.xPos, zenith.obj.yPos + 1);

            if (tileId == 3 || tileId == 4) {
                playSoundB(despawn_data, despawn_length, 0);
                levelData->mapTiles[OFFSET(zenith.obj.xPos, zenith.obj.yPos + 1, mapWidth)] = 0;
                drawTile(zenith.obj.xPos * 2, (zenith.obj.yPos + 1) * 2, 1);

            } // if
            
        } // if

        for (int i = 0; i < numBlocks; i++) {
            
            if (zenith.obj.xPos == blocks[i].obj.xPos && zenith.obj.yPos + 1 == blocks[i].obj.yPos) {
                playSoundB(despawn_data, despawn_length, 0);
                blocks[i].obj.sprite.hide = 1;
                blocks[i].obj.active = 0;
                break;
        
            } // if

        } // for
            
        break;
        
    case BACKWALK:
            
        if (zenith.obj.yPos > 0) {
                
            tileId = getTileId(zenith.obj.xPos, zenith.obj.yPos - 1);

            if (tileId == 3 || tileId == 4) {
                playSoundB(despawn_data, despawn_length, 0);
                levelData->mapTiles[OFFSET(zenith.obj.xPos, zenith.obj.yPos - 1, mapWidth)] = 0;
                drawTile(zenith.obj.xPos * 2, (zenith.obj.yPos - 1) * 2, 1);

            } else if (tileId == 33) {
                playSoundB(despawn_data, despawn_length, 0);
                levelData->mapTiles[OFFSET(zenith.obj.xPos, zenith.obj.yPos - 1, mapWidth)] = 30;
                drawTile(zenith.obj.xPos * 2, (zenith.obj.yPos - 1) * 2, 30);
                levelData->mapTiles[OFFSET(zenith.obj.xPos, zenith.obj.yPos - 2, mapWidth)] = 15;
                drawTile(zenith.obj.xPos * 2, (zenith.obj.yPos - 2) * 2, 15);

            } // tileId
            
        } // if

        for (int i = 0; i < numBlocks; i++) {
            
            if (zenith.obj.xPos == blocks[i].obj.xPos && zenith.obj.yPos - 1 == blocks[i].obj.yPos) {
                playSoundB(despawn_data, despawn_length, 0);
                blocks[i].obj.sprite.hide = 1;
                blocks[i].obj.active = 0;
                break;
        
            } // if

        } // for
            
        break;
        
    case LEFTWALK:
            
        if (zenith.obj.xPos > 0) {
                
            tileId = getTileId(zenith.obj.xPos - 1, zenith.obj.yPos);

            if (tileId == 3 || tileId == 4) {
                playSoundB(despawn_data, despawn_length, 0);
                levelData->mapTiles[OFFSET(zenith.obj.xPos - 1, zenith.obj.yPos, mapWidth)] = 0;
                drawTile((zenith.obj.xPos - 1) * 2, zenith.obj.yPos * 2, 1);

            } // if
            
        } // if

        for (int i = 0; i < numBlocks; i++) {
            
            if (zenith.obj.xPos - 1 == blocks[i].obj.xPos && zenith.obj.yPos == blocks[i].obj.yPos) {
                playSoundB(despawn_data, despawn_length, 0);
                blocks[i].obj.sprite.hide = 1;
                blocks[i].obj.active = 0;
                break;
        
            } // if

        } // for
            
        break;
        
    case RIGHTWALK:
            
        if (zenith.obj.xPos < mapWidth - 1) {
                
            tileId = getTileId(zenith.obj.xPos + 1, zenith.obj.yPos);

            if (tileId == 3 || tileId == 4) {
                playSoundB(despawn_data, despawn_length, 0);
                levelData->mapTiles[OFFSET(zenith.obj.xPos + 1, zenith.obj.yPos, mapWidth)] = 0;
                drawTile((zenith.obj.xPos + 1) * 2, zenith.obj.yPos * 2, 1);

            } // if
            
        } // if

        for (int i = 0; i < numBlocks; i++) {
            
            if (zenith.obj.xPos + 1 == blocks[i].obj.xPos && zenith.obj.yPos == blocks[i].obj.yPos) {
                playSoundB(despawn_data, despawn_length, 0);
                blocks[i].obj.sprite.hide = 1;
                blocks[i].obj.active = 0;
                break;
        
            } // if

        } // for
            
        break;

    default:
        break;

    } // switch

} // cheaterShovelFunction

void drawCheaterShovel() {

    if (cheaterShovel.obj.sprite.hide) {
        cheaterShovel.obj.sprite.attributes->attr0 |= ATTR0_HIDE;

    } else {
        cheaterShovel.obj.sprite.attributes->attr0 = (cheaterShovel.obj.sprite.worldRow - vOff) | ATTR0_SQUARE;
        cheaterShovel.obj.sprite.attributes->attr1 = (cheaterShovel.obj.sprite.worldCol - hOff) | ATTR1_SMALL;
        cheaterShovel.obj.sprite.attributes->attr2 = ATTR2_PALROW(0) | ATTR2_TILEID(14, 4);
        
    } // if

} // drawCheaterShovel