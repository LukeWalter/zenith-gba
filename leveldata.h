#ifndef LEVEL_DATA
#define LEVEL_DATA

#define LEVELCOUNT 2

#define BLOCKCOUNT 1
#define PLATECOUNT 1
#define TOOLCOUNT 1

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

    const volatile void* pal;
    const volatile void* tiles;
    const volatile void* map;

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