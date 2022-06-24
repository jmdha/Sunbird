#include "headers/minimax.hh"

MiniMax::MiniMax(BitBoard* board) {
    this->board = board;
    evaluators[(int) Color::White] = new Evaluator(Color::White);
    evaluators[(int) Color::Black] = new Evaluator(Color::Black);
    moveGens[(int) Color::White] = new MoveGen(Color::White);
    moveGens[(int) Color::Black] = new MoveGen(Color::Black);
}

Move MiniMax::GetBestMove(int depth) {
    Move move;
    int alpha = -(int) PieceValue::Inf;
    int beta = (int) PieceValue::Inf;
    NegaMax(true, &move, depth, alpha, beta);
    return move;
}

std::random_device rd;
std::mt19937 g(rd());

int MiniMax::NegaMax(bool original, Move* bestMove, int depth, int alpha, int beta) {
    if (depth == 0)
        return evaluators[(int) board->GetColor()]->EvaluatePieceCount(*board);
    std::vector<Move> moves = moveGens[(int) board->GetColor()]->GetAllMoves(*board);
    std::shuffle(moves.begin(), moves.end(), g);

    int score = -(int) PieceValue::Inf;

    for (int i = 0; i < moves.size(); i++) {
        board->DoMove(moves[i]);
        int tempScore = -NegaMax(false, bestMove, depth - 1, -beta, -alpha);
        board->UndoMove(moves[i]);

        if (tempScore > score) {
            if (original)
                *bestMove = moves[i];
            score = tempScore;
        }

        alpha = std::max(alpha, score);
        if (alpha > beta)
            break;
    }
    if (original)
        
    return score;
}
