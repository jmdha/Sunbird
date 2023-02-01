#include <chrono>
#include "minimax.hh"
#include "headers/openings.hh"

Move MiniMax::GetBestMove(int depth) {
    Move openingMove;
    if (OpeningBook::GetMove(board->GetHash(), &openingMove))
        return openingMove;
    U64 timeLimit = 1000;
    std::vector<U64> timeUsed;
    U64 totalTime = 0;
    int workingDepth = (depth == -1) ? 1 : depth;
    MoveVals moveScores;
    board->SetOriginalColor(board->GetColor());

    do {
        //printf("Depth %d\n", workingDepth);
        moveScores = NegaMax(workingDepth++, timeLimit, &totalTime, moveScores);
        if (moveScores.scores.at(0) == (U64) PieceValue::Inf)
            break;
        //printf("Checking depth: %d, time used %llu ms\n", workingDepth, totalTime);
    } while (depth == -1 &&
            // If iterative it will do what's below
            (workingDepth < 1000/*Relevant to late game where each iteration is 0ms*/ && (
                 totalTime < timeLimit)));

    return moveScores.moves.at(0);
}
MiniMax::MoveVals MiniMax::NegaMax(int depth, U64 timeLimit, U64 *timeUsed, MoveVals moveVals) {
    U64 attackedSquares = board->GenerateAttackSquares(board->GetOppColor());
    if (moveVals.moveCount == -1)
        moveVals.moveCount = moveGens[(int) board->GetColor()].GetAllMoves(&moveVals.moves, board, attackedSquares);

    for (int i = 0; i < moveVals.moveCount; ++i) {
        assert(moveVals.moves.at(i).GetValue() != 0);
        auto t0 = std::chrono::steady_clock::now();
        board->DoMove(moveVals.moves[i]);
        moveVals.scores.at(i) = -NegaMax(depth - 1, -(int) PieceValue::Inf, (int) PieceValue::Inf);
        board->UndoMove(moveVals.moves[i]);
        auto t1 = std::chrono::steady_clock::now();
        (*timeUsed) += (U64) std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
        if (*timeUsed > timeLimit)
            break;
        //printf("%s %d\n", moveVals.moves.at(i).ToString().c_str(), moveVals.scores.at(i));
    }

    moveVals.Sort();

    return moveVals;
}

int MiniMax::NegaMax(int depth, int alpha, int beta) {
    auto ttNode = tt.Retrieve(board->GetHash());
    if (ttNode != nullptr) {
        if (ttNode->depth >= depth) {
            if (ttNode->col == board->GetColor())
                return ttNode->eval;
            else
                return -ttNode->eval;
        }

    }

    if (depth == 0)
        return Quiesce(alpha, beta);

    std::array<Move, MAXMOVECOUNT> moves;
    U64 attackedSquares = board->GenerateAttackSquares(board->GetOppColor());
    int moveCount = moveGens[(int) board->GetColor()].GetAllMoves(&moves, board, attackedSquares);

    if (moveCount == 0)
        return evaluator.EvaluateNoMoves(*board, board->IsKingSafe());
    if (board->IsThreefoldRep())
        return 0;

    if (ttNode != nullptr) {
        if (ttNode->bestMoveIndex < moveCount && moves.at(ttNode->bestMoveIndex).GetValue() == ttNode->bestMove.GetValue()) {
            Move tempMove = moves.at(0);
            moves.at(0) = moves.at(ttNode->bestMoveIndex);
            moves.at(ttNode->bestMoveIndex) = tempMove;
        }
    }

    U8 bestMoveIndex = 0;
    for (int i = 0; i < moveCount; ++i) {
        assert(moves.at(i).GetValue() != 0);
        Move move = moves.at(i);
        PieceType type = board->GetType(move.GetFrom());
        //for (int i = 0; i <= (2 - depth); i++)
            //printf(" ");
        //printf("%s\n", moves[i].ToString().c_str());

        board->DoMove(moves[i]);
        int score = -NegaMax(depth - 1, -beta, -alpha);
        board->UndoMove(moves[i]);

        if(score >= beta)
            return beta;   //  fail hard beta-cutoff
        if(score > alpha) {
            alpha = score; // alpha acts like max in MiniMax
            bestMoveIndex = i;
        }
    }

    tt.Insert(board->GetHash(), depth, TTFlag::Exact, alpha, bestMoveIndex, moves.at(bestMoveIndex), board->GetColor());

    return alpha;
}

int MiniMax::Quiesce(int alpha, int beta) {
    std::array<Move, MAXMOVECOUNT> moves;
    U64 attackedSquares = board->GenerateAttackSquares(board->GetOppColor());
    int moveCount = moveGens[(int) board->GetColor()].GetAttackMoves(&moves, board, attackedSquares);

    if (moveCount == 0 && (attackedSquares & board->GetPiecePos(board->GetColor(), PieceType::King))) {
        moveCount = moveGens[(int) board->GetColor()].GetAllMoves(&moves, board, attackedSquares);
        if (moveCount == 0)
            return evaluator.EvaluateNoMoves(*board, false);
    }

    int standPat = evaluator.Evaluate(*board);
    if (standPat >= beta)
        return beta;
    if (alpha < standPat)
        alpha = standPat;

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
