#include "game.h"

typedef struct {
    int col;
    int row;

} COORDINATE;

// Level 1

COORDINATE zenithLoc = {0, 0};

COORDINATE blockLocs[BLOCKCOUNT] = {
    {3, 3}

};

COORDINATE plateLocs[PLATECOUNT] = {
    {4, 4}

};

// Level 2

COORDINATE zenithLoc2 = {1, 1};

COORDINATE blockLocs2[BLOCKCOUNT] = {
    {4, 4}

};

COORDINATE plateLocs2[PLATECOUNT] = {
    {5, 5}

};