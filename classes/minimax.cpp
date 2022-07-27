#include "headers/minimax.hh"

MiniMax::MiniMax(BitBoard* board) {
    this->board = board;
    evaluators[(int) Color::White] = new Evaluator(Color::White);
    evaluators[(int) Color::Black] = new Evaluator(Color::Black);
    moveGens[(int) Color::White] = new MoveGen(Color::White);
    moveGens[(int) Color::Black] = new MoveGen(Color::Black);
}

Move MiniMax::GetBestMove(int depth) {
    Move* move = new Move(MoveType::BPromotionCapture, Color::None);
    int alpha = -(int) PieceValue::Inf;
    int beta = (int) PieceValue::Inf;
    U64 attacks = 0;
    Move* moves = (Move*) calloc(256, sizeof(Move));
    int moveCount = moveGens[(int) board->GetColor()]->GetAllMoves(moves, *board, &attacks);
    free(moves);
    NegaMax(true, move, depth, alpha, beta, attacks);
    return *move;
}

std::random_device rd;
std::mt19937 g(rd());

int MiniMax::NegaMax(bool original, Move* bestMove, int depth, int alpha, int beta, U64 attackedSquares) {
    if (depth == 0)
        return evaluators[(int) board->originalColor]->Evalute(*board);
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
        
#ifdef DEBUGGING
        if (original)
            printf("%s : %d\n", moves[i].ToString().c_str(), tempScore);
#endif

        if (tempScore >= score ) {
            // As move gen generates psuedo legal moves, check whether it is legal
            // Only do so on possible return moves, as otherwise it impacts performance greatly 
            if (original && moveGens[(int) Utilities::GetOppositeColor(board->GetColor())]->IsKingSafe(*board)){
                (*bestMove) = moves[i];
                score = tempScore;
            } else
                score = tempScore;
        }

        board->UndoMove(moves[i]);

        if (alpha >= beta)
            return beta;
        if (score > alpha)
            alpha = score;
    }
    free(moves);
    return score;
}
