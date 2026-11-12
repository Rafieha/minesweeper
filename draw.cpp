// draw.cpp - Person 3: Drawing
// Uses shared globals from globals.h:
//   ROWS, COLS, CELL, TOP
//   mines[][], counts[][], state[][]  (0 = covered, 1 = revealed, 2 = flagged)
//   gameOver

#include "raylib.h"
#include "globals.h"

static int tileX(int c) { return c * CELL; }
static int tileY(int r) { return TOP + r * CELL; }

// Reveal every mine (called on game over)
void revealAllMines()
{
    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c < COLS; c++)
            if (mines[r][c]) state[r][c] = 1;
}

// Title text
void drawTopBar()
{
    DrawRectangle(0, 0, COLS * CELL, TOP, DARKGRAY);
    DrawText("MINESWEEPER", 10, TOP / 2 - 12, 24, WHITE);
}

// Covered or revealed colour and border
void drawTile(int r, int c)
{
    int x = tileX(c);
    int y = tileY(r);

    Color fill = LIGHTGRAY;                                  // covered
    if (state[r][c] == 1) fill = WHITE;                      // revealed
    if (state[r][c] == 1 && mines[r][c]) fill = RED; // mine

    DrawRectangle(x, y, CELL, CELL, fill);
    DrawRectangleLines(x, y, CELL, CELL, GRAY);

    // raised edge on covered/flagged tiles
    if (state[r][c] != 1)
    {
        DrawLine(x, y, x + CELL - 1, y, RAYWHITE);
        DrawLine(x, y, x, y + CELL - 1, RAYWHITE);
    }
}

// Number on a tile, different colour for each value
void drawNumber(int r, int c)
{
    int n = counts[r][c];
    if (n <= 0 || n > 8) return;

    Color colors[9] = {
        BLACK, BLUE, DARKGREEN, RED, DARKBLUE,
        MAROON, SKYBLUE, BLACK, GRAY
    };

    char text[2] = { (char)('0' + n), '\0' };
    int fontSize = CELL * 2 / 3;
    int textW = MeasureText(text, fontSize);

    int x = tileX(c) + (CELL - textW) / 2;
    int y = tileY(r) + (CELL - fontSize) / 2;
    DrawText(text, x, y, fontSize, colors[n]);
}

void drawFlag(int r, int c)
{
    int x = tileX(c);
    int y = tileY(r);

    DrawRectangle(x + CELL / 2 - 1, y + CELL / 5, 3, CELL * 3 / 5, BLACK); // pole
    DrawRectangle(x + CELL / 4, y + CELL * 4 / 5 - 2, CELL / 2, 4, BLACK); // base

    Vector2 a = { (float)(x + CELL / 2), (float)(y + CELL / 5) };
    Vector2 b = { (float)(x + CELL / 2), (float)(y + CELL * 3 / 5) };
    Vector2 d = { (float)(x + CELL / 5), (float)(y + CELL * 2 / 5) };
    DrawTriangle(a, d, b, ORANGE);
}

void drawMine(int r, int c)
{
    int cx = tileX(c) + CELL / 2;
    int cy = tileY(r) + CELL / 2;
    int rad = CELL / 4;

    DrawCircle(cx, cy, rad, BLACK);
    DrawLine(cx - rad - 4, cy, cx + rad + 4, cy, BLACK);
    DrawLine(cx, cy - rad - 4, cx, cy + rad + 4, BLACK);
    DrawLine(cx - rad - 2, cy - rad - 2, cx + rad + 2, cy + rad + 2, BLACK);
    DrawLine(cx - rad - 2, cy + rad + 2, cx + rad + 2, cy - rad - 2, BLACK);
    DrawCircle(cx - rad / 3, cy - rad / 3, rad / 4, WHITE); // shine
}

// "Game Over" message
void drawGameOver()
{
    DrawText("Game Over", 10, TOP / 2 - 15, 30, RED);
}

// Loops through the grid and calls the functions above
void drawBoard()
{
    drawTopBar();

    for (int r = 0; r < ROWS; r++)
    {
        for (int c = 0; c < COLS; c++)
        {
            drawTile(r, c);

            if (state[r][c] == 2)
            {
                drawFlag(r, c);
            }
            else if (state[r][c] == 1)
            {
                if (mines[r][c]) drawMine(r, c);
                else drawNumber(r, c);
            }
        }
    }

    if (gameOver) drawGameOver();
}