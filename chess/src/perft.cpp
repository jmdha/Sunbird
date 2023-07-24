#include <chess/perft.hpp>

using namespace Chess;

int Perft::Run(Board &board, int depth) {
    if (depth == 0)
        return 1;
    MoveList moves = MoveGen::GenerateMoves<MoveGen::GenType::All>(board, board.GetColor());
    int leafCount = 0;

    for (auto move : moves) {
        auto hash = board.GetHash();
        board.DoMove(move);
        leafCount += Run(board, depth - 1);
        board.UndoMove(move);
        assert(hash == board.GetHash());
    }

    return leafCount;
}

int Perft::RunFromPosition(Board &board, int depth) { return this->Run(board, depth); }

int Perft::PerftDivide(Board &board, int depth) {
    int total = 0;

    MoveList moves = MoveGen::GenerateMoves<MoveGen::GenType::All>(board, board.GetColor());

    for (auto move : moves) {
        board.DoMove(move);
        int count = Run(board, depth - 1);
        board.UndoMove(move);
        printf("%s: %d\n", move.ToString().c_str(), count);
        total += count;
    }

    return total;
}
