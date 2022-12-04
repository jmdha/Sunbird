#include <iostream>
#include <chrono>

#include "../classes/headers/board.hh"
#include "../classes/headers/board_importer.hh"
#include "../classes/headers/minimax.hh"
#include "../classes/headers/move_gen.hh"
#include "../classes/headers/perft.hh"

int main(int argc, char* argv[]) {
    std::vector<int> test;
    test.push_back(0);
    test.push_back(2);
    test.push_back(1);
    test.push_back(4);
    BitShifts::Init();
    Board board = Board();
    BoardImporter::ImportFEN(&board, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    MiniMax max = MiniMax(&board);
    auto t0 = std::chrono::steady_clock::now();
    Move move = max.GetBestMove(4);
    auto t1 = std::chrono::steady_clock::now();
    auto elapsed = (unsigned long long) std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
    printf("Move found %s\n", move.ToString().c_str());
    printf("Moves made %llu (%llu/s)\n", board.GetStats().totalMoves, (board.GetStats().totalMoves / elapsed) * 1000);
}
