#include "chess/internal/constants.hpp"
#include "chess/internal/move_list.hpp"
#include <chrono>
#include <ctime>
#include <fstream>
#include <iostream>

#include <chess/board.hpp>
#include <chess/import.hpp>
#include <chess/move_gen.hpp>
#include <engine/negamax.hpp>
#include <unordered_set>

#include <indicators/indicators.hpp>

using namespace Chess;

constexpr int SEARCH_DEPTH = 4;
constexpr int SEARCH_TIME = 200;

U64 searchSpace = 0;
U64 currentSearch = 0;

indicators::BlockProgressBar *bar = nullptr;

std::unordered_set<U64> checkedStates;
std::ofstream stream;

void EstimateSearchSpace(Board &board, int depth, double placement) {
    if (depth == 0)
        return;

    MoveList moves = MoveGen::GenerateMoves<MoveGen::GenType::All>(board, board.GetColor());
    searchSpace += moves.size();

    for (int i = 0; i < moves.size() * (1 - placement); i++) {
        board.DoMove(moves[i]);
        if (!checkedStates.contains(board.GetHash()))
            EstimateSearchSpace(board, depth - 1, (double)i / (double)moves.size());
        board.UndoMove(moves[i]);
    }
}

void Search(Board &board, int depth, double placement) {
    if (depth == 0)
        return;

    MoveList moves = MoveGen::GenerateMoves<MoveGen::GenType::All>(board, board.GetColor());
    std::array<int, MAXMOVECOUNT> scores{0};
    for (int i = 0; i < moves.size(); i++) {
        scores[i] = Engine::Negamax::EvaluateMove(board, moves[i], SEARCH_TIME);
        currentSearch++;
        if (currentSearch > searchSpace)
            searchSpace = currentSearch;
        bar->set_progress((double)currentSearch / (double)searchSpace * 100);
    }
    moves.sort(scores);

    Move move;
    // Recreate move to avoid weird shit
    if (!moves[0].IsCapture())
        move = Move(moves[0].GetType(), moves[0].GetFrom(), moves[0].GetTo());
    else
        move = Move(moves[0].GetType(), moves[0].GetFrom(), moves[0].GetTo(),
                    moves[0].GetCapturedPiece());
    stream << '{' << board.GetHash() << "LLU," << move.GetValue() << "},\n";

    for (int i = 0; i < moves.size() * (1 - placement); i++) {
        board.DoMove(moves[i]);
        if (!checkedStates.contains(board.GetHash()))
            Search(board, depth - 1, (double)i / (double)moves.size());
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

    Search(board, SEARCH_DEPTH, 0);
    tempBar.mark_as_completed();
    indicators::show_console_cursor(true);
    stream.close();
    std::cout << "Actual search space: " << currentSearch << '\n';

    return 0;
}
