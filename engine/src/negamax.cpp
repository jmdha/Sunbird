#include <chess/export.hpp>
#include <chess/internal/constants.hpp>
#include <chess/move_gen.hpp>
#include <chrono>
#include <csetjmp>
#include <cstring>
#include <engine/evaluation.hpp>
#include <engine/internal/values.hpp>
#include <engine/negamax.hpp>
#include <iostream>
#include <optional>

namespace Chess::Engine::Negamax {
namespace {
struct PV {
    std::array<Move, MAX_PLY> _moves;
    int _count = 0;
};
struct Limiter {
public:
    Limiter(std::jmp_buf &jmpBuf, size_t time)
        : _jmpBuf(jmpBuf),
          _endTime(std::chrono::steady_clock::now() + std::chrono::milliseconds(time)) {}
    bool Reached() { return _endTime < std::chrono::steady_clock::now(); }
    void Exit() { longjmp(_jmpBuf, 1); }

private:
    std::jmp_buf &_jmpBuf;
    std::chrono::steady_clock::time_point _endTime;
};

std::optional<AlternativeResult> IsTerminal(const Position &pos) {
    MoveList moves = MoveGen::GenerateMoves<MoveGen::GenType::All>(pos, pos.GetTurn());
    if (moves.empty())
        return Evaluation::Eval(pos) == 0 ? AlternativeResult::Draw : AlternativeResult::Checkmate;
    else
        return {};
}

bool AB(int score, int &alpha, int beta) {
    if (score >= beta)
        return true;
    if (score > alpha)
        alpha = score;
    return false;
}

int Quiesce(Board &board, int alpha, int beta) {
    int standPat = Evaluation::Eval(board.Pos());
    if (AB(standPat, alpha, beta))
        return beta;

    MoveList moves =
        MoveGen::GenerateMoves<MoveGen::GenType::Attack>(board.Pos(), board.Pos().GetTurn());
    for (auto move : moves) {
        board.MakeMove(move);
        int score = -Quiesce(board, -beta, -alpha);
        board.UndoMove();
        if (AB(score, alpha, beta))
            return beta;
    }

    return alpha;
}

int Negamax(Board &board, int depth, int alpha, int beta, PV &pv, Limiter *limitter = nullptr) {
    if (limitter != nullptr && limitter->Reached())
        limitter->Exit();

    if (board.IsThreefoldRepetition())
        return 0;

    if (depth == 0)
        return Quiesce(board, alpha, beta);

    MoveList moves =
        MoveGen::GenerateMoves<MoveGen::GenType::All>(board.Pos(), board.Pos().GetTurn());
    if (moves.empty())
        return Evaluation::EvalNoMove(board.Pos().IsKingSafe());

    for (auto move : moves) {
        PV tempPV;
        board.MakeMove(move);
        int score = -Negamax(board, depth - 1, -beta, -alpha, tempPV, limitter);
        board.UndoMove();
        if (score > alpha) {
            alpha = score;
            pv._moves[0] = move;
            std::memmove(&pv._moves[1], &tempPV._moves[0], tempPV._count * sizeof(Move));
            pv._count = tempPV._count + 1;
        }
        if (alpha >= beta)
            break;
    }

    return alpha;
}
} // namespace

std::variant<Move, AlternativeResult> GetBestMove(Board &board, int depth) {
    if (auto terminal = IsTerminal(board.Pos()); terminal.has_value())
        return terminal.value();

    PV pv;
    Negamax(board, depth, -MaterialValue::Inf, MaterialValue::Inf, pv);
    return pv._moves[0];
}

std::variant<Move, AlternativeResult> GetBestMoveTime(Board &board, std::optional<int> timeLimit) {
    if (auto terminal = IsTerminal(board.Pos()); terminal.has_value())
        return terminal.value();

    std::cout << "info fen " << Export::FEN(board.Pos()) << '\n';

    PV pv;

    std::jmp_buf exitBuffer;
    Limiter *limitter = nullptr;
    if (timeLimit.has_value())
        limitter = new Limiter(exitBuffer, timeLimit.value());

    if (setjmp(exitBuffer)) {
        free(limitter);
        return pv._moves[0];
    }

    for (int depth = 1; depth < 1000; depth++) {
        Board tempBoard = board;
        auto t0 = std::chrono::steady_clock::now();
        PV tempPV;
        int score =
            -Negamax(tempBoard, depth, -MaterialValue::Inf, MaterialValue::Inf, tempPV, limitter);
        pv = tempPV;
        auto t1 = std::chrono::steady_clock::now();
        size_t t = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
        std::cout << "info";
        printf(" depth %2d", depth);
        printf(" score %4d", score);
        printf(" time %5zu ms", t);
        auto nodes = tempBoard.MoveCount() - board.MoveCount();
        printf(" nodes %9zu", nodes);
        auto nps = nodes / std::max((size_t)1, t) * 1000;
        printf(" nps %8zu", nps);
        std::cout << " pv ";
        for (int i = 0; i < std::min(pv._count, 8); i++)
            std::cout << pv._moves[i].ToString() << " ";
        std::cout << '\n';
        if (std::abs(score) == MaterialValue::Inf)
            break;
    }
    longjmp(exitBuffer, 1);
}
} // namespace Chess::Engine::Negamax
