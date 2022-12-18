#include "headers/perft.hh"

int Perft::RunFromPosition(int depth) {
    U64 attacks[2] = { 0, 0 };
    std::array<Move, MAXMOVECOUNT> moves;
    int moveCount = moveGens[(int) Utilities::GetOppositeColor(board->GetColor())].GetAllMoves(&moves, *board, &attacks);
    int leafCount = Run(depth, attacks);
    return leafCount;
}

int Perft::Run(int depth, U64 attackedSquares[2]) {
    if (depth == 0)
        return 1;
    std::array<Move, MAXMOVECOUNT> moves;
    U64 attackSquares[2] = { attackedSquares[0], attackedSquares[1] };
    int moveCount = moveGens[(int) board->GetColor()].GetAllMoves(&moves, *board, &attackSquares);
    int leafCount = 0;

    for (int i = 0; i < moveCount; i++) {
        board->DoMove(&moves[i]);
        if (moveGens[(int) Utilities::GetOppositeColor(board->GetColor())].IsKingSafe(*board)) {
            moveTypeCount[(int) moves[i].GetType()]++;
            leafCount += Run(depth - 1, attackSquares);
        }
        board->UndoMove(moves[i]);
    }

    return leafCount;
}
