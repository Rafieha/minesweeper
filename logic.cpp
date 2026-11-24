#include "globals.h"
#include <cstdlib>
#include <ctime>

// Global variables (ROWS, COLS, NUM_MINES come from globals.h)
int mines[ROWS][COLS];        // true = mine
int counts[ROWS][COLS];
int state[ROWS][COLS];         // 0 = covered, 1 = revealed, 2 = flagged
bool gameOver = false;
bool gameWon = false;
int flagsPlaced = 0;
int minesLeft = NUM_MINES;
bool timerRunning = false;
time_t startTime = 0;
int elapsedTime = 0;

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
    gameWon = false;
}

// Function 3

void countNeighbours()
{
    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c < COLS; c++)
        {
            int n = 0;
            for (int dr = -1; dr <= 1; dr++)
                for (int dc = -1; dc <= 1; dc++) {
                    if (dr == 0 && dc == 0) continue;
                    int nr = r + dr, nc = c + dc;
                    if (nr >= 0 && nr < ROWS && nc >= 0 && nc < COLS && mines[nr][nc])
                        n++;
                }
            counts[r][c] = n;
        }
}
// Function 4
void revealTile(int r, int c)
{
    if (r < 0 || r >= ROWS || c < 0 || c >= COLS) return;
    if (state[r][c] != 0) return;

    state[r][c] = 1;

    if (mines[r][c]) {
        gameOver = true;
        revealAllMines();
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
// ----- Timer -----
void startTimer()
{
    startTime = time(0);
    timerRunning = true;
}

void updateTimer()          // call once per frame in main loop
{
    if (timerRunning)
        elapsedTime = (int)(time(0) - startTime);
}

void stopTimer()
{
    updateTimer();
    timerRunning = false;
}

// ----- New game (calls your initBoard and resets the new variables) -----
void newGame()
{
    initBoard();
    firstClick = true;
    flagsPlaced = 0;
    minesLeft = NUM_MINES;
    timerRunning = false;
    elapsedTime = 0;
}

// ----- Safe first click -----
void placeMines(int safeR, int safeC)
{
    int placed = 0;
    while (placed < NUM_MINES) {
        int r = rand() % ROWS;
        int c = rand() % COLS;

        bool nearFirstClick = (r >= safeR - 1 && r <= safeR + 1 &&
                               c >= safeC - 1 && c <= safeC + 1);

        if (!mines[r][c] && !nearFirstClick) {
            mines[r][c] = 1;
            placed++;
        }
    }
}

// ----- Flags and mines left -----
void toggleFlag(int r, int c)
{
    if (gameOver || gameWon) return;
    if (r < 0 || r >= ROWS || c < 0 || c >= COLS) return;

    if (state[r][c] == 0) {
        state[r][c] = 2;
        flagsPlaced++;
    }
    else if (state[r][c] == 2) {
        state[r][c] = 0;
        flagsPlaced--;
    }
    minesLeft = NUM_MINES - flagsPlaced;
}

// ----- Win check -----
void checkWin()
{
    if (gameOver || gameWon) return;

    int revealed = 0;
    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c < COLS; c++)
            if (state[r][c] == 1)
                revealed++;

    if (revealed == ROWS * COLS - NUM_MINES) {
        gameWon = true;
        stopTimer();

        for (int r = 0; r < ROWS; r++)
            for (int c = 0; c < COLS; c++)
                if (mines[r][c]) state[r][c] = 2;
        flagsPlaced = NUM_MINES;
        minesLeft = 0;
    }
}

// ----- Flood-fill (recursion) -----
static void floodReveal(int r, int c)
{
    if (r < 0 || r >= ROWS || c < 0 || c >= COLS) return;
    if (state[r][c] != 0) return;

    state[r][c] = 1;

    if (counts[r][c] == 0) {
        for (int dr = -1; dr <= 1; dr++)
            for (int dc = -1; dc <= 1; dc++)
                if (dr != 0 || dc != 0)
                    floodReveal(r + dr, c + dc);
    }
}

// ----- Main reveal (use this on left click instead of revealTile) -----
void revealCell(int r, int c)
{
    if (gameOver || gameWon) return;
    if (r < 0 || r >= ROWS || c < 0 || c >= COLS) return;
    if (state[r][c] != 0) return;

    if (firstClick) {
        placeMines(r, c);
        countNeighbours();
        startTimer();
        firstClick = false;
    }

    if (mines[r][c]) {
        state[r][c] = 1;
        gameOver = true;
        revealAllMines();
        stopTimer();
        return;
    }

    floodReveal(r, c);
    checkWin();
}

// ----- Chord reveal -----
void chordReveal(int r, int c)
{
    if (gameOver || gameWon) return;
    if (r < 0 || r >= ROWS || c < 0 || c >= COLS) return;
    if (state[r][c] != 1 || counts[r][c] == 0) return;

    int flags = 0;
    for (int dr = -1; dr <= 1; dr++)
        for (int dc = -1; dc <= 1; dc++) {
            int nr = r + dr, nc = c + dc;
            if (nr >= 0 && nr < ROWS && nc >= 0 && nc < COLS && state[nr][nc] == 2)
                flags++;
        }

    if (flags != counts[r][c]) return;

    for (int dr = -1; dr <= 1; dr++)
        for (int dc = -1; dc <= 1; dc++) {
            int nr = r + dr, nc = c + dc;
            if (nr >= 0 && nr < ROWS && nc >= 0 && nc < COLS && state[nr][nc] == 0)
                revealCell(nr, nc);
        }
}
