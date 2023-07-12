#include "chess/perft.hpp"

int Perft::Run(int depth) {
    if (depth == 0)
        return 1;
    std::array<Move, MAXMOVECOUNT> moves{};
    int moveCount = moveGens[(int) board->GetColor()].GetAllMoves(moves, board);
    int leafCount = 0;

    for (int i = 0; i < moveCount; i++) {
        board->DoMove(moves[i]);
        moveTypeCount[(int) moves[i].GetType()]++;
        leafCount += Run(depth - 1);
        board->UndoMove(moves[i]);
    }

    return leafCount;
}

int Perft::RunFromPosition(int depth) {
    return this->Run(depth);
}

int Perft::PerftDivide(int depth) {
    int total = 0;

    std::array<Move, MAXMOVECOUNT> moves{};
    int moveCount = moveGens[(int) board->GetColor()].GetAllMoves(moves, board);

    for (int i = 0; i < moveCount; i++) {
        board->DoMove(moves[i]);
        int count = Run(depth - 1);
        board->UndoMove(moves[i]);
        printf("%s: %d\n", moves.at(i).ToString().c_str(), count);
        total += count;
    }

    return total;
}
