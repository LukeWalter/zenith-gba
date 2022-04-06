#define LEVELCOUNT 2

#define BLOCKCOUNT 1
#define PLATECOUNT 1

enum { FRONTWALK, BACKWALK, RIGHTWALK, LEFTWALK, IDLE };

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

    unsigned int palLen;
    unsigned int tileLen;
    unsigned int mapLen;

    const unsigned short pal[256];
    const unsigned short tiles[1024];
    const unsigned short map[1024];

} LEVEL;

extern LEVEL levels[LEVELCOUNT];

extern int gameOver;
extern int gameWon;

void initGame();
void updateGame();
void drawGame();