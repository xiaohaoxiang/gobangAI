#include <cuda_runtime.h>
#include <thrust/sort.h>
#include "ai.h"
#include "board.h"

namespace CUAI
{
    const int MaxDepth = 8;
    const int MaxBranch = 20;

    class cuai
    {
    public:
        cudaDeviceProp devProp;
        const int BLOCK_PER_GRID = devProp.multiProcessorCount;
        const int THREAD_PER_BLOCK = devProp.maxThreadsPerBlock;
        const int TOTAL_COUNT = BLOCK_PER_GRID * THREAD_PER_BLOCK;

        cuai();
        ~cuai();
    } ai;

    cuai::cuai()
    {
        cudaGetDeviceProperties(&devProp, 0);
    }

    cuai::~cuai()
    {
    }

    __host__ P solve(const ChessBoard &board)
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

} // namespace CUAI
