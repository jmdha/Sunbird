#include "headers/perft.hh"

Perft::Perft(BitBoard* board) {
    this->board = board;
    moveGens[(int) Color::White] = new MoveGen(Color::White);
    moveGens[(int) Color::Black] = new MoveGen(Color::Black);
}

int Perft::RunFromPosition(int depth) {
    U64 attacks = 0;
    Move* moves = (Move*) calloc(256, sizeof(Move));
    int moveCount = moveGens[(int) Utilities::GetOppositeColor(board->GetColor())]->GetAllMoves(moves, *board, &attacks);
    int leafCount = Run(depth, attacks);
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
        if (moveGens[(int) Utilities::GetOppositeColor(board->GetColor())]->IsKingSafe(*board)) {
            leafCount += Run(depth - 1, attackSquares);
        }
        board->UndoMove(moves[i]);
    }

    free(moves);
    return leafCount;
}
