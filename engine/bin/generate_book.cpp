#include "chess/export.hpp"
#include "chess/internal/constants.hpp"
#include "chess/internal/move_list.hpp"
#include <chrono>
#include <condition_variable>
#include <ctime>
#include <fstream>
#include <functional>
#include <future>
#include <iostream>

#include <chess/board.hpp>
#include <chess/import.hpp>
#include <chess/move_gen.hpp>
#include <engine/negamax.hpp>
#include <mutex>
#include <queue>
#include <thread>
#include <unordered_set>

#include <indicators/indicators.hpp>
#include <CTPL/ctpl_stl.h>

using namespace Chess;

constexpr int SEARCH_DEPTH = 4;
constexpr int SEARCH_TIME = 10000;
constexpr int THREAD_LIMIT = 18;
ctpl::thread_pool pool(std::min(THREAD_LIMIT, (int)std::thread::hardware_concurrency()));

U64 searchSpace = 0;

U64 currentSearch = 0;
indicators::BlockProgressBar *bar = nullptr;
std::mutex progressMutex;

std::unordered_set<U64> checkedStates;
std::ofstream stream;

void EstimateSearchSpace(Board &board, int depth, double placement) {
    if (depth == 0)
        return;

    MoveList moves = MoveGen::GenerateMoves<MoveGen::GenType::All>(board, board.GetColor());
    searchSpace++;

    for (int i = 0; i < moves.size() * (1 - placement); i++) {
        board.DoMove(moves[i]);
        if (!checkedStates.contains(board.GetHash()))
            EstimateSearchSpace(board, depth - 1, (double)i / (double)moves.size());
        board.UndoMove(moves[i]);
    }
}

void UpdateProgress(Board &board, Move move) {
    while (!progressMutex.try_lock()) {
    }
    if (currentSearch++ > searchSpace)
        searchSpace = currentSearch;
    bar->set_progress((double)currentSearch / (double)searchSpace * 100);

    // Recreate move to avoid weird shit
    if (!move.IsCapture())
        move = Move(move.GetType(), move.GetFrom(), move.GetTo());
    else
        move = Move(move.GetType(), move.GetFrom(), move.GetTo(), move.GetCapturedPiece());
    stream << '{' << board.GetHash() << "LLU," << move.GetValue() << "},//" << move.ToString()
           << ',' << Export::FEN(board) << '\n';
    progressMutex.unlock();
}

void Search(int id, Board &board, int depth, double placement) {
    if (depth == 0)
        return;

    MoveList moves = Engine::Negamax::GetOrderdMoves(board, SEARCH_TIME);
    UpdateProgress(board, moves[0]);

    for (int i = 0; i < moves.size() * (1 - placement); i++) {
        board.DoMove(moves[i]);
        if (!checkedStates.contains(board.GetHash())) {
            Board tempBoard = board;

            pool.push(Search, tempBoard, depth - 1, (double)i/(double)moves.size());
        }
        board.UndoMove(moves[i]);
    }
}

int main(int, char *argv[]) {
    Board board;
    board.Initialize();

    std::cout << "Starting opening book generation\n";
    std::cout << "Estimating search space...\n";
    EstimateSearchSpace(board, SEARCH_DEPTH, 0);
    std::cout << "Estimated search space: " << searchSpace << '\n';
    std::cout << "Generating opening book...\n";

    stream.open("opening_book.csv");
    indicators::show_console_cursor(false);
    using namespace indicators;
    BlockProgressBar tempBar{option::BarWidth{80},
                             option::Start{"["},
                             option::End{"]"},
                             option::ForegroundColor{indicators::Color::white},
                             option::ShowElapsedTime{true},
                             option::ShowRemainingTime{true}};
    bar = &tempBar;

    Search(0, board, SEARCH_DEPTH, 0);
    pool.stop(true);

    tempBar.mark_as_completed();
    indicators::show_console_cursor(true);
    stream.close();
    std::cout << "Actual search space: " << currentSearch << '\n';

    return 0;
}
