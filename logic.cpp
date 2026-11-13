#include "globals.h"
#include <cstdlib>
#include <ctime>

// Global variables
const int ROWS = 9;
const int COLS = 9;
const int NUM_MINES = 10;      // define this once, at the top with ROWS and COLS

bool mines[ROWS][COLS];        // true = mine

int counts[ROWS][COLS];
int state[ROWS][COLS];
bool gameOver = false;

// Function 1
void initBoard()
{
    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c < COLS; c++)
        {
            mines[r][c] = 0;
            counts[r][c] = 0;
            state[r][c] = 0;
        }
    gameOver = false;
}
void placeMines()
{
    int placed = 0;
    while (placed < NUM_MINES) {
        int r = rand() % ROWS;
        int c = rand() % COLS;
        if (!mines[r][c]) {    // only place if there is no mine here yet
            mines[r][c] = true;
            placed++;
        }
    }
}

// Function 3
void countNeighbours()
{
    int n = 0;
    for (int dr = -1; dr <= 1; dr++)
        for (int dc = -1; dc <= 1; dc++) {
            if (dr == 0 && dc == 0) continue;
            int nr = r + dr, nc = c + dc;
            if (nr >= 0 && nr < ROWS && nc >= 0 && nc < COLS && mines[nr][nc])
                n++;
        }
    return n;
}

// Function 4
void revealTile(int r, int c)
{
    if (r < 0 || r >= ROWS || c < 0 || c >= COLS) return;
    if (state[r][c] != 0) return;

    state[r][c] = 1;

    if (mines[r][c]) {
        gameOver = true;
        return;
    }
}

// Function 5
void revealAllMines()
{
    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c < COLS; c++)
            if (mines[r][c]) state[r][c] = 1;
}

