#include "board.h"
#include <algorithm>

const P dv[8] = {{1, 0}, {1, 1}, {0, 1}, {1, -1}, -dv[0], -dv[1], -dv[2], -dv[3]};

ChessBoard::ChessBoard() : _stat(BoardState::TurnOfBlack)
{
    for (auto &v : _brd)
    {
        std::fill(v.begin(), v.end(), PieceState::Empty);
    }
}

const BoardMat &ChessBoard::board() const noexcept
{
    return _brd;
}

const BoardState &ChessBoard::state() const noexcept
{
    return _stat;
}

BoardState &ChessBoard::state() noexcept
{
    return _stat;
}

const BoardVec &ChessBoard::operator[](size_t n) const noexcept
{
    return _brd[n];
}

const PieceState &ChessBoard::operator[](P p) const noexcept
{
    return _brd[p.x][p.y];
}

BoardMat &ChessBoard::board() noexcept
{
    return _brd;
}

BoardVec &ChessBoard::operator[](size_t n) noexcept
{
    return _brd[n];
}

PieceState &ChessBoard::operator[](P p) noexcept
{
    return _brd[p.x][p.y];
}

bool ChessBoard::inBoard(P p)
{
    return 0 <= p.x && p.x < BoardSize && 0 <= p.y && p.y < BoardSize;
}

bool ChessBoard::equal(P p, PieceState clr) const noexcept
{
    return inBoard(p) && clr == _brd[p.x][p.y];
}

bool ChessBoard::unreachable(P p, PieceState clr) const noexcept
{
    return !inBoard(p) || clr == -_brd[p.x][p.y];
}

BoardState ChessBoard::put(int x, int y) noexcept
{
    return put(P(x, y));
}

BoardState ChessBoard::put(P p) noexcept
{
    PieceState clr = _stat == BoardState::TurnOfBlack ? PieceState::Black : PieceState::White;
    int maxCnt = 0;
    _brd[p.x][p.y] = clr;
    for (int i = 0; i < 4; i++)
    {
        int cnt = 1;
        for (P tmp = p + dv[i]; equal(tmp, clr); tmp += dv[i])
        {
            ++cnt;
        }
        for (P tmp = p + dv[i + 4]; equal(tmp, clr); tmp += dv[i + 4])
        {
            ++cnt;
        }
        maxCnt = std::max(maxCnt, cnt);
    }
    if (maxCnt >= 5)
    {
        _stat = clr == PieceState::Black ? BoardState::WinBlack : BoardState::WinWhite;
    }
    else
    {
        _stat = clr == PieceState::Black ? BoardState::TurnOfWhite : BoardState::TurnOfBlack;
    }

    return _stat;
}
