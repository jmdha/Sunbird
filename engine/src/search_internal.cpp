#include "engine/internal/move_ordering.hpp"
#include <chess/internal/move_list.hpp>
#include <chess/move_gen.hpp>
#include <cstring>
#include <engine/evaluation.hpp>
#include <engine/search.hpp>

using namespace Chess::MoveGen;

namespace Chess::Engine::Search::Internal {
namespace {
bool AB(int score, int &alpha, int beta) {
    if (score >= beta)
        return true;
    if (score > alpha)
        alpha = score;
    return false;
}
} // namespace
int Quiesce(Board &board, int alpha, int beta, PV &ppv) {
    int standPat = Evaluation::Eval(board.Pos());
    if (AB(standPat, alpha, beta))
        return beta;

    MoveList moves = GenerateMoves<GenType::Attack>(board.Pos(), board.Pos().GetTurn());
    MoveOrdering::All(board, ppv, moves);
    for (auto move : moves) {
        board.MakeMove(move);
        int score = -Quiesce(board, -beta, -alpha, ppv);
        board.UndoMove();
        if (AB(score, alpha, beta))
            return beta;
    }

    return alpha;
}

int Negamax(Board &board, int alpha, int beta, int depth, PV &pv, PV &ppv, SearchLimit *limit) {
    if (limit != nullptr && limit->Reached())
        limit->Exit();
    if (board.IsThreefoldRepetition())
        return 0;
    if (depth == 0)
        return Quiesce(board, alpha, beta, ppv);

    MoveList moves = GenerateMoves<GenType::All>(board.Pos(), board.Pos().GetTurn());
    if (moves.empty())
        return Evaluation::EvalNoMove(board.Pos());

    MoveOrdering::All(board, ppv, moves);
    for (auto move : moves) {
        PV moveLine;
        board.MakeMove(move);
        int score = -Negamax(board, -beta, -alpha, depth - 1, moveLine, ppv, limit);
        board.UndoMove();
        if (alpha >= beta)
            return beta;
        if (score > alpha) {
            alpha = score;
            pv._moves[0] = move;
            std::memmove(&pv._moves[1], &moveLine._moves[0], moveLine._count * sizeof(Move));
            pv._count = moveLine._count + 1;
        }
    }

    return alpha;
}
} // namespace Chess::Engine::Search::Internal
