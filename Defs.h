#pragma once

#include <array>

enum class DotState
{
    Null,
    Black,
    White
};

enum class KeyType
{
    Up,
    Down,
    Left,
    Right,
    Dot,
    Quit,
    Clear,
    AiDo
};

using ChessBoard = std::array<std::array<DotState, 15>, 15>;