#include "game.h"

PLAYER zenith;
BLOCK blocks[BLOCKCOUNT];
PLATE plates[PLATECOUNT];
TOOL tools[TOOLCOUNT];

LEVEL levels[LEVELCOUNT];

int gamePaused;
int gameWon;

int mapWidth;
int mapHeight;
int mapXOffset;
int mapYOffset;

int hOff;
int vOff;

int level;
LEVEL* levelData;

void initGame() {

    buildRooms();

    level = 2;
    levelData = initLevel(level);

    gamePaused = 0;
    gameWon = 0;

} // initGame

void updateLevel() {
    
    level++;
    
    if (level > LEVELCOUNT) gameWon = 1;
    else levelData = initLevel(level);
    
} // updateLevel

void updateGame() {
    
    gamePaused = 0;
    if (BUTTON_PRESSED(BUTTON_START) && zenith.obj.idle) gamePaused = 1;
    if (BUTTON_PRESSED(BUTTON_L) && zenith.obj.idle) levelData = initLevel(level);
    
    updateZenith();
    updatePlates();

    if (getTileId(zenith.obj.xPos, zenith.obj.yPos) == 30) updateLevel();

} // updateGame

void drawGame() {

    drawZenith();
    drawBlocks();
    drawPlates();
    drawTools();

    waitForVBlank();
    updateOAM();

    REG_BG1VOFF = vOff;
    REG_BG1HOFF = hOff;

} // drawGame
















int canMoveUp(OBJECT* obj) {

    if (obj->yTarget - 1 > -1) {
        
        int tileId = getTileId(obj->xTarget, obj->yTarget - 1);
        if (tileId != 0 && tileId != 30) return 0;

        for (int i = 0; i < BLOCKCOUNT; i++) {
            
            if (obj->yTarget - 1 == blocks[i].obj.yPos && obj->xPos == blocks[i].obj.xPos) {
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

        for (int i = 0; i < BLOCKCOUNT; i++) {
            
            if (obj->yTarget + 1 == blocks[i].obj.yPos && obj->xPos == blocks[i].obj.xPos) {
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

        for (int i = 0; i < BLOCKCOUNT; i++) {
            
            if (obj->xTarget - 1 == blocks[i].obj.xPos && obj->yPos == blocks[i].obj.yPos) {
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

        for (int i = 0; i < BLOCKCOUNT; i++) {
            
            if (obj->xTarget + 1 == blocks[i].obj.xPos && obj->yPos == blocks[i].obj.yPos) {
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
                    levelData->mapTiles[OFFSET(zenith.obj.xPos, zenith.obj.yPos + 1, mapWidth)] = 0;
                    drawTile(zenith.obj.xPos * 2, (zenith.obj.yPos + 1) * 2, 1);

                } // if
            
            } // if
            
            break;
        
        case BACKWALK:
            
            if (zenith.obj.yPos > 0) {
                
                tileId = getTileId(zenith.obj.xPos, zenith.obj.yPos - 1);

                if (tileId == 3 || tileId == 4) {
                    levelData->mapTiles[OFFSET(zenith.obj.xPos, zenith.obj.yPos - 1, mapWidth)] = 0;
                    drawTile(zenith.obj.xPos * 2, (zenith.obj.yPos - 1) * 2, 1);

                } // if
            
            } // if
            
            break;
        
        case LEFTWALK:
            
            if (zenith.obj.xPos > 0) {
                
                tileId = getTileId(zenith.obj.xPos - 1, zenith.obj.yPos);

                if (tileId == 3 || tileId == 4) {
                    levelData->mapTiles[OFFSET(zenith.obj.xPos - 1, zenith.obj.yPos, mapWidth)] = 0;
                    drawTile((zenith.obj.xPos - 1) * 2, zenith.obj.yPos * 2, 1);

                } // if
            
            } // if
            
            break;
        
        case RIGHTWALK:
            
            if (zenith.obj.xPos < mapWidth - 1) {
                
                tileId = getTileId(zenith.obj.xPos + 1, zenith.obj.yPos);

                if (tileId == 3 || tileId == 4) {
                    levelData->mapTiles[OFFSET(zenith.obj.xPos + 1, zenith.obj.yPos, mapWidth)] = 0;
                    drawTile((zenith.obj.xPos + 1) * 2, zenith.obj.yPos * 2, 1);

                } // if
            
            } // if
            
            break;

        default:
            break;

        } // switch

    } // if
    
} // pickaxe