#include <iostream>
#include <chrono>

#include <chess/internal/bit_shifts.hpp>
#include <chess/board.hpp>
#include <chess/board_importer.hpp>
#include <chess/perft.hpp>

int main(int, char* argv[]) {
    BitShifts::Init();
    Board board = BoardImporter::ImportFEN(argv[1]);
    auto perft = Perft(std::make_shared<Board>(board));
    auto depth = strtol(argv[2], nullptr, 10);
    auto total = perft.PerftDivide((int) depth);
    printf("Nodes searched: %d\n", total);
}

