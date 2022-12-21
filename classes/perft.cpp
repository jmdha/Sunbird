#include "headers/perft.hh"

int Perft::RunFromPosition(int depth) {
    int leafCount = Run(depth);
    return leafCount;
}

int Perft::Run(int depth) {
    if (depth == 0)
        return 1;
    std::array<Move, MAXMOVECOUNT> moves;
    U64 attackedSquares = moveGens[(int) Utilities::GetOppositeColor(board->GetColor())].GetAttackSquares(board);
    int moveCount = moveGens[(int) board->GetColor()].GetAllMoves(&moves, board, attackedSquares);
    int leafCount = 0;

    for (int i = 0; i < moveCount; i++) {
        board->DoMove(moves[i]);
        if (moveGens[(int) Utilities::GetOppositeColor(board->GetColor())].IsKingSafe(board)) {
            moveTypeCount[(int) moves[i].GetType()]++;
            leafCount += Run(depth - 1);
        }
        board->UndoMove(moves[i]);
    }

    return leafCount;
}
