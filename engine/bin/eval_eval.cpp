#include "chess/internal/utilities.hpp"
#include "engine/evaluation.hpp"
#include <CTPL/ctpl_stl.h>
#include <chess/import.hpp>
#include <engine/internal/positions.hpp>
#include <engine/internal/values.hpp>
#include <engine/negamax.hpp>
#include <string>

constexpr int POS_COUNT = 128;

constexpr int THREAD_LIMIT = 18;
ctpl::thread_pool pool(std::min(THREAD_LIMIT, (int)std::thread::hardware_concurrency()));

std::array<size_t, 2> wins{0};
std::mutex progressMutex;

void AddWin(int winner) {
    while (progressMutex.try_lock()) {
    }
    wins[winner]++;
    printf("----------\n");
    printf("Old: %zu\n", wins[0]);
    printf("New: %zu\n", wins[1]);
    printf("----------\n");
    progressMutex.unlock();
}

using namespace Chess;
using namespace Chess::Engine;

int OldEval(const Board &board) { return Evaluation::Eval(board); }

int NewEval(const Board &board) {
    int value;
    if (Utilities::PopCount(board.GetPiecePos(PieceType::Pawn)) < 10 ||
        Utilities::PopCount(board.GetPiecePos(PieceType::Knight) |
                            board.GetPiecePos(PieceType::Bishop)) < 3 ||
        Utilities::PopCount(board.GetPiecePos(PieceType::Queen) == 0))
        value = Evaluation::EvalMaterial(board);
    else
        value = Evaluation::Eval(board);
    return (board.GetColor() == Color::White) ? value : -value;
}

const std::function<int(const Board &)> oldEval = OldEval;
const std::function<int(const Board &)> newEval = NewEval;

int main() {
    printf("Running eval of new eval method\n");

    std::vector<std::string> positions = Positions::GetPositions(POS_COUNT);
    size_t winCount[2]{0};
    for (const auto pos : positions) {
        for (int i = 0; i < 2; i++) {
            pool.push([i, pos](int) {
                Board board = Import::FEN(pos);
                for (int m_i = 0; m_i < 256; m_i++) {
                    if (board.GetColor() == (Color)i) {
                        Negamax::SetEvalFunc(oldEval);
                    } else {
                        Negamax::SetEvalFunc(newEval);
                    }
                    auto move = Negamax::GetBestMoveTime(board, 100);

                    if (std::holds_alternative<Move>(move)) {
                        board.DoMove(std::get<Move>(move));
                    } else {
                        const auto result = std::get<Negamax::AlternativeResult>(move);
                        if (result == Negamax::AlternativeResult::Checkmate) {
                            if (board.GetColor() != (Color)i)
                                AddWin(0);
                            else
                                AddWin(1);
                        }
                        break;
                    }
                };
            });
        }
    }

    pool.stop(true);
}
