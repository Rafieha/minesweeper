// stubs.cpp - TEMPORARY, only so Part 1 can be tested alone.
// Remove this file from the project once Person 2's logic.cpp and
// Person 3's draw.cpp are added (they define all of these for real).
#include "raylib.h"
#include "globals.h"
#include <cstdlib>
#include <ctime>

int  mines[ROWS][COLS];
int  counts[ROWS][COLS];
int  state[ROWS][COLS];
bool gameOver = false;

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

void placeMines() {
    int placed = 0;
    while (placed < NUM_MINES) {
        int r = rand() % ROWS;
        int c = rand() % COLS;
        if (!mines[r][c]) {         
            mines[r][c] = true;
            placed++;
        }
    }

int countNeighbours(int r, int c) {
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
void revealAllMines() {
    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c < COLS; c++)
            if (mines[r][c]) state[r][c] = 1;
}

void revealTile(int r, int c) {
    if (r < 0 || r >= ROWS || c < 0 || c >= COLS) return;  
    if (state[r][c] != 0) return;                         

    state[r][c] = 1;

    if (mines[r][c]) {
        gameOver = true;
        return;
    }
void drawBoard()
{
    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c < COLS; c++)
        {
            Color col = LIGHTGRAY;                 // covered
            if (state[r][c] == 1) col = WHITE;     // revealed
            if (state[r][c] == 2) col = ORANGE;    // flagged
            if (state[r][c] == 1 && mines[r][c]) col = RED;
            DrawRectangle(c * CELL, TOP + r * CELL, CELL - 2, CELL - 2, col);
        }
    if (gameOver) DrawText("Game Over", 10, 15, 30, RED);
}
