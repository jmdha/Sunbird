#include <iostream>
#include <chrono>

#include "bit_shifts.hh"
#include "board.hh"
#include "board_importer.hh"
#include "perft.hh"
#include "tracy/Tracy.hpp"

int main(int, char* argv[]) {
    BitShifts::Init();
    Board board = BoardImporter::ImportFEN(argv[1]);
    auto perft = Perft(std::make_shared<Board>(board));
    auto depth = strtol(argv[2], nullptr, 10);
    auto total = perft.PerftDivide((int) depth);
    printf("Nodes searched: %d", total);
}
