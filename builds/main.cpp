#include <iostream>
#include <chrono>

#include "headers/bit_shifts.hh"
#include "headers/board.hh"
#include "headers/board_importer.hh"
#include "headers/minimax.hh"

int main(int argc, char* argv[]) {
    BitShifts::Init();
    Board board = Board();
    BoardImporter::ImportFEN(&board, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    MiniMax max = MiniMax(&board);
    auto t0 = std::chrono::steady_clock::now();
    Move move = max.GetBestMove();
    auto t1 = std::chrono::steady_clock::now();
    auto elapsed = (unsigned long long) std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
    if (elapsed == 0)
        elapsed = 1;
    printf("Move found %s\n", move.ToString().c_str());
    printf("Moves made %llu (%llu/s)\n", board.GetStats().totalMoves, (board.GetStats().totalMoves / elapsed) * 1000);
    printf("Elapsed %llu ms\n", elapsed);
}
