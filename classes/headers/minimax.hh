#ifndef MINIMAX
#define MINIMAX

#include <random>
#include <algorithm>
#include <iterator>

#include "constants.hh"
#include "board.hh"
#include "move_gen.hh"
#include "move.hh"
#include "evaluator.hh"

class MiniMax {
public:
    struct Stats {
        U64 negamaxExpansionCount = 0;
        U64 quiesceExpansionCount = 0;
    };
    MiniMax(Board* board) : board(board), evaluator(Evaluator(board->GetColor())), moveGens{ MoveGen(Color::White), MoveGen(Color::Black) } {}
    Move GetBestMove(int depth);
    
private:
    Board *board;
    MoveGen moveGens[2];
    Evaluator evaluator;

    Move NegaMax(int depth);
    int NegaMax(int depth, int alpha, int beta);
    int Quiesce(int alpha, int beta);
    static inline void ReOrderMoves(std::array<Move, MAXMOVECOUNT> &moves, U8 moveCount);
};

inline void MiniMax::ReOrderMoves(std::array<Move, MAXMOVECOUNT> &moves, U8 moveCount) {
    struct {
        bool operator()(Move &a, Move &b) const {
            if (a.IsCapture()) {
                if (!b.IsCapture())
                    return true;
                else
                    return false;
            } else {
                if (!a.IsCapture())
                    return false;
                else
                    return true;
            }
        }
    } moveCompare;
    std::sort(moves.begin(), std::next(moves.begin(), moveCount), moveCompare);
}

#endif // MINIMAX
