#include "chess/internal/move_list.hpp"
#include "chess/move_gen.hpp"
#include <chess/perft.hpp>

namespace Chess::Perft {
namespace {
int Run(Board &board, int depth) {
    MoveList moves = MoveGen::GenerateAll(board.Pos(), board.Pos().GetTurn());
    if (depth == 1)
        return moves.size();
    int leafCount = 0;

    for (auto move : moves) {
        board.MakeMove(move);
        leafCount += Run(board, depth - 1);
        board.UndoMove();
    }

    return leafCount;
}
} // namespace

int RunFromPosition(Board &board, int depth) { return Run(board, depth); }

int PerftDivide(Board &board, int depth) {
    int total = 0;

    MoveList moves = MoveGen::GenerateAll(board.Pos(), board.Pos().GetTurn());

    for (auto move : moves) {
        board.MakeMove(move);
        int count = Run(board, depth - 1);
        board.UndoMove();
        printf("%s: %d\n", move.ToString().c_str(), count);
        total += count;
    }

    return total;
}
} // namespace Chess::Perft
