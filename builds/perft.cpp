#include <iostream>
#include <chrono>

#include "bit_shifts.hh"
#include "board.hh"
#include "board_importer.hh"
#include "perft.hh"

int main(int argc, char* argv[]) {
    BitShifts::Init();
    Board board = Board();
    BoardImporter::ImportFEN(&board, argv[1]);
    auto perft = Perft(&board);
    auto depth = strtol(argv[2], nullptr, 10);
    auto total = perft.PerftDivide((int) depth);
    printf("Nodes searched: %d", total);
}
