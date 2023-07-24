#include <chrono>
#include <iostream>

#include <chess/board.hpp>
#include <chess/import.hpp>
#include <chess/perft.hpp>

using namespace Chess;

int main(int, char *argv[]) {
    Board board = Import::FEN(argv[1]);
    auto depth = strtol(argv[2], nullptr, 10);
    Perft perft = Perft();
    auto total = perft.PerftDivide(board, (int)depth);
    printf("Nodes searched: %d\n", total);
}
