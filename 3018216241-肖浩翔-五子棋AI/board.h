#ifndef BOARD_H
#define BOARD_H

#include "defs.h"

class ChessBoard
{
  private:
    BoardMat _brd;
    BoardState _stat;

  public:
    ChessBoard();
    const BoardMat &board() const noexcept;
    const BoardState &state() const noexcept;
    BoardState &state() noexcept;
    const BoardVec &operator[](size_t n) const noexcept;
    const PieceState &operator[](P p) const noexcept;
    BoardMat &board() noexcept;
    BoardVec &operator[](size_t n) noexcept;
    PieceState &operator[](P p) noexcept;
    BoardState put(int x, int y) noexcept;
    BoardState put(P p) noexcept;
    bool equal(P p, PieceState clr) const noexcept;
    bool unreachable(P p, PieceState clr) const noexcept;
    static bool inBoard(P p);
};

#endif // BOARD_H