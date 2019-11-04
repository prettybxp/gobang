
#include "Eveluation.h"
#include "math.h"
#include <string.h>
#include "spdlog/spdlog.h"
#include "spdlog/fmt/bin_to_hex.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// ---------------------------------------------------------
int PosValue[GRID_NUM][GRID_NUM] =
	{
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
		{0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
		{0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0},
		{0, 1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 1, 0},
		{0, 1, 2, 3, 4, 4, 4, 4, 4, 4, 4, 3, 2, 1, 0},
		{0, 1, 2, 3, 4, 5, 5, 5, 5, 5, 4, 3, 2, 1, 0},
		{0, 1, 2, 3, 4, 5, 6, 6, 6, 5, 4, 3, 2, 1, 0},
		{0, 1, 2, 3, 4, 5, 6, 7, 6, 5, 4, 3, 2, 1, 0},
		{0, 1, 2, 3, 4, 5, 6, 6, 6, 5, 4, 3, 2, 1, 0},
		{0, 1, 2, 3, 4, 5, 5, 5, 5, 5, 4, 3, 2, 1, 0},
		{0, 1, 2, 3, 4, 4, 4, 4, 4, 4, 4, 3, 2, 1, 0},
		{0, 1, 2, 3, 3, 3, 3, 3, 3, 3, 3, 3, 2, 1, 0},
		{0, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 1, 0},
		{0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
		{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
// ----------------------------------------------------------

CEveluation::CEveluation()
{
}

CEveluation::~CEveluation()
{
}
// ---------------------------------------------------------------------------
//黑棋落的子，为黑棋评分
//白棋落的子，为白棋评分
int count = 0;
int CEveluation::Eveluate(unsigned char position[][GRID_NUM], bool bIsBlackTurn)
{
	int i, j, k;
	unsigned char nStoneType;
	count++;
	memset(TypeRecord, TOBEANALSIS, GRID_COUNT * 4 * 4);
	memset(TypeCount, 0, 40 * 4);
	for (i = 0; i < GRID_NUM; i++)
	{
		for (j = 0; j < GRID_NUM; j++)
		{
			if (position[i][j] != NOSTONE)
			{
				if (TypeRecord[i][j][0] == TOBEANALSIS)
					AnalysisHorizon(position, i, j);
				if (TypeRecord[i][j][1] == TOBEANALSIS)
					AnalysisVertical(position, i, j);
				if (TypeRecord[i][j][2] == TOBEANALSIS)
					AnalysisLeft(position, i, j);
				if (TypeRecord[i][j][3] == TOBEANALSIS)
					AnalysisRight(position, i, j);
			}
		}
	}

	for (i = 0; i < GRID_NUM; i++)
	{
		for (j = 0; j < GRID_NUM; j++)
		{
			for (k = 0; k < 4; k++)
			{
				nStoneType = position[i][j];
				if (nStoneType != NOSTONE)
				{
					//spdlog::info("pos({},{}), stone {} , direction {} result {}", i, j, (int)position[i][j], k, TypeRecord[i][j][k]);
					switch (TypeRecord[i][j][k])
					{
					case FIVE:
						TypeCount[nStoneType][FIVE]++;
						break;
					case FOUR:
						TypeCount[nStoneType][FOUR]++;
						break;
					case SFOUR:
						TypeCount[nStoneType][SFOUR]++;
						break;
					case THREE:
						TypeCount[nStoneType][THREE]++;
						break;
					case STHREE:
						TypeCount[nStoneType][STHREE]++;
						break;
					case TWO:
						TypeCount[nStoneType][TWO]++;
						break;
					case STWO:
						TypeCount[nStoneType][STWO]++;
						break;
					default:
						break;
					}
				}
			}
		}
	}

	if (bIsBlackTurn)
	{
		if (TypeCount[BLACK][FIVE])
			return 9999;
		if (TypeCount[WHITE][FIVE])
			return -9999;
	}
	else
	{
		if (TypeCount[BLACK][FIVE])
			return -9999;
		if (TypeCount[WHITE][FIVE])
			return +9999;
	}
	if (TypeCount[WHITE][SFOUR] > 1)
		TypeCount[WHITE][FOUR]++;

	if (TypeCount[BLACK][SFOUR] > 1)
		TypeCount[BLACK][FOUR]++;

	int WValue = 0, BValue = 0;
	if (bIsBlackTurn)
	{
		// 黑棋落的子，下一步是白棋

		//白棋活四，必输
		if (TypeCount[WHITE][FOUR])
		{
			return -9990;
		}

		//白棋有四活，也必输
		if (TypeCount[WHITE][SFOUR])
		{
			return -9980;
		}

		//白棋没有活四和死死，我方有活四，比胜
		if (TypeCount[BLACK][FOUR])
		{
			return 9970;
		}

		//黑棋有活三和冲四，比胜
		if (TypeCount[BLACK][SFOUR] && TypeCount[BLACK][THREE])
		{
			return 9960;
		}

		//白棋有活三，我方没有冲死，比输
		if (TypeCount[WHITE][THREE] && TypeCount[BLACK][SFOUR] == 0)
		{
			return -9950;
		}

		//我方有双活三，对方没有死四或，活三和死三，比胜
		if (TypeCount[BLACK][THREE] > 1 &&
			TypeCount[WHITE][SFOUR] == 0 &&
			TypeCount[WHITE][THREE] == 0 &&
			TypeCount[WHITE][STHREE] == 0)
		{
			return 9940;
		}

		if (TypeCount[WHITE][THREE] > 1)
			WValue += 2000;
		else
		{
			if (TypeCount[WHITE][THREE])
				WValue += 200;
		}

		if (TypeCount[BLACK][THREE] > 1)
			BValue += 500;
		else
		{
			if (TypeCount[BLACK][THREE])
				BValue += 100;
		}

		if (TypeCount[WHITE][STHREE])
			WValue += TypeCount[WHITE][STHREE] * 10;

		if (TypeCount[BLACK][STHREE])
			BValue += TypeCount[BLACK][STHREE] * 10;

		if (TypeCount[WHITE][TWO])
			WValue += TypeCount[WHITE][TWO] * 4;

		if (TypeCount[BLACK][STWO])
			BValue += TypeCount[BLACK][TWO] * 4;

		if (TypeCount[WHITE][STWO])
			WValue += TypeCount[WHITE][STWO];

		if (TypeCount[BLACK][STWO])
			BValue += TypeCount[BLACK][STWO];
	}
	else
	{
		if (TypeCount[BLACK][FOUR])
		{
			return -9990;
		}

		if (TypeCount[BLACK][SFOUR])
		{
			return -9980;
		}

		if (TypeCount[WHITE][FOUR])
		{
			return 9970;
		}

		if (TypeCount[WHITE][SFOUR] && TypeCount[WHITE][THREE])
		{
			return 9960;
		}

		if (TypeCount[BLACK][THREE] && TypeCount[WHITE][SFOUR] == 0)
		{
			return -9950;
		}

		if (TypeCount[WHITE][THREE] > 1 &&
			TypeCount[BLACK][SFOUR] == 0 &&
			TypeCount[BLACK][THREE] == 0 &&
			TypeCount[BLACK][STHREE] == 0)
		{
			return 9940;
		}

		if (TypeCount[BLACK][THREE] > 1)
			BValue += 2000;
		else
		{
			if (TypeCount[BLACK][THREE])
				BValue += 200;
		}

		if (TypeCount[WHITE][THREE] > 1)
			WValue += 500;
		else
		{
			if (TypeCount[WHITE][THREE])
				WValue += 100;
		}

		if (TypeCount[WHITE][STHREE])
			WValue += TypeCount[WHITE][STHREE] * 10;

		if (TypeCount[BLACK][STHREE])
			BValue += TypeCount[BLACK][STHREE] * 10;

		if (TypeCount[WHITE][TWO])
			WValue += TypeCount[WHITE][TWO] * 4;

		if (TypeCount[BLACK][STWO])
			BValue += TypeCount[BLACK][TWO] * 4;

		if (TypeCount[WHITE][STWO])
			WValue += TypeCount[WHITE][STWO];

		if (TypeCount[BLACK][STWO])
			BValue += TypeCount[BLACK][STWO];
	}

	for (i = 0; i < GRID_NUM; i++)
		for (j = 0; j < GRID_NUM; j++)
		{
			nStoneType = position[i][j];
			if (nStoneType != NOSTONE)
			{
				if (nStoneType == BLACK)
					BValue += PosValue[i][j];
				else
					WValue += PosValue[i][j];
			}
		}

	if (bIsBlackTurn)
		return BValue - WValue;
	else
		return WValue - BValue;
}

int CEveluation::AnalysisHorizon(unsigned char position[][GRID_NUM], int i, int j)
{

	AnalysisLine(position[i], 15, j);
	for (int s = 0; s < 15; s++)
	{
		if (m_LineRecord[s] != TOBEANALSIS)
			TypeRecord[i][s][0] = m_LineRecord[s];
	}
	return TypeRecord[i][j][0];
}

int CEveluation::AnalysisVertical(unsigned char position[][GRID_NUM], int i, int j)
{
	unsigned char tempArray[GRID_NUM];

	for (int k = 0; k < GRID_NUM; k++)
		tempArray[k] = position[k][j];
	AnalysisLine(tempArray, GRID_NUM, i);
	for (int s = 0; s < GRID_NUM; s++)
	{
		if (m_LineRecord[s] != TOBEANALSIS)
			TypeRecord[s][j][1] = m_LineRecord[s];
	}
	return TypeRecord[i][j][1];
}

int CEveluation::AnalysisLeft(unsigned char position[][GRID_NUM], int i, int j)
{
	unsigned char tempArray[GRID_NUM];
	int x, y;
	if (i < j)
	{
		y = 0;
		x = j - i;
	}
	else
	{
		x = 0;
		y = i - j;
	}
	int k;
	for (k = 0; k < GRID_NUM; k++)
	{
		if (x + k > 14 || y + k > 14)
			break;
		tempArray[k] = position[y + k][x + k];
	}
	AnalysisLine(tempArray, k, j - x);
	for (int s = 0; s < k; s++)
	{
		if (m_LineRecord[s] != TOBEANALSIS)
			TypeRecord[y + s][x + s][2] = m_LineRecord[s];
	}
	return TypeRecord[i][j][2];
}

int CEveluation::AnalysisRight(unsigned char position[][GRID_NUM], int i, int j)
{
	unsigned char tempArray[GRID_NUM];
	int x, y, realnum;

	if (14 - i < j)
	{
		y = 14;
		x = j - 14 + i;
		realnum = 14 - i;
	}
	else
	{
		x = 0;
		y = i + j;
		realnum = j;
	}
	int k;
	for (k = 0; k < GRID_NUM; k++)
	{
		if (x + k > 14 || y - k < 0)
			break;
		tempArray[k] = position[y - k][x + k];
	}
	AnalysisLine(tempArray, k, j - x);
	for (int s = 0; s < k; s++)
	{
		if (m_LineRecord[s] != TOBEANALSIS)
			TypeRecord[y - s][x + s][3] = m_LineRecord[s];
	}
	return TypeRecord[i][j][3];
}

int CEveluation::AnalysisLine(unsigned char *position, int GridNum, int StonePos)
{
	unsigned char StoneType;
	unsigned char AnalyLine[30];
	int nAnalyPos;
	int LeftEdge, RightEdge;
	int LeftRange, RightRange;

	if (GridNum < 5)
	{
		memset(m_LineRecord, ANALSISED, GridNum);
		return 0;
	}

	nAnalyPos = StonePos;
	memset(m_LineRecord, TOBEANALSIS, 30);
	memset(AnalyLine, 0x0F, 30);
	memcpy(&AnalyLine, position, GridNum);

	GridNum--;

	StoneType = AnalyLine[nAnalyPos];
	LeftEdge = nAnalyPos;
	RightEdge = nAnalyPos;

	while (LeftEdge > 0)
	{
		if (AnalyLine[LeftEdge - 1] != StoneType)
			break;
		LeftEdge--;
	}

	while (RightEdge < GridNum)
	{
		if (AnalyLine[RightEdge + 1] != StoneType)
			break;
		RightEdge++;
	}

	LeftRange = LeftEdge;
	RightRange = RightEdge;

	while (LeftRange > 0)
	{
		if (AnalyLine[LeftRange - 1] == !StoneType)
			break;
		LeftRange--;
	}

	while (RightRange < GridNum)
	{
		if (AnalyLine[RightRange + 1] == !StoneType)
			break;
		RightRange++;
	}

	if (RightRange - LeftRange < 4)
	{
		for (int k = LeftRange; k <= RightRange; k++)
			m_LineRecord[k] = ANALSISED;
		return false;
	}

	for (int k = LeftEdge; k <= RightEdge; k++)
		m_LineRecord[k] = ANALSISED;

	if (RightEdge - LeftEdge > 3)
	{
		m_LineRecord[nAnalyPos] = FIVE;
		return FIVE;
	}

	if (RightEdge - LeftEdge == 3)
	{
		bool Leftfour = false;
		if (LeftEdge > 0)
		{
			if (AnalyLine[LeftEdge - 1] == NOSTONE)
				Leftfour = true;
		}

		if (RightEdge < GridNum)
		{
			if (AnalyLine[RightEdge + 1] == NOSTONE)
			{
				if (Leftfour == true)
					m_LineRecord[nAnalyPos] = FOUR;
				else
					m_LineRecord[nAnalyPos] = SFOUR;
			}
			else
			{
				if (Leftfour == true)
					m_LineRecord[nAnalyPos] = SFOUR;
			}
		}
		else
		{
			if (Leftfour = true)
				m_LineRecord[nAnalyPos] = SFOUR;
		}

		return m_LineRecord[nAnalyPos];
	}

	if (RightEdge - LeftEdge == 2)
	{
		bool LeftThree = false;
		if (LeftEdge > 0)
		{

			if (AnalyLine[LeftEdge - 1] == NOSTONE)
			{
				if (LeftEdge > 1 && AnalyLine[LeftEdge - 2] == AnalyLine[LeftEdge])
				{
					m_LineRecord[LeftEdge] = SFOUR;
					m_LineRecord[LeftEdge - 2] = ANALSISED;
				}
				else
					LeftThree = true;
			}
			else
			{
				if (RightEdge == GridNum || AnalyLine[RightEdge + 1] != NOSTONE)
					return false;
			}
		}

		if (RightEdge < GridNum)
		{
			if (AnalyLine[RightEdge + 1] == NOSTONE)
			{
				if (RightEdge < GridNum - 1 && AnalyLine[RightEdge + 2] == AnalyLine[RightEdge])
				{
					m_LineRecord[RightEdge] = SFOUR;
					m_LineRecord[RightEdge + 2] = ANALSISED;
				}
				else
				{
					if (LeftThree == true)
						m_LineRecord[RightEdge] = THREE;
					else
						m_LineRecord[RightEdge] = STHREE;
				}
			}
			else
			{
				if (m_LineRecord[LeftEdge] == SFOUR)
					return m_LineRecord[LeftEdge];
				if (LeftThree == true)
					m_LineRecord[nAnalyPos] = STHREE;
			}
		}
		else
		{
			if (m_LineRecord[LeftEdge] == SFOUR)
				return m_LineRecord[LeftEdge];
			if (LeftThree == true)
				m_LineRecord[nAnalyPos] = STHREE;
		}
		return m_LineRecord[nAnalyPos];
	}

	if (RightEdge - LeftEdge == 1)
	{
		bool Lefttwo = false;
		bool Leftthree = false;
		if (LeftEdge > 2)
		{
			if (AnalyLine[LeftEdge - 1] == NOSTONE)
			{
				if (LeftEdge - 1 > 1 && AnalyLine[LeftEdge - 2] == AnalyLine[LeftEdge])
				{
					if (AnalyLine[LeftEdge - 3] == AnalyLine[LeftEdge])
					{
						m_LineRecord[LeftEdge - 3] = ANALSISED;
						m_LineRecord[LeftEdge - 2] = ANALSISED;
						m_LineRecord[LeftEdge] = SFOUR;
					}
					else if (AnalyLine[LeftEdge - 3] == NOSTONE)
					{
						m_LineRecord[LeftEdge - 2] = ANALSISED;
						m_LineRecord[LeftEdge] = STHREE;
					}
				}
				else
					Lefttwo = true;
			}
		}
		if (RightEdge < GridNum)
		{
			if (AnalyLine[RightEdge + 1] == NOSTONE)
			{
				if (RightEdge + 1 < GridNum - 1 && AnalyLine[RightEdge + 2] == AnalyLine[RightEdge])
				{
					if (AnalyLine[RightEdge + 3] == AnalyLine[RightEdge])
					{
						m_LineRecord[RightEdge + 3] = ANALSISED;
						m_LineRecord[RightEdge + 2] = ANALSISED;
						m_LineRecord[RightEdge] = SFOUR;
					}
					else if (AnalyLine[RightEdge + 3] == NOSTONE)
					{
						m_LineRecord[RightEdge + 2] = ANALSISED;
						if (Lefttwo == true)
							m_LineRecord[RightEdge] = THREE;
						else
							m_LineRecord[RightEdge] = STHREE;
					}
				}
				else
				{
					if (m_LineRecord[LeftEdge] == SFOUR)
						return m_LineRecord[LeftEdge];
					if (m_LineRecord[LeftEdge] == STHREE)
					{
						m_LineRecord[LeftEdge] = THREE;
						return m_LineRecord[LeftEdge];
					}

					if (Lefttwo == true)
						m_LineRecord[nAnalyPos] = TWO;
					else
						m_LineRecord[nAnalyPos] = STWO;
				}
			}
			else
			{
				if (m_LineRecord[LeftEdge] == SFOUR)
					return m_LineRecord[LeftEdge];
				if (Lefttwo == true)
					m_LineRecord[nAnalyPos] = STWO;
			}
		}
		return m_LineRecord[nAnalyPos];
	}

	return 0;
}