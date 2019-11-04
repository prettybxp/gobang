#pragma once

#define BOARD_POS_X 60
#define BOARD_POS_Y 30
#define BOARD_WIDTH 34
#define GRID_NUM 15
#define GRID_COUNT 225

#define BLACK 0
#define WHITE 1

#define NOSTONE 0xFF
#define IsValidPos(x, y) ((x >= 0 && x < GRID_NUM) && (y >= 0 && y < GRID_NUM))

//-------------------------------------------------
typedef struct _stoneposition
{
  char x;
  char y;
} STONEPOS;

typedef struct _stonemove
{
  STONEPOS StonePos;
  int Score;
} STONEMOVE;
//-----------------------------------------------
