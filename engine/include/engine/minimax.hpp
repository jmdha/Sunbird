#ifndef MINIMAX
#define MINIMAX

#include <random>
#include <algorithm>
#include <iterator>

#include <chess/internal/constants.hpp>
#include <chess/board.hpp>
#include <chess/move.hpp>
#include <chess/move_gen.hpp>
#include "engine/evaluator.hpp"

class MiniMax {
public:
    explicit MiniMax(std::shared_ptr<Board> board) : board(board), evaluator(Evaluator()), moveGens{ MoveGen(Color::White), MoveGen(Color::Black) } {}
    MiniMax(std::shared_ptr<Board> board, const Evaluator& eval) : board(board), evaluator(eval), moveGens{ MoveGen(Color::White), MoveGen(Color::Black) } {}
    Move GetBestMove(int depth = -1);

private:
    struct MoveVals {
        std::array<Move, MAXMOVECOUNT> moves;
        std::array<int, MAXMOVECOUNT> scores;
        int moveCount;
        MoveVals() : moveCount(-1) {};
        void Sort() {
            struct {
                bool operator()(std::pair<Move, int> &a, std::pair<Move, int> &b) const {
                    return a.second > b.second;
                }
            } moveCompare;
            std::array<std::pair<Move, int>, MAXMOVECOUNT> moveVals;
            for (int i = 0; i < moveCount; ++i)
                moveVals.at(i) = std::make_pair(moves.at(i), scores.at(i));
            std::sort(moveVals.begin(), std::next(moveVals.begin(), moveCount), moveCompare);
            for (int i = 0; i < moveCount; ++i) {
                moves.at(i) = moveVals.at(i).first;
                scores.at(i) = moveVals.at(i).second;
            }
        }
    };
    std::shared_ptr<Board> board;
    MoveGen moveGens[2];
    Evaluator evaluator;

    MiniMax::MoveVals NegaMax(int depth, U64 timeLimit, U64 *timeUsed, MoveVals moveVals = MoveVals());
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
