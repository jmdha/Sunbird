#include "engine/minimax.hpp"

#include "engine/internal/openings.hpp"
#include <chrono>

// TODO: Implement dynamic time limit
Move MiniMax::GetBestMove(Board &board, int depth) {
    if (board.GetPly() > 150)
        return Move(MoveType::SPECIAL_DRAW);

    U64 timeLimit = 1000;
    std::vector<U64> timeUsed;
    U64 totalTime = 0;
    int workingDepth = (depth == -1) ? 1 : depth;

    Move bestMove;
    int bestScore;
    do {
        auto result = NegaMax(board, workingDepth++, timeLimit, &totalTime);
        bestMove = result.first;
        bestScore = result.second;
        if (bestScore == (U64)PieceValue::Inf)
            break;
    } while (depth == -1 &&
             // If iterative it will do what's below
             (workingDepth < 1000 /*Relevant to late game where each iteration is 0ms*/ &&
              (totalTime < timeLimit)));

    return bestMove;
}
std::pair<Move, int> MiniMax::NegaMax(Board &board, int depth, U64 timeLimit, U64 *timeUsed) {
    MoveList moves = MoveGen::GenerateMoves<MoveGen::GenType::All>(board);

    Move bestMove;
    std::optional<int> bestScore;
    for (auto move : moves) {
        auto t0 = std::chrono::steady_clock::now();
        board.DoMove(move);
        int score = -NegaMax(board, depth - 1, -(int)PieceValue::Inf, (int)PieceValue::Inf);
        board.UndoMove(move);
        if (!bestScore.has_value() || score > bestScore) {
            bestMove = move;
            bestScore = score;
        }
        auto t1 = std::chrono::steady_clock::now();
        (*timeUsed) += (U64)std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
        if (*timeUsed > timeLimit)
            break;
    }

    return std::make_pair(bestMove, bestScore.value());
}

int MiniMax::NegaMax(Board &board, int depth, int alpha, int beta) {
    if (depth == 0)
        return Quiesce(board, alpha, beta);

    MoveList moves = MoveGen::GenerateMoves<MoveGen::GenType::All>(board);

    if (moves.size() == 0)
        return evaluator.EvaluateNoMoves(board.IsKingSafe());
    if (board.IsThreefoldRep())
        return 0;

    for (auto move : moves) {
        board.DoMove(move);
        int score = -NegaMax(board, depth - 1, -beta, -alpha);
        board.UndoMove(move);

        if (score >= beta)
            return beta; //  fail hard beta-cutoff
        if (score > alpha) {
            alpha = score; // alpha acts like max in MiniMax
        }
    }

    return alpha;
}

int MiniMax::Quiesce(Board &board, int alpha, int beta) {
    MoveList moves = MoveGen::GenerateMoves<MoveGen::GenType::All>(board);

    if (moves.empty() && !board.IsKingSafe()) {
        moves = MoveGen::GenerateMoves<MoveGen::GenType::All>(board);
        if (moves.empty())
            return evaluator.EvaluateNoMoves(false);
    }

    int standPat = evaluator.Evaluate(board);
    if (standPat >= beta)
        return beta;
    if (alpha < standPat)
        alpha = standPat;

    for (auto move : moves) {
        board.DoMove(move);
        int score = -Quiesce(board, -beta, -alpha);
        board.UndoMove(move);

        if (score >= beta)
            return beta; //  fail hard beta-cutoff
        if (score > alpha)
            alpha = score; // alpha acts like max in MiniMax
    }

    return alpha;
}
