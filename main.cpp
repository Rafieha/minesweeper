#include "raylib.h"
#include "globals.h"
#include <cstdlib>
#include <ctime>

bool firstClick = true;       // true until the player makes the first reveal
int  numMines = NUM_MINES;    // current number of mines (changed by difficulty)

// Converts the mouse position into a row and column
bool getClickedTile(int& row, int& col)
{
    int x = GetMouseX();
    int y = GetMouseY();

    // ignore anything outside the grid
    if (x < 0 || x >= COLS * CELL) return false;
    if (y < TOP || y >= TOP + ROWS * CELL) return false;

    col = x / CELL;
    row = (y - TOP) / CELL;
    return true;
}
// Returns true if the mouse is over the restart button in the top bar
bool clickedRestartButton()
{
    int x = GetMouseX();
    int y = GetMouseY();

    return x >= BTN_X && x < BTN_X + BTN_W &&
           y >= BTN_Y && y < BTN_Y + BTN_H;
}

// Starts a fresh game. Mines are placed on the first click, so the first tile is always safe 
void resetGame()
{
    initBoard();          // clears arrays, gameOver and gameWon (logic.cpp)
    firstClick = true;
}

// Changes the number of mines and starts a new game
void setDifficulty(int mines)
{
    numMines = mines;
    resetGame();
}

void handleRightClick()
{
    int r, c;
    if (!getClickedTile(r, c)) return;
    toggleFlag(r, c);
}
void handleLeftClick()
{
    int r, c;
    if (!getClickedTile(r, c)) return;

    if (state[r][c] == 1) chordReveal(r, c);
    else                  revealCell(r, c);
}

int main()
{
    InitWindow(SCREEN_W, SCREEN_H, "Minesweeper");
    SetTargetFPS(60);
     srand((unsigned)time(NULL));      // different mine layout every run

    resetGame();

    while (!WindowShouldClose())
    {
        // keys that work at any time
        if (IsKeyPressed(KEY_R))     resetGame();
        if (IsKeyPressed(KEY_ONE))   setDifficulty(10);   // easy
        if (IsKeyPressed(KEY_TWO))   setDifficulty(15);   // medium
        if (IsKeyPressed(KEY_THREE)) setDifficulty(20);   // hard

        // restart button in the top bar
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && clickedRestartButton())
            resetGame();

        // board clicks are ignored once the game is won or lost
        if (!gameOver && !gameWon)
        {
            updateTimer();
           if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))  handleLeftClick();
            if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) handleRightClick();
        }

        // drawing
        BeginDrawing();
        ClearBackground(RAYWHITE);
        drawBoard();           
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
