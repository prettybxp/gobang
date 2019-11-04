#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>
#include <functional>
#include "Defs.h"
#include "Eveluation.h"

#include "spdlog/spdlog.h"

// 清除屏幕
#define CLEAR() printf("\033[2J")

// 上移光标
#define MOVEUP(x) printf("\033[%dA", (x))

// 下移光标
#define MOVEDOWN(x) printf("\033[%dB", (x))

// 左移光标
#define MOVELEFT(y) printf("\033[%dD", (y))

// 右移光标
#define MOVERIGHT(y) printf("\033[%dC", (y))

// 定位光标
#define MOVETO(x, y) printf("\033[%d;%dH", (x), (y))

// 光标复位
#define RESET_CURSOR() printf("\033[H")

// 隐藏光标
#define HIDE_CURSOR() printf("\033[?25l")

// 显示光标
#define SHOW_CURSOR() printf("\033[?25h")

//反显
#define HIGHT_LIGHT() printf("\033[7m")
#define UN_HIGHT_LIGHT() printf("\033[27m")

class ConsoleView
{
public:
    ConsoleView(ChessBoard &board, bool &turn) : m_board(board), m_whiteTrue(turn)
    {
        set_keypress();
        show();
    }

    ~ConsoleView()
    {
        reset_keypress();
        CLEAR();
        RESET_CURSOR();
    }

    void setProcess(size_t index, size_t count)
    {
        m_index = index;
        m_count = count;
        showProcess();
        MOVETO(Row_To_X(m_row), Col_To_Y(m_col));
        flush();
    }

    void run(std::function<void(KeyType key, size_t &, size_t &)> call)
    {
        char before[2] = {0, 0};
        while (true)
        {
            char c = getchar();
            if (c == 65 && before[0] == 91 && before[1] == 27)
            {
                c = 'w';
            }
            else if (c == 66 && before[0] == 91 && before[1] == 27)
            {
                c = 's';
            }
            else if (c == 67 && before[0] == 91 && before[1] == 27)
            {
                c = 'd';
            }
            else if (c == 68 && before[0] == 91 && before[1] == 27)
            {
                c = 'a';
            }

            before[1] = before[0];
            before[0] = c;

            if (c == 'q')
            {
                break;
            }
            else if (c == 'w')
            {
                m_row--;
                if (m_row < 0)
                {
                    m_row = 0;
                }
            }
            else if (c == 'a')
            {
                m_col--;
                if (m_col < 0)
                {
                    m_col = 0;
                }
            }
            else if (c == 's')
            {
                m_row++;
                if (m_row >= m_board.size())
                {
                    m_row = m_board.size() - 1;
                }
            }
            else if (c == 'd')
            {
                m_col++;
                if (m_col >= m_board[0].size())
                {
                    m_col = m_board[0].size() - 1;
                }
            }
            else if (c == ' ')
            {
                call(KeyType::Dot, m_row, m_col);
                show();
                sleep(1);
                call(KeyType::AiDo, m_row, m_col);
            }
            else if (c == 127)
            {
                call(KeyType::Clear, m_row, m_col);
            }
            else
            {
                continue;
            }

            show();
        }
    }

private:
    int Row_To_X(int i)
    {
        return 2 + i;
    }

    int Col_To_Y(int i)
    {
        return 10 + i * 2;
    }

    void flush()
    {
        fflush(stdin);
        fflush(stdout);
    }

    void showCursor()
    {
        MOVETO(Row_To_X(m_row), Col_To_Y(m_col));
    }

    void showWhoTrun()
    {
        MOVETO(8, 2);
        printf("%s", m_whiteTrue ? "O" : "X");
    }

    void showCursorCoordinate()
    {
        MOVETO(10, 0);
        printf("(%lu, %lu)", m_row, m_col);
    }

    void showProcess()
    {
        MOVETO(18, 0);
        printf("percent %03lu, %03lu", m_index, m_count);
    }

    void showScore()
    {
        MOVETO(17, 0);

        unsigned char board[15][15];
        for (int i = 0; i < 15; i++)
        {
            for (int j = 0; j < 15; j++)
            {
                if (m_board[i][j] == DotState::Null)
                {
                    board[i][j] = 0xFF;
                }
                else if (m_board[i][j] == DotState::Black)
                {
                    board[i][j] = 0;
                }
                else
                {
                    board[i][j] = 1;
                }
            }
        }
        CEveluation eva;
        printf("score %d", eva.Eveluate(board, m_whiteTrue));
    }

    void showChessBoard()
    {
        for (size_t i = 0; i < m_board.size(); i++)
        {
            for (size_t j = 0; j < m_board[0].size(); j++)
            {
                MOVETO(Row_To_X(i), Col_To_Y(j));
                auto c = m_board[i][j];
                if (c == DotState::Null)
                {
                    printf(".");
                }
                else if (c == DotState::Black)
                {
                    printf("X");
                }
                else if (c == DotState::White)
                {
                    printf("O");
                }
            }
        }
    }

    void show()
    {
        CLEAR();
        showChessBoard();
        showWhoTrun();
        showCursorCoordinate();
        showProcess();
        showScore();
        showCursor();
        flush();
    }

    void set_keypress(void) //设置终端为RAW模式，并关闭回显
    {
        struct termios new_settings;
        tcgetattr(0, &stored_settings);
        new_settings = stored_settings;
        new_settings.c_lflag &= (~ICANON);
        new_settings.c_lflag &= (~ECHO);
        new_settings.c_cc[VTIME] = 0;
        new_settings.c_cc[VMIN] = 1;
        tcsetattr(0, TCSANOW, &new_settings);
    }

    void reset_keypress(void) //恢复终端属性
    {
        tcsetattr(0, TCSANOW, &stored_settings);
    }
    struct termios stored_settings;

    size_t m_row = 7;
    size_t m_col = 7;
    ChessBoard &m_board;
    bool &m_whiteTrue;
    size_t m_index = 0;
    size_t m_count = 0;
};