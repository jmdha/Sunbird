#include "headers/minimax.hh"

MiniMax::MiniMax(BitBoard* board) {
    this->board = board;
    evaluators[(int) Color::White] = new Evaluator(Color::White);
    evaluators[(int) Color::Black] = new Evaluator(Color::Black);
    moveGens[(int) Color::White] = new MoveGen(Color::White);
    moveGens[(int) Color::Black] = new MoveGen(Color::Black);
}

Move MiniMax::GetBestMove(int depth) {
    int max = - (int) PieceValue::Inf;

    std::vector<Move> moves = moveGens[(int) board->GetColor()]->GetAllMoves(*board);
    Move bestMove;

    for (int i = 0; i < moves.size(); i++) {
        board->DoMove(moves[i]);
        int score = -NegaMax(depth - 1);
        if (score > max) {
            max = score;
            bestMove = moves[i];
        }
        board->UndoMove(moves[i]);
    }
    return bestMove;
}

int MiniMax::NegaMax(int depth) {
    if (depth == 0)
        return evaluators[(int) board->GetColor()]->EvaluatePieceCount(*board);
    
    int max = - (int) PieceValue::Inf;

    std::vector<Move> moves = moveGens[(int) board->GetColor()]->GetAllMoves(*board);

    for (int i = 0; i < moves.size(); i++) {
        board->DoMove(moves[i]);
        int score = -NegaMax(depth - 1);
        if (score > max)
            max = score;
        board->UndoMove(moves[i]);
    }
    return max;
}
