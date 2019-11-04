#pragma once

#define STWO 1
#define STHREE 2
#define SFOUR 3
#define TWO 4
#define THREE 5
#define FOUR 6
#define FIVE 7
#define DFOUR 8
#define FOURT 9
#define DTHREE 10
#define NOTYPE 11
#define ANALSISED 255
#define TOBEANALSIS 0

#include "define.h"

// -----------------------------------------------
extern int PosValue[15][15];
// --------------------------------------------------

class CEveluation
{
public:
	CEveluation();
	virtual ~CEveluation();

	virtual int Eveluate(unsigned char position[][GRID_NUM], bool bIsRedTurn);

protected:
	int AnalysisHorizon(unsigned char position[][GRID_NUM], int i, int j);
	int AnalysisVertical(unsigned char position[][GRID_NUM], int i, int j);
	int AnalysisLeft(unsigned char position[][GRID_NUM], int i, int j);
	int AnalysisRight(unsigned char position[][GRID_NUM], int i, int j);
	int AnalysisLine(unsigned char *position, int GridNum, int StonePos);
	unsigned char m_LineRecord[30];
	int TypeRecord[GRID_NUM][GRID_NUM][4];
	int TypeCount[2][20];
};
