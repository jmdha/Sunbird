#include <chrono>
#include "headers/minimax.hh"

Move MiniMax::GetBestMove(int depth) {
    U64 timeLimit = 2000;
    std::vector<U64> timeUsed;
    int workingDepth = (depth == -1) ? 1 : depth;
    MoveVals moveScores;
    board->SetOriginalColor(board->GetColor());

    do {
        auto t0 = std::chrono::steady_clock::now();
        moveScores = NegaMax(workingDepth++, moveScores);
        auto t1 = std::chrono::steady_clock::now();
        timeUsed.push_back((U64) std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count());
        moveScores.Sort();
        if (moveScores.scores.at(0) == (U64) PieceValue::Inf)
            break;
        //printf("Checking depth: %d, time used %llu ms\n", workingDepth, timeUsed.at(timeUsed.size() - 1));
    } while (depth == -1 &&
            // If iterative it will do what's below
            (workingDepth < 1000/*Relevant to late game where each iteration is 0ms*/ && (
                 timeUsed.size() <= 2 ||
                 timeUsed.at(timeUsed.size() - 1) < 10 ||
                 timeUsed.at(timeUsed.size() - 2) < 10 ||
                 timeUsed.at(timeUsed.size() - 1) * timeUsed.at(timeUsed.size() - 1) / timeUsed.at(timeUsed.size() - 2) < timeLimit)));

    return moveScores.moves.at(0);
}
MiniMax::MoveVals MiniMax::NegaMax(int depth, MoveVals moveVals) {
    U64 attackedSquares = board->GenerateAttackSquares(board->GetOppColor());
    if (moveVals.moveCount == -1)
        moveVals.moveCount = moveGens[(int) board->GetColor()].GetAllMoves(&moveVals.moves, board, attackedSquares);

    for (int i = 0; i < moveVals.moveCount; ++i) {
        board->DoMove(moveVals.moves[i]);
        moveVals.scores.at(i) = -NegaMax(depth - 1, -(int) PieceValue::Inf, (int) PieceValue::Inf);
        board->UndoMove(moveVals.moves[i]);
        //printf("%s %d\n", moveVals.moves.at(i).ToString().c_str(), moveVals.scores.at(i));
    }

    return moveVals;
}

int MiniMax::NegaMax(int depth, int alpha, int beta) {
    if (depth == 0)
        return Quiesce(alpha, beta);
    
    std::array<Move, MAXMOVECOUNT> moves;
    U64 attackedSquares = board->GenerateAttackSquares(board->GetOppColor());
    int moveCount = moveGens[(int) board->GetColor()].GetAllMoves(&moves, board, attackedSquares);
    ReOrderMoves(moves, moveCount);

    if (moveCount == 0)
        return evaluator.EvaluateNoMoves(*board, board->IsKingSafe());
    if (board->IsThreefoldRep())
        return 0;

    for (int i = 0; i < moveCount; ++i) {
        //for (int i = 0; i <= (2 - depth); i++)
            //printf(" ");
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

    std::array<Move, MAXMOVECOUNT> moves;
    U64 attackedSquares = board->GenerateAttackSquares(board->GetOppColor());
    int moveCount = moveGens[(int) board->GetColor()].GetAttackMoves(&moves, board, attackedSquares);
    ReOrderMoves(moves, moveCount);

    for (int i = 0; i < moveCount; ++i) {
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
