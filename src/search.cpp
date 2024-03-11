#include "search.hpp"
#include "evaluation.hpp"
#include "move_gen.hpp"
#include "move_ordering.hpp"
#include "tt.hpp"
#include "values.hpp"
#include <chrono>
#include <cmath>
#include <cstring>
#include <iostream>
#include <stdexcept>
#include <unistd.h>

std::size_t Time() {
    const auto t = std::chrono::system_clock::now().time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(t).count();
}

MoveList ExtractPV(Board board) {
    MoveList moves;
    while (moves.size() < 8) {
        const Move move = TT::ProbeMove(board.GetHash());
        if (move.IsDefined()) {
            moves << move;
            board.ApplyMove(move);
        } else {
            break;
        }
    }
    return moves;
}

int Quiesce(Board &board, int alpha, int beta) {
    const int standpat = Eval(board);
    if (standpat >= beta) return beta;
    if (standpat > alpha) alpha = standpat;

    MoveList moves;
    GenerateMovesTactical(moves, board, board.Turn());
    OrderMoves(board, moves);
    for (const Move &move : moves) {
        int score = -Values::INF;
        board.ApplyMove(move);
        if (board.IsKingSafe(!board.Turn())) score = -Quiesce(board, -beta, -alpha);
        board.UndoMove(move);
        if (score >= beta) return beta;
        if (score > alpha) alpha = score;
    }
    return alpha;
}

int Negamax(
    Board &board, int alpha, int beta, int depth, int search_depth, const MoveList &pv,
    std::size_t time_limit
) {
    if (board.IsThreefold()) return 0;
    if (depth > 3 && Time() > time_limit) throw std::logic_error("out of time");
    if (depth <= 0) return Quiesce(board, alpha, beta);

    const Hash hash = board.GetHash();
    auto tt         = TT::Probe(hash, depth, search_depth, alpha, beta);
    if (tt.score != TT::ProbeFail) return tt.score;
    int bound = TT::ProbeUpper;

    int legal_moves = 0;
    Move bm;
    MoveList moves;
    GenerateMovesAll(moves, board, board.Turn());
    OrderMoves(board, moves, tt.move, pv[search_depth]);
    for (std::size_t i = 0; i < moves.size(); i++) {
        const Move &move = moves[i];
        int score        = -Values::INF;
        board.ApplyMove(move);
        if (board.IsKingSafe(!board.Turn())) {
            score =
                -Negamax(board, -alpha - 1, -alpha, depth - 1, search_depth + 1, pv, time_limit);
            if (score > alpha && score < beta)
                score = -Negamax(board, -beta, -alpha, depth - 1, search_depth + 1, pv, time_limit);
            legal_moves++;
        }
        board.UndoMove(move);
        if (score >= beta) {
            TT::StoreEval(hash, depth, search_depth, beta, TT::ProbeLower, move);
            return beta;
        }
        if (score > alpha) {
            bound = TT::ProbeExact;
            alpha = score;
            bm    = move;
        }
    }
    if (legal_moves == 0) {
        if (!board.IsKingSafe(board.Turn()))
            return -Values::CHECKMATE;
        else
            return 0;
    }
    TT::StoreEval(hash, depth, search_depth, alpha, bound, bm);
    return alpha;
}

Move Search(const Board &c_board, std::size_t time) {
    // If Onlt a single legal move exists, return that
    if (auto moves = GenerateLegalMoves(c_board, c_board.Turn()); moves.size() == 1) {
        printf("info only 1 legal move\n");
        return moves[0];
    }
    const size_t time_limit = Time() + time;
    Board board             = c_board;

    size_t elapsed = 0;

    MoveList pv;
    int alpha       = -Values::INF;
    int beta        = Values::INF;
    int delta_alpha = WINDOW_SIZE;
    int delta_beta  = WINDOW_SIZE;
    for (std::size_t depth = 1; depth < MAX_PLY; depth++) {
        const std::size_t t0 = Time();
        int score;
        try {
            score = Negamax(board, alpha, beta, depth, 0, pv, time_limit);
        } catch (...) {
            printf("info out of time\n");
            break; // This "should" only happen if search ran out of time
        }
        const std::size_t t = Time() - t0;
        elapsed += t;

        if (score <= alpha) {
            delta_alpha = std::pow(delta_alpha, 2);
            alpha       = score - delta_alpha;
            depth--;
            continue;
        } else if (score >= beta) {
            delta_beta = std::pow(delta_beta, 2);
            beta       = score + delta_beta;
            depth--;
            continue;
        } else if (std::abs(score) == Values::CHECKMATE) {
            printf("info found forced checkmate\n");
            break;
        }
        delta_alpha = WINDOW_SIZE;
        delta_beta  = WINDOW_SIZE;
        alpha       = score - WINDOW_SIZE;
        beta        = score + WINDOW_SIZE;
        pv          = ExtractPV(board);

        printf(
            "info depth %zu score cp %d time %zu nodes %zu nps %zu hashfull %zu pv", depth, score,
            elapsed, board.MoveCount(),
            board.MoveCount() * 1000 / std::max((std::size_t)1, elapsed), TT::HashFull()
        );
        for (std::size_t i = 0; i < std::min((std::size_t)4, pv.size()); i++)
            printf(" %s", pv[i].Export().c_str());
        printf("\n");
        std::flush(std::cout);
    }
    if (pv.size() == 0) pv = ExtractPV(board);
    return pv[0];
}
