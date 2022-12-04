#include "headers/minimax.hh"

MiniMax::MiniMax(Board* board) : evaluator(Evaluator(board->color)) {
    this->board = board;
    moveGens[(int) Color::White] = new MoveGen(Color::White);
    moveGens[(int) Color::Black] = new MoveGen(Color::Black);
}

Move MiniMax::GetBestMove(int depth) {
    return NegaMax(depth);
}

Move MiniMax::NegaMax(int depth) {
    Move* moves = (Move*) calloc(MAXMOVECOUNT, sizeof(Move));
    U64 attacks[2] = { 0, 0 };
    int moveCount = moveGens[(int) board->GetColor()]->GetAllMoves(moves, *board, &attacks);

    Move bestMove = Move(MoveType::Quiet);
    int bestScore = -(int) PieceValue::Inf;

    for (int i = 0; i < moveCount; i++) {
        auto currentMove = &moves[i];

        board->DoMove(currentMove);
        int score = NegaMax(depth - 1, -(int) PieceValue::Inf, (int) PieceValue::Inf, attacks);
        board->UndoMove(*currentMove);

        if (score > bestScore) {
            bestScore = score;
            bestMove = moves[i];
        }
    }

    free(moves);
    return bestMove;
}

int MiniMax::NegaMax(int depth, int alpha, int beta, U64 attackedSquares[2]) {
    if (depth == 0)
        return evaluator.Evalute(*board);
    
    Move* moves = (Move*) calloc(MAXMOVECOUNT, sizeof(Move));
    U64 attackSquares[2] = { attackedSquares[0], attackedSquares[1] };
    int moveCount = moveGens[(int) board->GetColor()]->GetAllMoves(moves, *board, &attackSquares);

    for (int i = 0; i < moveCount; i++) {
        const Move currentMove = moves[i];

        board->DoMove(&moves[i]);
        int score = -NegaMax(depth - 1, -beta, -alpha, attackSquares);
        board->UndoMove(moves[i]);

        if (score >= beta)
            return beta;
        if (score > alpha)
            alpha = score;
    }

    free(moves);
    return alpha;
}
