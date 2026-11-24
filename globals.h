#pragma once
//Constants 
const int ROWS = 9;
const int COLS = 9;
const int CELL = 50;   // size of one tile in pixels
const int TOP = 60;// height of the top bar in pixels
const int NUM_MINES = 10;

const int SCREEN_W = COLS * CELL;
const int SCREEN_H = TOP + ROWS * CELL;

const int BTN_X = SCREEN_W / 2 - 20;
const int BTN_Y = 10;
const int BTN_W = 40;
const int BTN_H = 40;

extern int  mines[ROWS][COLS];   
extern int  counts[ROWS][COLS];  // number of mines around the tile
extern int  state[ROWS][COLS];   
extern bool gameOver;
extern bool gameWon;
extern int   minesLeft;          // mines minus flags placed
extern float elapsedTime;        // seconds since the first click

// Shared data defined in main.cpp
extern bool firstClick;          // true until the first reveal
extern int  numMines;          

// game logic (logic.cpp)
void initBoard();
void placeMines(int safeR, int safeC);
void countNeighbours();
void revealTile(int r, int c);   
void revealAllMines();
void checkWin();                         
void updateTimer(); 

//drawing (draw.cpp) 
void drawBoard();               
// setup and input (main.cpp)
bool getClickedTile(int& row, int& col);
bool clickedRestartButton();
void handleLeftClick();
void handleRightClick();
void resetGame();
void setDifficulty(int mines);
void startTimer();
void stopTimer();
void newGame();
void toggleFlag(int r, int c);
void revealCell(int r, int c);
void chordReveal(int r, int c); 
