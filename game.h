enum { FRONTWALK, BACKWALK, RIGHTWALK, LEFTWALK, IDLE };

extern int gameOver;
extern int gameWon;

void initGame();
void updateGame();
void drawGame();
void updateOAM();