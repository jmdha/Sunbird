#include "headers/perft.hh"

int Perft::Run(int depth) {
    if (depth == 0)
        return 1;
    std::array<Move, MAXMOVECOUNT> moves{};
    U64 attackedSquares = board->GenerateAttackSquares(board->GetOppColor());
    int moveCount = moveGens[(int) board->GetColor()].GetAllMoves(&moves, board, attackedSquares);
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
    U64 attackedSquares = board->GenerateAttackSquares(board->GetOppColor());
    int moveCount = moveGens[(int) board->GetColor()].GetAllMoves(&moves, board, attackedSquares);

    for (int i = 0; i < moveCount; i++) {
        board->DoMove(moves[i]);
        int count = Run(depth - 1);
        board->UndoMove(moves[i]);
        printf("%s: %d\n", moves.at(i).ToString().c_str(), count);
        total += count;
    }

    return total;
}
