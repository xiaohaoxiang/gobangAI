#include "ai.h"
#include "board.h"
#include "defs.h"
#include "keys.h"
#include <iostream>

const char *const posi[] = {" 1  ", " 2  ", " 3  ", " 4  ", " 5  ", " 6  ", " 7  ", " 8  ",
                            " 9  ", " 10 ", " 11 ", " 12 ", " 13 ", " 14 ", " 15 "};

int main()
{
    ChessBoard brd;

    for (int x = 7, y = 7, chcnt = 0;;)
    {
        if (brd.state() == BoardState::WinWhite)
        {
            std::cout << "you win\n";
            return 0;
        }
        auto ai = AI::solve(brd);
        brd.put(ai);
        x = ai.x;
        y = ai.y;

        auto showBoard = [&]() {
            std::cout << "      ";
            for (int i = 0; i < BoardSize; i++)
            {
                std::cout << posi[i];
            }
            std::cout << "\n\n";

            for (int i = 0; i < BoardSize; i++)
            {
                if (i == y)
                {
                    std::cout << "      ";
                    for (int k = 0; k < BoardSize; k++)
                    {
                        std::cout << (k == x ? "|--|" : "----");
                    }
                }
                std::cout << '\n' << posi[i] << "  ";
                for (int j = 0; j < BoardSize; j++)
                {
                    switch (brd[j][i])
                    {
                    case PieceState::Black:
                        std::cout << (j == x ? "|■■|" : " ■■ ");
                        break;
                    case PieceState::White:
                        std::cout << (j == x ? "|□□|" : " □□ ");
                        break;
                    case PieceState::Empty:
                        std::cout << (j == x ? "|  |" : "    ");
                        break;
                    }
                }
                std::cout << '\n';
                if (i == y)
                {
                    std::cout << "      ";
                    for (int k = 0; k < BoardSize; k++)
                    {
                        std::cout << (k == x ? "|--|" : "----");
                    }
                }
                else if (i != y - 1)
                {
                    std::cout << "      ";
                    for (int j = 0; j < BoardSize; j++)
                    {
                        std::cout << (j == x ? "|  |" : "    ");
                    }
                }
            }
            std::cout << "\n\n";
            std::cout.flush();
        };

        if (brd.state() == BoardState::WinBlack)
        {
            showBoard();
            std::cout << "computer win\n";
            break;
        }

        showBoard();

        for (int ch, change = 0;; change = 0)
        {
            ch = keyBoradHit();
            if (ch == 'w' || ch == 'W')
            {
                if (y > 0)
                    y--;
                change = 1;
            }
            else if (ch == 'a' || ch == 'A')
            {
                if (x > 0)
                    x--;
                change = 1;
            }
            else if (ch == 's' || ch == 'S')
            {
                if (y < BoardSize - 1)
                    y++;
                change = 1;
            }
            else if (ch == 'd' || ch == 'D')
            {
                if (x < BoardSize - 1)
                    x++;
                change = 1;
            }
            else if (ch == ' ')
            {
                if (brd[x][y] == PieceState::Empty)
                {
                    break;
                }
                change = 1;
            }
            if (change)
            {
                showBoard();
            }
        }

        brd.put(x, y);
        showBoard();
    }

    return 0;
}