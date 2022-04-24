#ifndef DEFS_H
#define DEFS_H

#include "point.h"
#include <array>
#include <cstddef>
#include <cstdint>
#include <utility>

using P = Point<int>;

enum : int
{
    BoardSize = 15,
    MaxDepth = 8
};

enum RowType : int
{
    Live1,
    Live2,
    Live3,
    Live4,
    Five,
    Dead1,
    Dead2,
    Dead3,
    Dead4,
    None
};

enum PieceState : char
{
    White = -1,
    Empty = 0,
    Black = 1
};

enum class BoardState : int
{
    TurnOfBlack,
    TurnOfWhite,
    WinBlack,
    WinWhite,
    Empty,
    Error
};

using BoardVec = std::array<PieceState, BoardSize>;
using BoardMat = std::array<BoardVec, BoardSize>;

using WeightType = float;
using WeightedMat = std::array<std::array<WeightType, BoardSize>, BoardSize>;
using RowTypeMatVec = std::array<WeightedMat, 4>;
using WeightedPoint = std::pair<P, WeightType>;

// 0->Black    1->White
using TwoWeightedMat = std::array<WeightedMat, 2>;
// 0->Black    1->White
using TwoRowTypeMatVec = std::array<RowTypeMatVec, 2>;

class ChessBoard;

extern const P dv[8];

#endif // DEFS_H