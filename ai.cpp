#include "ai.h"
#include "board.h"
#include <algorithm>
#include <array>
#include <cstring>
#include <deque>
#include <memory>
#include <random>

namespace AI
{

    const int MaxDepth = 8;
    const int MaxBranch = 20;
    const WeightType RowTypeScore[32] = {0, 1, 1, 10, 1, 10, 10, 100, 1, 10, 10,
                                         100, 10, 100, 100, 10000, 1, 5, 10, 30, 10, 100,
                                         100, 10000, 10, 100, 100, 1000, 100, 10000, 10000, 10000000};
    const WeightType INF = 1e20;

    const int MaxTableSize = 100000000;

    TwoWeightedMat WMat;

    P solve(const ChessBoard &board)
    {
        ChessBoard brd = board;
        if (brd.state() == BoardState::Empty)
        {
            return P(7, 7);
        }
        else
        {
            PieceState clr = board.state() == BoardState::TurnOfBlack ? PieceState::Black : PieceState::White;

            std::memset(&WMat, 0, sizeof(WMat));
            assessBoard(brd, WMat);
            auto &&nextSteps = getNext(brd, WMat);
            WeightType best = -INF;
            WeightType A = -INF, B = INF;
            P res;

            for (const auto &i : nextSteps)
            {
                brd[i.first] = clr;
                best = std::max(best, DFS(brd, -B,
                                          -A, 0, PieceState(-clr)));

                brd[i.first] = PieceState::Empty;
                if (best > A)
                {
                    A = best;
                    res = i.first;
                }
            }
            return res;
        }
    }

    WeightType DFS(ChessBoard &brd, WeightType A, WeightType B, int depth,
                   PieceState clr)
    {
        WeightType res = 0;

        if (depth == MaxDepth)
        {
            return std::max(res, A);
        }
        else
        {
            auto &&nextSteps = getNext(brd, WMat);

            WeightType best = -INF;
            for (const auto &i : nextSteps)
            {
                brd[i.first] = clr;
                best = std::max(best, -DFS(brd, -B,
                                           -A, depth + 1, PieceState(-clr)));
                brd[i.first] = PieceState::Empty;
                if (best >= B)
                {
                    break;
                }
                A = std::max(A, best);
                if (best >= RowTypeScore[31])
                {
                    break;
                }
            }
            return best * 0.9;
        }
    }

    std::vector<WeightedPoint> getNext(const ChessBoard &brd, const TwoWeightedMat &dwmat)
    {
        std::vector<WeightedPoint> res;
        for (int i = 0; i < BoardSize; ++i)
        {
            for (int j = 0; j < BoardSize; ++j)
            {
                if (brd[i][j] == PieceState::Empty)
                {
                    res.push_back(std::make_pair(P(i, j), dwmat[0][i][j] + dwmat[1][i][j]));
                }
            }
        }
        std::sort(res.begin(), res.end(),
                  [](const WeightedPoint &_L, const WeightedPoint &_R)
                  { return _L.second > _R.second; });

        if (res.size() > MaxBranch)
        {
            res.resize(MaxBranch);
        }
        return std::move(res);
    }

    WeightType assessBoard(const ChessBoard &brd, TwoWeightedMat &dwmat)
    {
        WeightType res1 = 0, res2 = 0;

        for (int i = 0; i < BoardSize; ++i)
        {
            res1 = std::max(res1, assessBoard(brd, dwmat[0], PieceState::Black, P(0, i), P(1, 0)));
            res1 = std::max(res1, assessBoard(brd, dwmat[0], PieceState::Black, P(i, 0), P(0, 1)));
            res1 = std::max(res1, assessBoard(brd, dwmat[0], PieceState::Black, P(0, i), P(1, 1)));
            res1 = std::max(res1, assessBoard(brd, dwmat[0], PieceState::Black, P(0, i), P(1, -1)));

            res2 = std::max(res2, assessBoard(brd, dwmat[1], PieceState::White, P(0, i), P(1, 0)));
            res2 = std::max(res2, assessBoard(brd, dwmat[1], PieceState::White, P(i, 0), P(0, 1)));
            res2 = std::max(res2, assessBoard(brd, dwmat[1], PieceState::White, P(0, i), P(1, 1)));
            res2 = std::max(res2, assessBoard(brd, dwmat[1], PieceState::White, P(0, i), P(1, -1)));
        }
        for (int i = 1; i < BoardSize; ++i)
        {
            res1 = std::max(res1, assessBoard(brd, dwmat[0], PieceState::Black, P(i, 0), P(1, 1)));
            res1 = std::max(res1, assessBoard(brd, dwmat[0], PieceState::Black, P(i, BoardSize - 1), P(1, -1)));

            res2 = std::max(res2, assessBoard(brd, dwmat[1], PieceState::White, P(i, 0), P(1, 1)));
            res2 = std::max(res2, assessBoard(brd, dwmat[1], PieceState::White, P(i, BoardSize - 1), P(1, -1)));
        }
        return res1 - res2;
    }

    WeightType assessBoard(const ChessBoard &brd, WeightedMat &wmat, const PieceState clr, P S, P dir)
    {
        std::deque<WeightType> dq;
        int curStat = 0, last = 0;
        P cur = S;
        WeightType sum = 0, res = 0;

        for (int i = 0; i < 5; i++, cur += dir)
        {
            const PieceState c = brd[cur];
            WeightType score;
            --last;
            if (c == -clr)
            {
                last = 5;
                curStat = 0;
            }
            else
            {
                curStat = ((curStat << 1) + (c == clr)) & 31;
            }
            if (last > 0)
            {
                score = 0;
            }
            else
            {
                score = RowTypeScore[curStat];
                if (score < 1000)
                {
                    if (last == 0)
                    {
                        score *= 0.1;
                    }
                    if (!ChessBoard::inBoard(cur + dir))
                    {
                        return 0;
                    }
                    else if (brd[cur + dir] == -c)
                    {
                        score *= 0.1;
                    }
                }
            }
            sum += score;
            dq.push_back(score);
        }
        P pst = S;
        for (; ChessBoard::inBoard(cur); cur += dir)
        {
            const PieceState c = brd[cur];
            WeightType score;
            --last;
            if (c == -clr)
            {
                last = 5;
                curStat = 0;
            }
            else
            {
                curStat = ((curStat << 1) + (c == clr)) & 31;
            }
            if (last > 0)
            {
                score = 0;
            }
            else
            {
                score = RowTypeScore[curStat];
                if (score < 1000)
                {
                    if (last == 0)
                    {
                        score *= 0.1;
                    }
                    if (brd.unreachable(cur + dir, clr))
                    {
                        score *= 0.1;
                    }
                }
            }

            wmat[pst.x][pst.y] += sum;
            res += sum;
            sum -= dq.front();
            dq.pop_front();
            pst += dir;

            sum += score;
            dq.push_back(score);
        }

        for (; ChessBoard::inBoard(pst); pst += dir)
        {
            wmat[pst.x][pst.y] += sum;
            res += sum;
            sum -= dq.front();
            dq.pop_front();
        }
        return res;
    }

} // namespace AI
