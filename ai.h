#ifndef AI_H
#define AI_H

#include "defs.h"
#include <vector>

namespace AI
{
P solve(const ChessBoard &brd);

WeightType DFS(ChessBoard &brd, WeightType A, WeightType B, int depth,
               PieceState clr);

std::vector<WeightedPoint> getNext(const ChessBoard &brd, const TwoWeightedMat &dwmat);

// 评估当前局势：记录每个位置每个方向的连子类型至rtmat，记录每个位置评估值至wmat
WeightType assessBoard(const ChessBoard &brd, TwoWeightedMat &dwmat);

WeightType assessBoard(const ChessBoard &brd, WeightedMat &wmat, const PieceState clr, P S, P dir);

} // namespace AI

#endif // AI_H