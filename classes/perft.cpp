#include "headers/perft.hh"

Perft::Perft(Board* board) {
    this->board = board;
    moveGens[(int) Color::White] = new MoveGen(Color::White);
    moveGens[(int) Color::Black] = new MoveGen(Color::Black);
}

int Perft::RunFromPosition(int depth) {
    U64 attacks[2] = { 0, 0 };
    Move* moves = (Move*) calloc(MAXMOVECOUNT, sizeof(Move));
    int moveCount = moveGens[(int) Utilities::GetOppositeColor(board->GetColor())]->GetAllMoves(moves, *board, &attacks);
    int leafCount = Run(depth, attacks);
    return leafCount;
}

int Perft::Run(int depth, U64 attackedSquares[2]) {
    if (depth == 0)
        return 1;
    Move* moves = (Move*) calloc(MAXMOVECOUNT, sizeof(Move));
    U64 attackSquares[2] = { attackedSquares[0], attackedSquares[1] };
    int moveCount = moveGens[(int) board->GetColor()]->GetAllMoves(moves, *board, &attackSquares);
    int leafCount = 0;

    for (int i = 0; i < moveCount; i++) {
        board->DoMove(moves[i]);
        if (moves[i].ToString() == "g2g3")
            printf("\n");
        if (moveGens[(int) Utilities::GetOppositeColor(board->GetColor())]->IsKingSafe(*board)) {
            #ifdef DEBUG_MOVE_TYPE_COUNT
                moveTypeCount[(int) moves[i].type]++;
            #endif 
            #ifdef DEBUG_PRINT_MOVES
                for (int i = 0; i < depth - 1; i++)
                    printf("\t");
                printf("%s\n", moves[i].ToString().c_str());
            #endif
            leafCount += Run(depth - 1, attackSquares);
        }
        board->UndoMove(moves[i]);
    }

    free(moves);
    return leafCount;
}
