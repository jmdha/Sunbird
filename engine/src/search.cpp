#include <chess/export.hpp>
#include <chess/internal/constants.hpp>
#include <chess/move_gen.hpp>
#include <chrono>
#include <csetjmp>
#include <cstdlib>
#include <cstring>
#include <engine/evaluation.hpp>
#include <engine/internal/move_ordering.hpp>
#include <engine/internal/pv.hpp>
#include <engine/internal/tt.hpp>
#include <engine/internal/values.hpp>
#include <engine/search.hpp>
#include <iostream>
#include <optional>
#include <stdexcept>

namespace Chess::Engine::Search {
namespace {
std::optional<AlternativeResult> IsTerminal(const Position &pos) {
    MoveList moves =
        MoveGen::GenerateMoves<MoveGen::GenType::All>(pos, pos.GetTurn());
    if (moves.empty())
        return Evaluation::Eval(pos) == 0 ? AlternativeResult::Draw
                                          : AlternativeResult::Checkmate;
    else
        return {};
}
PV ExtractPV(Board board) {
    int ply = board.Ply();
    std::vector<Move> moves;
    while (moves.size() < 8) {
        auto move = TT::ProbeMove(board.Pos().GetHash());
        if (move != nullptr && move->GetValue() != 0) {
            moves.push_back(*move);
            board.MakeMove(*move);
        } else {
            break;
        }
    }
    return PV(ply, moves);
}
} // namespace

std::variant<Move, AlternativeResult> GetBestMove(Board &board, int depth) {
    if (auto terminal = IsTerminal(board.Pos()); terminal.has_value())
        return terminal.value();

    Internal::Negamax(board, -Values::INF, Values::INF, depth, PV());
    return ExtractPV(board).moves[0];
}

std::variant<Move, AlternativeResult> GetBestMoveTime(Board &board,
                                                      int timeLimit) {
    if (auto terminal = IsTerminal(board.Pos()); terminal.has_value())
        return terminal.value();
    if (auto moves = MoveGen::GenerateMoves(board.Pos()); moves.size() == 1)
        return moves[0];

    std::cout << "info fen " << Export::FEN(board.Pos()) << '\n';

    std::jmp_buf exitBuffer;
    SearchLimit limit = SearchLimit(exitBuffer, timeLimit);
    PV pv;

    for (int depth = 1; depth < 256 && !setjmp(exitBuffer); depth++) {
        Board tempBoard = board;
        auto t0 = std::chrono::steady_clock::now();
        PV tempPV;
        int score = Internal::Negamax(tempBoard, -Values::INF, Values::INF,
                                       depth, pv, &limit);
        auto t1 = std::chrono::steady_clock::now();
        size_t t =
            std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0)
                .count();
        timeLimit -= t;
        std::cout << "info";
        printf(" depth %2d", depth);
        printf(" score cp %6d", score);
        printf(" time %5zu ms", t);
        auto nodes = tempBoard.MoveCount() - board.MoveCount();
        printf(" nodes %9zu", nodes);
        auto nps = nodes / std::max((size_t)1, (t / 1000));
        printf(" nps %8zu", nps);
        printf(" hashfull %4zu", TT::HashFull());
        bool errorOccured = true;
        if (auto tempPV = ExtractPV(board); !tempPV.moves.empty()) {
            pv = tempPV; 
            errorOccured = false;
        }
        std::cout << " pv ";
        for (int i = 0; i < pv.moves.size(); i++)
            std::cout << pv.moves[i].ToString() << " ";
        std::cout << '\n';
        if (std::abs(score) == Values::INF || errorOccured)
            break;
    }
    return pv.moves[0];
}
} // namespace Chess::Engine::Search
