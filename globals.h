
#ifndef GLOBALS_H
#define GLOBALS_H

//Constants 
const int ROWS = 9;
const int COLS = 9;
const int CELL = 50;   // size of one tile in pixels
const int TOP = 60;// height of the top bar in pixels
const int NUM_MINES = 10;

const int SCREEN_W = COLS * CELL;
const int SCREEN_H = TOP + ROWS * CELL;

//Shared data defined in logic.cpp
extern int  mines[ROWS][COLS];   // 1 = mine, 0 = no mine
extern int  counts[ROWS][COLS];  // number of mines around the tile
extern int  state[ROWS][COLS];   // 0 = covered, 1 = revealed, 2 = flagged
extern bool gameOver;

// game logic (logic.cpp)
void initBoard();
void placeMines();
void countNeighbours();
void revealTile(int r, int c);   // sets gameOver = true if it is a mine
void revealAllMines();

//drawing (draw.cpp) 
void drawBoard();                // draws top bar, tiles, numbers, flags, game over

// setup and input (main.cpp)
bool getClickedTile(int& row, int& col);
void handleLeftClick();
void handleRightClick();

#endif#pragma once
