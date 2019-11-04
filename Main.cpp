
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

#include "ConsoleView.h"

#include "Eveluation.h"

CEveluation eva;
int log_count = 0;
unsigned long min_cut_count = 0;
unsigned long max_cut_count = 0;

std::vector<std::pair<int, int>> last;
int distance(std::pair<int, int> const &point)
{
    int lastx = 7;
    int lasty = 7;
    if (!last.empty())
    {
        lastx = last.back().first;
        lasty = last.back().second;
    }
    return std::abs(point.first - lastx) + std::abs(point.second - lasty);
}

bool compare(std::pair<int, int> const &l, std::pair<int, int> const &r)
{
    return distance(l) < distance(r);
}

bool hasNeighbor(unsigned char chess[15][15], int x, int y)
{
    int nei = 1;
    for (int i = x - nei; i <= x + nei; i++)
    {
        if (i < 0 || i >= 15)
        {
            continue;
        }
        for (int j = y - nei; j <= y + nei; j++)
        {
            if (j < 0 || j >= 15)
            {
                continue;
            }

            if (chess[i][j] != 0xff)
            {
                //spdlog::info("find i{}, j{} is nei", i, j);
                return true;
            }
        }
    }

    //spdlog::info("no nei");
    return false;
}

int min(unsigned char chess[15][15], int depth, int a, int b);

//max是ai方调用，假设ai执黑子，则上一步是白子走的，ena只能为白子评分，要得到黑子分数，要加负号
int max(unsigned char chess[15][15], int depth, int a, int b)
{
    int valg = eva.Eveluate(chess, false);
    if (depth == 0)
    {
        return -valg;
    }

    if (std::abs(valg) > 9000)
    {
        return -valg;
    }

    std::vector<std::pair<int, int>> todolist;
    for (int i = 0; i < 15; i++)
    {
        for (int j = 0; j < 15; j++)
        {
            if (chess[i][j] == 0xff && hasNeighbor(chess, i, j))
            {
                todolist.push_back(std::make_pair(i, j));
            }
        }
    }

    std::sort(todolist.begin(), todolist.end(), compare);

    //spdlog::info("in max >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> b ={}", b);
    //a = -10000;

    for (auto const &todo : todolist)
    {
        int i = todo.first;
        int j = todo.second;

        chess[i][j] = 0;
        last.push_back(std::make_pair(i, j));

        int val = min(chess, depth - 1, a, b);
        chess[i][j] = 0xff;
        last.pop_back();

        if (val > a)
        {
            //spdlog::info("in max >>>>>> update a ={},val= {}", a, val);
            a = val;
        }

        if (a >= b)
        {
            //spdlog::info("max cut out");
            max_cut_count++;
            return a;
        }

        //spdlog::info("count {}, depth {}, attemp set {},{} to {}, get value {} ", log_count++, depth, i, j, bIsBlackTurn ? "X" : "O", val);
        // if (val > best)
        // {
        //     best = val;
        // }
    }

    return a;
}

int min(unsigned char chess[15][15], int depth, int a, int b)
{
    int valg = eva.Eveluate(chess, true);

    if (depth == 0)
    {
        return valg;
    }

    if (std::abs(valg) > 9000)
    {
        return valg;
    }

    std::vector<std::pair<int, int>> todolist;
    for (int i = 0; i < 15; i++)
    {
        for (int j = 0; j < 15; j++)
        {
            if (chess[i][j] == 0xff && hasNeighbor(chess, i, j))
            {
                todolist.push_back(std::make_pair(i, j));
            }
        }
    }

    std::sort(todolist.begin(), todolist.end(), compare);

    //b = 10000;
    for (auto const &todo : todolist)
    {
        int i = todo.first;
        int j = todo.second;

        chess[i][j] = 1;
        last.push_back(std::make_pair(i, j));

        int val = max(chess, depth - 1, a, b);
        chess[i][j] = 0xff;
        last.pop_back();

        //spdlog::info("count {}, depth {}, attemp set {},{} to {}, get value {} ", log_count++, depth, i, j, bIsBlackTurn ? "X" : "O", val);

        if (val < b)
        {
            b = val;
        }

        if (a >= b)
        {
            //spdlog::info("min cut out");
            min_cut_count++;
            return b;
        }

        // if (val < best)
        // {
        //     best = val;
        // }
    }

    return b;
}

void aido(ChessBoard &board, size_t &x, size_t &y, bool whiteTrun, ConsoleView &view)
{
    min_cut_count = 0;
    max_cut_count = 0;
    unsigned char chess[15][15];
    for (int i = 0; i < 15; i++)
    {
        for (int j = 0; j < 15; j++)
        {
            if (board[i][j] == DotState::Null)
            {
                chess[i][j] = 0xFF;
            }
            else if (board[i][j] == DotState::Black)
            {
                chess[i][j] = 0;
            }
            else
            {
                chess[i][j] = 1;
            }
        }
    }
    int a = -100000;
    int b = 100000;

    std::vector<std::pair<int, int>> todolist;
    for (int i = 0; i < 15; i++)
    {
        for (int j = 0; j < 15; j++)
        {
            if (chess[i][j] == 0xff && hasNeighbor(chess, i, j))
            {
                todolist.push_back(std::make_pair(i, j));
            }
        }
    }

    size_t todosize = todolist.size();
    size_t index = 0;
    std::sort(todolist.begin(), todolist.end(), compare);

    //spdlog::info("todolist size {}", todosize);

    for (auto const &todo : todolist)
    {
        index++;
        view.setProcess(index, todosize);
        //spdlog::info("todolist index {}", index);
        int i = todo.first;
        int j = todo.second;

        chess[i][j] = 0;
        last.push_back(std::make_pair(i, j));

        int val = min(chess, 4, a, b);

        chess[i][j] = 0xff;
        last.pop_back();

        if (val > a)
        {
            a = val;
            x = i;
            y = j;
        }

        //spdlog::info("count {}, depth {}, attemp set {},{} to {}, get value {} ", log_count++, depth, i, j, bIsBlackTurn ? "X" : "O", val);
        // if (val > best)
        // {
        //     best = val;
        //     x = i;
        //     y = j;
        // }
    }

    board[x][y] = whiteTrun ? DotState::White : DotState::Black;
    spdlog::info("cut max {}, min {}", max_cut_count, min_cut_count);
}

int main(int argc, char **argv)
{
    auto file_logger = spdlog::basic_logger_mt("basic_logger", "basic.log");
    spdlog::set_default_logger(file_logger);
    spdlog::flush_on(spdlog::level::trace);

    ChessBoard board;
    std::array<DotState, 15> line;
    line.fill(DotState::Null);
    board.fill(line);

    bool whiteTrun = true;
    ConsoleView view(board, whiteTrun);

    view.run([&board, &whiteTrun, &view](KeyType key, size_t &x, size_t &y) {
        if (key == KeyType::Dot && board[x][y] == DotState::Null)
        {
            board[x][y] = whiteTrun ? DotState::White : DotState::Black;
            whiteTrun = !whiteTrun;
            last.push_back(std::make_pair(x, y));
        }
        else if (key == KeyType::Clear && board[x][y] != DotState::Null)
        {
            board[x][y] = DotState::Null;
            whiteTrun = !whiteTrun;
        }
        else if (key == KeyType::AiDo)
        {
            aido(board, x, y, whiteTrun, view);
            whiteTrun = !whiteTrun;
        }
    });
}
