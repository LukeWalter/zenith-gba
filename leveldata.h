#ifndef LEVEL_DATA
#define LEVEL_DATA

#define LEVELCOUNT 2

typedef struct {
    int col;
    int row;

} COORDINATE;

typedef struct {
    
    int mapWidth;
    int mapHeight;
    int mapXOffset;
    int mapYOffset;

    int hOff;
    int vOff;

    int zenithOrientation;
    
    COORDINATE zenithLoc;
    COORDINATE blockLocs[1];
    COORDINATE plateLocs[1];

    unsigned int palLen;
    unsigned int tileLen;
    unsigned int mapLen;

    const unsigned short pal[256];
    const unsigned short tiles[1024];
    const unsigned short map[1024];

} LEVEL;

extern LEVEL levels[LEVELCOUNT];

extern int mapWidth;
extern int mapHeight;
extern int mapXOffset;
extern int mapYOffset;

extern int hOff;
extern int vOff;

void buildRooms();
void initLevel(int);

#endif