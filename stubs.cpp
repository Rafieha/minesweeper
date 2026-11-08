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

void placeMines()
{
    srand((unsigned)time(NULL));
    for (int i = 0; i < 10; i++)
        mines[rand() % ROWS][rand() % COLS] = 1;
}

void countNeighbours() {}
void revealAllMines() {}

void revealTile(int r, int c)
{
    state[r][c] = 1;
    if (mines[r][c] == 1) gameOver = true;
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