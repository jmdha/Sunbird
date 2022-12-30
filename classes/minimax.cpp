#include "headers/minimax.hh"

Move MiniMax::GetBestMove(int depth) {
    return NegaMax(depth);
}

Move MiniMax::NegaMax(int depth) {
    std::array<Move, MAXMOVECOUNT> moves{};

    U64 attackedSquares = moveGens[(int) Utilities::GetOppositeColor(board->GetColor())].GetAttackSquares(board);
    U8 moveCount = moveGens[(int) board->GetColor()].GetAllMoves(&moves, board, attackedSquares);

    Move bestMove = Move(MoveType::Quiet);
    int bestScore = -(int) PieceValue::Inf;

    for (int i = 0; i < moveCount; i++) {
        //printf("%s\n", moves[i].ToString().c_str());
        board->DoMove(moves[i]);
        int score = -NegaMax(depth - 1, -(int) PieceValue::Inf, -bestScore);
        board->UndoMove(moves[i]);
        //printf("%s %d\n", moves[i].ToString().c_str(), score);

        if (score > bestScore) {
            bestScore = score;
            bestMove = moves[i];
        }
    }

    return bestMove;
}

int MiniMax::NegaMax(int depth, int alpha, int beta) {
    if (depth == 0)
        return Quiesce(alpha, beta);
    
    std::array<Move, MAXMOVECOUNT> moves{};
    U64 attackedSquares = moveGens[(int) Utilities::GetOppositeColor(board->GetColor())].GetAttackSquares(board);
    int moveCount = moveGens[(int) board->GetColor()].GetAllMoves(&moves, board, attackedSquares);

    if (moveCount == 0)
        return std::min(evaluator.EvaluateNoMoves(*board, moveGens[(int) board->GetColor()].IsKingSafe(board)), beta);
    if (board->IsThreefoldRep())
        return std::min(0, beta);

    for (int moveType = 0; moveType < 2; moveType++) 
        for (int i = 0; i < moveCount; i++) {
            // Do capture before quiet
            if (moves.at(i).IsCapture()) {
                if (moveType == 1)
                    continue;
            } else {
                if (moveType == 0)
                    continue;
            }

            //for (int i = 0; i < (8 - depth); i++)
                //printf("\t");
            //printf("%s\n", moves[i].ToString().c_str());

            board->DoMove(moves[i]);
            int score = -NegaMax(depth - 1, -beta, -alpha);
            board->UndoMove(moves[i]);

            if(score >= beta)
                return beta;   //  fail hard beta-cutoff
            if(score > alpha)
                alpha = score; // alpha acts like max in MiniMax
        }

    return alpha;
}

int MiniMax::Quiesce(int alpha, int beta) {
    int standPat = evaluator.Evaluate(*board);
    if (standPat >= beta)
        return beta;
    if (alpha < standPat)
        alpha = standPat;

    std::array<Move, MAXMOVECOUNT> moves{};
    U64 attackedSquares = moveGens[(int) Utilities::GetOppositeColor(board->GetColor())].GetAttackSquares(board);
    int moveCount = moveGens[(int) board->GetColor()].GetAllMoves(&moves, board, attackedSquares);    

    if (moveCount == 0)
        return std::min(evaluator.EvaluateNoMoves(*board, moveGens[(int) board->GetColor()].IsKingSafe(board)), beta);
    if (board->IsThreefoldRep())
        return std::min(0, beta);

    for (int i = 0; i < moveCount; i++) {
        if (!moves[i].IsCapture())
            continue;

        board->DoMove(moves[i]);
        int score = -Quiesce(-beta, -alpha);
        board->UndoMove(moves[i]);

        if(score >= beta) 
            return beta;   //  fail hard beta-cutoff
        if(score > alpha)
            alpha = score; // alpha acts like max in MiniMax
    }
    return alpha;
}
