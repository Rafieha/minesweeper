
// main.cpp - Person 1: window, main loop and mouse input
#include "raylib.h"
#include "globals.h"

// Converts the mouse position into a row and column.
// Returns true if the mouse is inside the grid, false if it is outside
// (for example on the top bar).
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

// Left click: reveal the tile if it is covered (not flagged, not revealed).
void handleLeftClick()
{
    int r, c;
    if (!getClickedTile(r, c)) return;

    if (state[r][c] == 0)       // 0 = covered
    {
        revealTile(r, c);       // Person 2's function
    }
}

// Right click: flag a covered tile, or remove the flag if it is already flagged.
void handleRightClick()
{
    int r, c;
    if (!getClickedTile(r, c)) return;

    if (state[r][c] == 0)        // covered -> flag it
        state[r][c] = 2;
    else if (state[r][c] == 2)   // flagged -> unflag it
        state[r][c] = 0;
    // revealed tiles (1) are left alone
}

int main()
{
    InitWindow(SCREEN_W, SCREEN_H, "Minesweeper");
    SetTargetFPS(60);

    // set up a new game
    initBoard();
    placeMines();
    countNeighbours();

    while (!WindowShouldClose())
    {
        // ---- input (clicks are ignored after the game is over) ----
        if (!gameOver)
        {
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))  handleLeftClick();
            if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) handleRightClick();
        }

        // ---- drawing ----
        BeginDrawing();
        ClearBackground(RAYWHITE);
        drawBoard();             // Person 3's function
        EndDrawing();
    }

    CloseWindow();
    return 0;
}