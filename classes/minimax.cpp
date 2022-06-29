#include "headers/minimax.hh"

MiniMax::MiniMax(BitBoard* board) {
    this->board = board;
    evaluators[(int) Color::White] = new Evaluator(Color::White);
    evaluators[(int) Color::Black] = new Evaluator(Color::Black);
    moveGens[(int) Color::White] = new MoveGen(Color::White);
    moveGens[(int) Color::Black] = new MoveGen(Color::Black);
}

Move MiniMax::GetBestMove(int depth) {
    Move* move;
    int alpha = -(int) PieceValue::Inf;
    int beta = (int) PieceValue::Inf;
    NegaMax(true, move, depth, alpha, beta, 0);
    return *move;
}

std::random_device rd;
std::mt19937 g(rd());

int MiniMax::NegaMax(bool original, Move* bestMove, int depth, int alpha, int beta, U64 attackedSquares) {
    if (depth == 0)
        return evaluators[(int) board->GetColor()]->EvaluatePieceCount(*board);
    // 218 I believe to be the max number of moves - as such its rounded up to 256
    Move* moves = (Move*) calloc(256, sizeof(Move));
    U64 attackSquares = attackedSquares;
    int moveCount = moveGens[(int) board->GetColor()]->GetAllMoves(moves, *board, &attackSquares);
    //if (original)
        //std::shuffle(moves.begin(), moves.end(), g);

    int score = -(int) PieceValue::Inf;

    for (int i = 0; i < moveCount; i++) {
        board->DoMove(moves[i]);
        int tempScore = -NegaMax(false, bestMove, depth - 1, -beta, -alpha, attackSquares);
        board->UndoMove(moves[i]);

        if (tempScore > score) {
            if (original)
                bestMove = &moves[i];
            score = tempScore;
        }

        alpha = std::max(alpha, score);
        if (alpha >= beta)
            break;
    }
    free(moves);
    return score;
}
