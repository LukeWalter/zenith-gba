#include <stdlib.h> // Possible includePath errors

#include "mygbalib.h"
#include "print.h"

#include "game.h"

#include "sound.h"
#include "cavemusic.h"
#include "titlemusic.h"
#include "gamecomplete.h"

#include "zenithtitle.h"
#include "cave.h"
#include "winscreen.h"
#include "losescreen.h"

#include "spritesheet.h"

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

void enableTimer();
void disableTimer();

void setupInterrupts();
void interruptHandler();

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
    
    DMANow(3, spritesheetPal, SPRITEPALETTE, spritesheetPalLen / 2);
    DMANow(3, spritesheetTiles, &CHARBLOCK[4], spritesheetTilesLen / 2);
    hideSprites();

    shadowOAM[127].attr0 = 80 | ATTR0_4BPP | ATTR0_WIDE;
    shadowOAM[127].attr1 = 50 | ATTR1_LARGE;
    shadowOAM[127].attr2 = ATTR2_TILEID(16, 21);

    shadowOAM[126].attr0 = 80 | ATTR0_4BPP | ATTR0_WIDE;
    shadowOAM[126].attr1 = 114 | ATTR1_LARGE;
    shadowOAM[126].attr2 = ATTR2_TILEID(24, 21);

    DMANow(3, zenithtitlePal, PALETTE, 256);
    DMANow(3, zenithtitleTiles, &CHARBLOCK[0], zenithtitleTilesLen / 2);
    DMANow(3, zenithtitleMap, &SCREENBLOCK[28], zenithtitleMapLen / 2);

    REG_BG1VOFF = 0;
    REG_BG1HOFF = 0;

    waitForVBlank();
    updateOAM();
    
    enableTimer();
    playSoundA(titlemusic_data, titlemusic_length, 1);
    state = START;

} // goToStart

// Runs every frame of the start state.
void start() {
    
    if (BUTTON_PRESSED(BUTTON_START)) {

        disableTimer();
        playSoundA(cavemusic_data, cavemusic_length, 1);
        goToGame();    
        initGame();

    } // if

} // start

// Sets up the game state.
void goToGame() {
    state = GAME;
    
} // goToGame

// Runs every frame of the game state.
void game() {  

    updateGame();
    drawGame();

    if (gameWon) goToWin();
    if (gamePaused) goToPause();

} // game

// Sets up the pause state.
void goToPause() {
    state = PAUSE;

} // goToPause

// Runs every frame of the pause state.
void pause() {

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
    
    playSoundA(gamecomplete_data, gamecomplete_length, 0);
    state = WIN;

} // goToWin

// Runs every frame of the win state.
void win() {
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
    if (BUTTON_PRESSED(BUTTON_SELECT)) goToStart();

} // lose

void enableTimer() {
  
    REG_IME = 0;

    REG_IE |= INT_TM3;

    REG_TM3CNT = 0;
    REG_TM3D = 65536 - 16384;
    REG_TM3CNT = TM_FREQ_1024 | TIMER_ON | TM_IRQ;

    REG_IME = 1;

} // enableTimer

void disableTimer() {

    REG_IME = 0;
    REG_TM3CNT = TIMER_OFF;
    REG_IME = 1;

} // disableTimer

void setupInterrupts() {

	REG_IME = 0;

    REG_IE |= INT_VBLANK;
    REG_DISPSTAT |= INT_VBLANK_ENABLE;

    REG_INTERRUPT = interruptHandler;
	REG_IME = 1;

} // setupInterrupts

void interruptHandler() {

	REG_IME = 0;

	if(REG_IF & INT_VBLANK) {
        if (soundA.isPlaying) {

            soundA.vBlankCount = soundA.vBlankCount + 1;
            if (soundA.vBlankCount > soundA.duration) {
                if (soundA.loops) {
                    playSoundA(soundA.data, soundA.length, soundA.loops);
                } else {
                    soundA.isPlaying = 0;
                    dma[1].cnt = 0;
                    REG_TM0CNT = TIMER_OFF;
                }
            }
        }

        if (soundB.isPlaying) {

            soundB.vBlankCount = soundB.vBlankCount + 1;
            if (soundB.vBlankCount > soundB.duration) {
                if (soundB.loops) {
                    playSoundB(soundB.data, soundB.length, soundB.loops);
                } else {
                    soundB.isPlaying = 0;
                    dma[2].cnt = 0;
                    REG_TM1CNT = TIMER_OFF;
                }
            }
		}


		REG_IF = INT_VBLANK;

	} // if

    if (REG_IF & INT_TM3) {
        
        if (shadowOAM[127].attr0 == ATTR0_HIDE) {
            shadowOAM[126].attr0 = 80 | ATTR0_4BPP | ATTR0_WIDE;
            shadowOAM[127].attr0 = 80 | ATTR0_4BPP | ATTR0_WIDE;
            updateOAM();

        } else {
            shadowOAM[126].attr0 = ATTR0_HIDE;
            shadowOAM[127].attr0 = ATTR0_HIDE;
            updateOAM();

        } // if 

    } // if

    REG_IF = REG_IF;
	REG_IME = 1;

} // interruptHandler