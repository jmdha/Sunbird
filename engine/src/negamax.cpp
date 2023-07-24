#include <chess/internal/constants.hpp>
#include <chess/move_gen.hpp>
#include <engine/evaluation.hpp>
#include <engine/negamax.hpp>
#include <optional>

namespace Chess::Engine::Negamax {
namespace {
int Quiesce(Board &board, int alpha, int beta) {
    MoveList moves = MoveGen::GenerateMoves<MoveGen::GenType::Attack>(board, board.GetColor());

    if (moves.empty() && !board.IsKingSafe()) {
        moves = MoveGen::GenerateMoves<MoveGen::GenType::All>(board, board.GetColor());
        if (moves.empty())
            return Evaluation::EvalNoMove(false);
    }

    int standPat = Evaluation::Eval(board);
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

int Negamax(Board &board, int depth, int alpha, int beta) {
    if (depth == 0)
        return Quiesce(board, alpha, beta);

    MoveList moves = MoveGen::GenerateMoves<MoveGen::GenType::All>(board, board.GetColor());

    if (moves.size() == 0)
        return Evaluation::EvalNoMove(board.IsKingSafe());
    if (board.IsThreefoldRep())
        return 0;

    for (auto move : moves) {
        board.DoMove(move);
        int score = -Negamax(board, depth - 1, -beta, -alpha);
        board.UndoMove(move);

        if (score >= beta)
            return beta; //  fail hard beta-cutoff
        if (score > alpha) {
            alpha = score; // alpha acts like max in MiniMax
        }
    }

    return alpha;
}

std::pair<Move, int> Negamax(Board &board, int depth) {
    std::optional<std::pair<Move, int>> bestMove;

    const MoveList moves = MoveGen::GenerateMoves<MoveGen::GenType::All>(board, board.GetColor());
    for (auto move : moves) {
        board.DoMove(move);
        int score = -Negamax(board, depth - 1, -MaterialValue::Inf, MaterialValue::Inf);
        board.UndoMove(move);
        if (!bestMove.has_value() || score > bestMove.value().second)
            bestMove = {move, score};
    }

    return bestMove.value();
}
} // namespace
Move GetBestMove(Board &board, int depth) {
    std::pair<Move, int> bestMove;

    int workingDepth = 1;
    do {
        bestMove = Negamax(board, depth);
        if (bestMove.second == MaterialValue::Inf)
            break;
    } while (workingDepth++ < depth);

    return bestMove.first;
}
} // namespace Chess::Engine::Negamax
