// #include <stdlib.h>
// #include <stdio.h>
#include "mygbalib.h"

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

// Shadow OAM.
OBJ_ATTR shadowOAM[128];

int main() {
    initialize();

    while (1) {
        // Update button variables.
        oldButtons = buttons;
        buttons = BUTTONS;

        // State Machine.
        switch (state) {
        case START:
            start();
            break;
        case GAME:
            game();
            break;
        case PAUSE:
            pause();
            break;
        case WIN:
            win();
            break;
        case LOSE:
            lose();
            break;
        }
    }
}

// Sets up GBA.
void initialize() {
    REG_DISPCTL = MODE0; // Bitwise OR the BG(s) you want to use and Bitwise OR SPRITE_ENABLE if you want to use sprites.
    // Don't forget to set up whatever BGs you enabled in the line above!

    buttons = BUTTONS;
    oldButtons = 0;

    goToStart();
}

// Sets up the start state.
void goToStart() {}

// Runs every frame of the start state.
void start() {}

// Sets up the game state.
void goToGame() {}

// Runs every frame of the game state.
void game() {}

// Sets up the pause state.
void goToPause() {}

// Runs every frame of the pause state.
void pause() {}

// Sets up the win state.
void goToWin() {}

// Runs every frame of the win state.
void win() {}

// Sets up the lose state.
void goToLose() {}

// Runs every frame of the lose state.
void lose() {}