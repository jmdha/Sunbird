#include "engine/evaluation.hpp"
#include "engine/internal/values.hpp"
#include <chess/internal/constants.hpp>
#include <chess/move_gen.hpp>
#include <chrono>
#include <engine/negamax.hpp>
#include <optional>

namespace Chess::Engine::Negamax {
namespace {
inline bool AB(int score, int &alpha, int beta) {
    if (score >= beta)
        return true; //  fail hard beta-cutoff
    if (score > alpha)
        alpha = score; // alpha acts like max in MiniMax
    return false;
}

int Quiesce(Board &board, int alpha, int beta) {
    int standPat = Evaluation::Eval(board);
    if (AB(standPat, alpha, beta))
        return beta;

    MoveList moves = MoveGen::GenerateMoves<MoveGen::GenType::Attack>(board, board.GetColor());
    if (board.GetPly() > 150)
        return Evaluation::EvalNoMove(board.IsKingSafe());


    for (auto move : moves) {
        board.DoMove(move);
        int score = -Quiesce(board, -beta, -alpha);
        board.UndoMove(move);
        if (AB(score, alpha, beta))
            return beta;
    }

    return alpha;
}

int Negamax(Board &board, int depth, int alpha, int beta) {
    if (depth == 0)
        return Quiesce(board, alpha, beta);

    MoveList moves = MoveGen::GenerateMoves<MoveGen::GenType::All>(board, board.GetColor());
    if (moves.empty() || board.IsThreefoldRep() || board.GetPly() > 150)
        return Evaluation::EvalNoMove(board.IsKingSafe());

    for (auto move : moves) {
        board.DoMove(move);
        int score = -Negamax(board, depth - 1, -beta, -alpha);
        board.UndoMove(move);
        if (AB(score, alpha, beta))
            return beta;
    }

    return alpha;
}
} // namespace

std::variant<Move, AlternativeResult> GetBestMove(Board &board, int depth) {
    MoveList moves = MoveGen::GenerateMoves<MoveGen::GenType::All>(board, board.GetColor());
    std::array<int, MAXMOVECOUNT> scores{0};

    int workingDepth = 1;
    do {
        for (int i = 0; i < moves.size(); i++) {
            Move &move = moves[i];
            board.DoMove(move);
            const int score =
                -Negamax(board, workingDepth, -MaterialValue::Inf, MaterialValue::Inf);
            board.UndoMove(move);
            scores[i] = score;
        }
        moves.sort(scores);
        if (std::any_of(scores.begin(), &scores[moves.size()],
                        [](int val) { return val == MaterialValue::Inf; }))
            break;
    } while (workingDepth++ < depth);

    return moves[0];
}

MoveList GetOrderdMoves(Board &board, int timeLimit) {
    MoveList moves = MoveGen::GenerateMoves<MoveGen::GenType::All>(board, board.GetColor());
    if (moves.empty())
        return moves;
    std::array<int, MAXMOVECOUNT> scores{0};

    U64 totalTime = 0;

    int workingDepth = 1;
    do {
        for (int i = 0; i < moves.size(); i++) {
            auto t0 = std::chrono::steady_clock::now();
            Move &move = moves[i];
            board.DoMove(move);
            const int score =
                -Negamax(board, workingDepth, -MaterialValue::Inf, MaterialValue::Inf);
            board.UndoMove(move);
            scores[i] = score;
            auto t1 = std::chrono::steady_clock::now();
            U64 time = (U64)std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
            totalTime += time;
            if (workingDepth > 1 &&
                (totalTime + time > timeLimit ||
                 (i == moves.size() - 1 && totalTime + 20 * time > timeLimit))) {
                totalTime = 999999;
                break;
            }
        }
        moves.sort(scores);
        if (std::any_of(scores.begin(), &scores[moves.size()],
                        [](int val) { return val == MaterialValue::Inf; }))
            break;
    } while (workingDepth++ < 1000 && totalTime < timeLimit);

    return moves;
}

std::variant<Move, AlternativeResult> GetBestMoveTime(Board &board, int timeLimit) {
    MoveList moves = GetOrderdMoves(board, timeLimit);
    if (moves.empty())
        return (Evaluation::EvalNoMove(board.IsKingSafe()) == 0) ? AlternativeResult::Draw
                                                                 : AlternativeResult::Checkmate;

    return moves[0];
}

} // namespace Chess::Engine::Negamax
