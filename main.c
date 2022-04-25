#include <stdlib.h> // Possible includePath errors

#include "mygbalib.h"
#include "print.h"

#include "game.h"

#include "sound.h"
#include "cavemusic.h"

#include "zenithtitle.h"
#include "cave.h"
#include "winscreen.h"
#include "losescreen.h"

// Prototypes.
void initialize();

// State Prototypes.
void goToStart();
void start();
void goToGame();
void game();
void goToPause();
void pause();
void goToWin();
void win();
void goToLose();
void lose();

// States.
enum {
    START,
    GAME,
    PAUSE,
    WIN,
    LOSE
};
int state;

// Button Variables.
unsigned short buttons;
unsigned short oldButtons;

// Seed.
int seed;

// Shadow OAM.
OBJ_ATTR shadowOAM[128];

int main() {

    initialize();

    while(1) {
        
        oldButtons = buttons;
        buttons = BUTTONS;

        switch (state) {

            case START: {
                start();
                break;

            } // START
            
            case GAME: {
                game();
                break;

            } // GAME
                
            case PAUSE: {
                pause();
                break;
                
            } // PAUSE
            
            case WIN: {
                win();
                break;
                
            } // WIN
            
            case LOSE: {
                lose();
                break;
                
            } // LOSE
            
            default: { break; }

        } // switch
    
    } // while

    return 0;

} // main

// Sets up GBA.
void initialize() {
    
    mgba_open();

    REG_DISPCTL = MODE0 | BG1_ENABLE | SPRITE_ENABLE;
    REG_BG1CNT = BG_CHARBLOCK(0) | BG_SCREENBLOCK(28) | BG_4BPP | BG_SIZE_LARGE;

    buttons = BUTTONS;
    oldButtons = 0;

    setupSounds();
    setupInterrupts();

    goToStart();
    // goToGame();

} // initialize

// Sets up the start state.
void goToStart() {
    
    hideSprites();

    DMANow(3, zenithtitlePal, PALETTE, 256);
    DMANow(3, zenithtitleTiles, &CHARBLOCK[0], zenithtitleTilesLen / 2);
    DMANow(3, zenithtitleMap, &SCREENBLOCK[28], zenithtitleMapLen / 2);

    REG_BG1VOFF = 0;
    REG_BG1HOFF = 0;

    waitForVBlank();
    updateOAM();
    
    state = START;

} // goToStart

// Runs every frame of the start state.
void start() {
    
    seed++;
    
    if (BUTTON_PRESSED(BUTTON_SELECT)) {
        srand(seed);
        goToGame();    
        initGame();

    } // if

} // start

// Sets up the game state.
void goToGame() {
    playSoundA(cavemusic_data, cavemusic_length, 1);
    state = GAME;
    
} // goToGame

// Runs every frame of the game state.
void game() {  

    updateGame();
    drawGame();

    if (gameWon) goToWin();
    if (gameOver) goToLose();
    if (BUTTON_PRESSED(BUTTON_START)) goToPause();

} // game

// Sets up the pause state.
void goToPause() {
    state = PAUSE;

} // goToPause

// Runs every frame of the pause state.
void pause() {

    shadowOAM[127].attr0 = 40 | ATTR0_4BPP | ATTR0_WIDE;
    shadowOAM[127].attr1 = (120 - 32)  | ATTR1_LARGE;
    shadowOAM[127].attr2 = ATTR2_TILEID(0, 20);

    waitForVBlank();
    updateOAM();

    if (BUTTON_PRESSED(BUTTON_START)) goToGame();
    if (BUTTON_PRESSED(BUTTON_SELECT)) goToStart();

} // pause

// Sets up the win state.
void goToWin() {
    
    DMANow(3, winscreenPal, PALETTE, 256);
    DMANow(3, winscreenTiles, &CHARBLOCK[0], winscreenTilesLen / 2);
    DMANow(3, winscreenMap, &SCREENBLOCK[28], winscreenMapLen / 2);

    hideSprites();

    waitForVBlank();
    updateOAM();
    
    state = WIN;

} // goToWin

// Runs every frame of the win state.
void win() {
    seed++;
    if (BUTTON_PRESSED(BUTTON_SELECT)) goToStart();

} // win

// Sets up the lose state.
void goToLose() {

    DMANow(3, losescreenPal, PALETTE, 256);
    DMANow(3, losescreenTiles, &CHARBLOCK[0], losescreenTilesLen / 2);
    DMANow(3, losescreenMap, &SCREENBLOCK[28], losescreenMapLen / 2);

    hideSprites();

    waitForVBlank();
    updateOAM();

    state = LOSE;
     
} // goToLose

// Runs every frame of the lose state.
void lose() {
    seed++;
    if (BUTTON_PRESSED(BUTTON_SELECT)) goToStart();

} // lose