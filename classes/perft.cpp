#include "headers/perft.hh"

Perft::Perft(BitBoard* board) {
    this->board = board;
    moveGens[(int) Color::White] = new MoveGen(Color::White);
    moveGens[(int) Color::Black] = new MoveGen(Color::Black);
}

int Perft::RunFromPosition(int depth) {
    int leafCount = Run(depth, 0);
    return leafCount;
}

int Perft::Run(int depth, U64 attackedSquares) {
    if (depth == 0)
        return 1;
    Move* moves = (Move*) calloc(256, sizeof(Move));
    U64 attackSquares = attackedSquares;
    int moveCount = moveGens[(int) board->GetColor()]->GetAllMoves(moves, *board, &attackSquares);
    int leafCount = 0;

    for (int i = 0; i < moveCount; i++) {
        board->DoMove(moves[i]);
        leafCount += Run(depth - 1, attackSquares);
        board->UndoMove(moves[i]);
    }

    free(moves);
    return leafCount;
}
