// draw.cpp  (Person 3: graphics)
// Uses globals from globals.h:
//   ROWS, COLS, CELL, TOP, SCREEN_W, BTN_X, BTN_Y, BTN_W, BTN_H
//   mines[][], counts[][], state[][]  (0 = covered, 1 = revealed, 2 = flagged)
//   gameOver, gameWon, minesLeft, elapsedTime

#include "raylib.h"
#include "globals.h"

static int tileX(int c) { return c * CELL; }
static int tileY(int r) { return TOP + r * CELL; }

// ---------------- TOP BAR ----------------

void drawMinesLeft()
{
    DrawText(TextFormat("Mines: %d", minesLeft), 10, TOP / 2 - 14, 18, WHITE);
}

void drawTimer()
{
    const char* t = TextFormat("Time: %d", (int)elapsedTime);
    DrawText(t, SCREEN_W - MeasureText(t, 18) - 10, TOP / 2 - 9, 18, WHITE);
}

void drawRestartButton()
{
    Rectangle r = { (float)BTN_X, (float)BTN_Y, (float)BTN_W, (float)BTN_H };
    bool hover = CheckCollisionPointRec(GetMousePosition(), r);

    DrawRectangleRec(r, hover ? WHITE : LIGHTGRAY);
    DrawRectangleLinesEx(r, 2, BLACK);

    int cx = BTN_X + BTN_W / 2;
    int cy = BTN_Y + BTN_H / 2;
    DrawCircle(cx, cy, 14, YELLOW);
    DrawCircleLines(cx, cy, 14, BLACK);

    // eyes
    if (gameWon)
    {
        DrawRectangle(cx - 10, cy - 6, 8, 5, BLACK);   // sunglasses
        DrawRectangle(cx + 2, cy - 6, 8, 5, BLACK);
    }
    else
    {
        DrawCircle(cx - 5, cy - 4, 2, BLACK);
        DrawCircle(cx + 5, cy - 4, 2, BLACK);
    }

    // mouth
    if (gameOver)
    {
        DrawLine(cx - 5, cy + 8, cx, cy + 5, BLACK);
        DrawLine(cx, cy + 5, cx + 5, cy + 8, BLACK);
    }
    else
    {
        DrawLine(cx - 5, cy + 5, cx, cy + 8, BLACK);
        DrawLine(cx, cy + 8, cx + 5, cy + 5, BLACK);
    }
}

void drawTopBar()
{
    DrawRectangle(0, 0, COLS * CELL, TOP, DARKGRAY);
    drawMinesLeft();
    drawTimer();
    drawRestartButton();

    // small instructions line
    DrawText("1/2/3 = difficulty, R = restart", 10, TOP - 16, 10, LIGHTGRAY);
}

// ---------------- TILES ----------------

// Covered or revealed colour and border
void drawTile(int r, int c)
{
    int x = tileX(c);
    int y = tileY(r);

    Color fill = LIGHTGRAY;                          // covered
    if (state[r][c] == 1) fill = WHITE;              // revealed
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

    DrawRectangle(x + CELL / 2 - 1, y + CELL / 5, 3, CELL * 3 / 5, BLACK);   // pole
    DrawRectangle(x + CELL / 4, y + CELL * 4 / 5 - 2, CELL / 2, 4, BLACK);   // base

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

// Red X over a flag placed on a tile that is not a mine
void drawWrongFlag(int r, int c)
{
    int x = tileX(c);
    int y = tileY(r);
    DrawLine(x + 5, y + 5, x + CELL - 5, y + CELL - 5, RED);
    DrawLine(x + CELL - 5, y + 5, x + 5, y + CELL - 5, RED);
    DrawLine(x + 6, y + 5, x + CELL - 4, y + CELL - 5, RED);
    DrawLine(x + CELL - 6, y + 5, x + 4, y + CELL - 5, RED);
}

// Light highlight on the covered tile under the mouse
void drawHover()
{
    if (gameOver || gameWon) return;

    Vector2 m = GetMousePosition();
    if (m.x < 0 || m.x >= COLS * CELL) return;
    if (m.y < TOP || m.y >= TOP + ROWS * CELL) return;

    int c = (int)(m.x / CELL);
    int r = (int)((m.y - TOP) / CELL);

    if (state[r][c] == 1) return;   // already revealed

    DrawRectangle(tileX(c), tileY(r), CELL, CELL, Fade(SKYBLUE, 0.45f));
}

// ---------------- WIN / LOSE OVERLAY ----------------

void drawGameOver()
{
    if (!gameOver && !gameWon) return;

    DrawRectangle(0, TOP, SCREEN_W, ROWS * CELL, Fade(BLACK, 0.6f));

    const char* msg = gameWon ? "YOU WIN!" : "GAME OVER";
    Color col = gameWon ? GREEN : RED;
    int cy = TOP + (ROWS * CELL) / 2;

    DrawText(msg, (SCREEN_W - MeasureText(msg, 36)) / 2, cy - 40, 36, col);

    const char* sub = "Press R or click the face to restart";
    DrawText(sub, (SCREEN_W - MeasureText(sub, 16)) / 2, cy + 5, 16, WHITE);

    if (gameWon)
    {
        const char* t = TextFormat("Time: %d seconds", (int)elapsedTime);
        DrawText(t, (SCREEN_W - MeasureText(t, 20)) / 2, cy + 35, 20, YELLOW);
    }
}

// ---------------- MAIN DRAW ----------------

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
                if (gameOver && !mines[r][c]) drawWrongFlag(r, c);
            }
            else if (state[r][c] == 1)
            {
                if (mines[r][c]) drawMine(r, c);
                else drawNumber(r, c);
            }
            else if (gameOver && mines[r][c])
            {
                drawMine(r, c);   // show remaining mines after a loss
            }
        }
    }

    drawHover();
    drawGameOver();
}