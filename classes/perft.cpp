#include "headers/perft.hh"

Perft::Perft(BitBoard* board) {
    this->board = board;
    moveGens[(int) Color::White] = new MoveGen(Color::White);
    moveGens[(int) Color::Black] = new MoveGen(Color::Black);
}

int Perft::RunFromMove(Move move, int depth) {
    board->DoMove(move);
    int leafCount = Run(depth - 1);
    board->UndoMove(move);
    return leafCount;
}

int Perft::Run(int depth) {
    if (depth == 0)
        return 1;
    Move* moves = (Move*) calloc(256, sizeof(Move));
    int moveCount = moveGens[(int) board->GetColor()]->GetAllMoves(moves, *board);
    int leafCount = 0;

    for (int i = 0; i < moveCount; i++) {
        board->DoMove(moves[i]);
        leafCount += Run(depth - 1);
        board->UndoMove(moves[i]);
    }

    free(moves);
    return leafCount;
}
