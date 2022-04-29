#ifndef LEVEL_DATA
#define LEVEL_DATA

#define LEVELCOUNT 3

#define BLOCKCOUNT 2
#define PLATECOUNT 1
#define TOOLCOUNT 3

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
    COORDINATE blockLocs[BLOCKCOUNT];
    COORDINATE plateLocs[PLATECOUNT];
    COORDINATE toolLocs[TOOLCOUNT];

    int plateInitStates[PLATECOUNT];
    void (*onFuncs[PLATECOUNT])(void);
    void (*offFuncs[PLATECOUNT])(void);

    void (*toolAbilities[TOOLCOUNT])(void);

    unsigned int palLen;
    unsigned int tileLen;
    unsigned int mapLen;

    const unsigned short* pal;
    const unsigned short* tiles;
    const unsigned short* map;

    unsigned short mapTiles[64 * 64];
    void (*setup)(void);

    int numBlocks;
    int numPlates;
    int numTools;

} LEVEL;

extern LEVEL levels[LEVELCOUNT];

extern int mapWidth;
extern int mapHeight;
extern int mapXOffset;
extern int mapYOffset;

extern int hOff;
extern int vOff;

void buildRooms();
LEVEL* initLevel(int);

int getTileId(int, int);
void drawTile(int, int, int);

void openDoor();
void closeDoor();

#endif