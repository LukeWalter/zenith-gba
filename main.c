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
#include "caveexit.h"
#include "instructions.h"

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
void goToControls();
void controls();

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
    CONTROL
};
int state;

// Button Variables.
unsigned short buttons;
unsigned short oldButtons;

int pauseSelected;

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
            
            case CONTROL: {
                controls();
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
    stopSound();
    playSoundA(titlemusic_data, titlemusic_length, 1);
    state = START;

} // goToStart

// Runs every frame of the start state.
void start() {
    
    if (BUTTON_PRESSED(BUTTON_START)) {

        disableTimer();
        stopSound();
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
    
    shadowOAM[0].attr0 = 14 | ATTR0_4BPP | ATTR0_WIDE;
    shadowOAM[0].attr1 = 14 | ATTR1_MEDIUM;

    shadowOAM[3].attr0 = 14 | ATTR0_4BPP | ATTR0_WIDE;
    shadowOAM[3].attr1 = 46 | ATTR1_MEDIUM;

    shadowOAM[1].attr0 = 34 | ATTR0_4BPP | ATTR0_WIDE;
    shadowOAM[1].attr1 = 14 | ATTR1_MEDIUM;

    shadowOAM[4].attr0 = 34 | ATTR0_4BPP | ATTR0_WIDE;
    shadowOAM[4].attr1 = 46 | ATTR1_MEDIUM;

    shadowOAM[2].attr0 = 54 | ATTR0_4BPP | ATTR0_WIDE;
    shadowOAM[2].attr1 = 14 | ATTR1_MEDIUM;

    shadowOAM[5].attr0 = 54 | ATTR0_4BPP | ATTR0_WIDE;
    shadowOAM[5].attr1 = 46 | ATTR1_MEDIUM;

    shadowOAM[6].attr0 = 10 | ATTR0_4BPP | ATTR0_SQUARE;
    shadowOAM[6].attr1 = 10 | ATTR1_LARGE;
    shadowOAM[6].attr2 = ATTR2_TILEID(0, 24);
    
    pauseSelected = 1;
    state = PAUSE;

} // goToPause

// Runs every frame of the pause state.
void pause() {

    shadowOAM[0].attr2 = ATTR2_TILEID(8, 20);
    shadowOAM[3].attr2 = ATTR2_TILEID(12, 20);
    shadowOAM[1].attr2 = ATTR2_TILEID(16, 16);
    shadowOAM[4].attr2 = ATTR2_TILEID(20, 16);
    shadowOAM[2].attr2 = ATTR2_TILEID(8, 16);
    shadowOAM[5].attr2 = ATTR2_TILEID(12, 16);

    switch (pauseSelected) {
    case 1: 
        
        shadowOAM[0].attr2 = ATTR2_TILEID(8, 22);
        shadowOAM[3].attr2 = ATTR2_TILEID(12, 22);

        if (BUTTON_PRESSED(BUTTON_START)) {

            for (int i = 0; i < 7; i++) {
                shadowOAM[i].attr0 = ATTR0_HIDE;
            
            } // for
            
            goToGame();
        
        } else if (BUTTON_PRESSED(BUTTON_UP)) {
            pauseSelected = 3;

        } else if (BUTTON_PRESSED(BUTTON_DOWN)) {
            pauseSelected = 2;

        } // if
        
        break;

    case 2: 

        shadowOAM[1].attr2 = ATTR2_TILEID(16, 18);
        shadowOAM[4].attr2 = ATTR2_TILEID(20, 18);

        if (BUTTON_PRESSED(BUTTON_START)) {

            for (int i = 0; i < 7; i++) {
                shadowOAM[i].attr0 = ATTR0_HIDE;
            
            } // for
            
            goToControls();
        
        } else if (BUTTON_PRESSED(BUTTON_UP)) {
            pauseSelected = 1;

        } else if (BUTTON_PRESSED(BUTTON_DOWN)) {
            pauseSelected = 3;

        } // if
        
        break;

    case 3:

        shadowOAM[2].attr2 = ATTR2_TILEID(8, 18);
        shadowOAM[5].attr2 = ATTR2_TILEID(12, 18);

        if (BUTTON_PRESSED(BUTTON_START)) {

            for (int i = 0; i < 7; i++) {
                shadowOAM[i].attr0 = ATTR0_HIDE;
            
            } // for
            
            goToStart();
        
        } else if (BUTTON_PRESSED(BUTTON_UP)) {
            pauseSelected = 2;

        } else if (BUTTON_PRESSED(BUTTON_DOWN)) {
            pauseSelected = 1;

        } // if
        
        break;

    default:
        break;

    } // switch

    waitForVBlank();
    updateOAM();

} // pause

// Sets up the win state.
void goToWin() {
    
    DMANow(3, caveexitPal, PALETTE, 256);
    DMANow(3, caveexitTiles, &CHARBLOCK[0], caveexitTilesLen / 2);
    DMANow(3, caveexitMap, &SCREENBLOCK[28], caveexitMapLen / 2);

    hideSprites();

    waitForVBlank();
    updateOAM();
    
    stopSound();
    playSoundB(gamecomplete_data, gamecomplete_length, 0);
    state = WIN;

} // goToWin

// Runs every frame of the win state.
void win() {
    if (BUTTON_PRESSED(BUTTON_START)) goToStart();

} // win

// Sets up the lose state.
void goToControls() {

    DMANow(3, instructionsPal, PALETTE, 256);
    DMANow(3, instructionsTiles, &CHARBLOCK[0], instructionsTilesLen / 2);
    DMANow(3, instructionsMap, &SCREENBLOCK[28], instructionsMapLen / 2);

    hideSprites();

    waitForVBlank();
    updateOAM();

    state = CONTROL;
     
} // goToLose

// Runs every frame of the lose state.
void controls() {
    
    if (BUTTON_PRESSED(BUTTON_START)) {

        DMANow(3, levels[level - 1].pal, PALETTE, 256);
        DMANow(3, levels[level - 1].tiles, &CHARBLOCK[0], levels[level - 1].tileLen / 2);
        DMANow(3, levels[level - 1].map, &SCREENBLOCK[28], levels[level - 1].mapLen / 2);
        REG_BG1VOFF = vOff;
        REG_BG1HOFF = hOff;

        for (int c = 0; c < levels[level - 1].mapWidth; c++) {

            for (int r = 0; r < levels[level - 1].mapHeight; r++) {
            
                int tileposition = levels[level - 1].mapTiles[OFFSET(c, r, mapWidth)];
                drawTile(c * 2, r * 2, tileposition);

            }  // for

        } // for

        goToGame();
    
    } // if

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