#include "engine/internal/move_ordering.hpp"
#include "engine/internal/tt.hpp"
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
int Quiesce(Board &board, int alpha, int beta, const PV &pv) {
    int standPat = Evaluation::Eval(board.Pos());
    if (AB(standPat, alpha, beta))
        return beta;

    MoveList moves = GenerateMoves<GenType::Attack>(board.Pos());
    MoveOrdering::MVVLVA(board, moves);
    MoveOrdering::PVPrioity(board, pv, moves);
    for (auto move : moves) {
        board.MakeMove(move);
        int score = -Quiesce(board, -beta, -alpha, pv);
        board.UndoMove();
        if (AB(score, alpha, beta))
            return beta;
    }

    return alpha;
}

int Negamax(Board &board, int alpha, int beta, int depth, const PV &pv,
            SearchLimit *limit) {
    if (limit != nullptr && depth > 1 && limit->Reached())
        limit->Exit();
    if (board.IsThreefoldRepetition())
        return 0;
    if (depth == 0)
        return Quiesce(board, alpha, beta, pv);

    TT::Flag flag = TT::Flag::Upper;
    auto ttResult = TT::Probe(board.Pos().GetHash(), depth, alpha, beta);
    if (ttResult.found)
        return ttResult.score;

    MoveList moves = GenerateMoves(board.Pos());
    if (moves.empty())
        return Evaluation::EvalNoMove(board.Pos());

    MoveOrdering::All(board, ttResult.move, pv, moves);
    Move bm;
    for (auto move : moves) {
        PV moveLine;
        board.MakeMove(move);
        int value =
            -Negamax(board, -beta, -alpha, depth - 1, pv, limit);
        board.UndoMove();
        if (value > alpha) {
            flag = TT::Flag::Exact;
            alpha = value;
            bm = move;
        }
        if (alpha >= beta) {
            TT::Save(board.Pos().GetHash(), depth, TT::Flag::Lower, beta, move);
            break;
        }
    }

    TT::Save(board.Pos().GetHash(), depth, flag, alpha, bm);
    return alpha;
}
} // namespace Chess::Engine::Search::Internal
