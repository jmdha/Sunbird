#include "chess/import.hpp"
#include "chess/perft.hpp"
#include <chrono>
#include <cstddef>
#include <string>
#include <vector>

using namespace Chess;

const std::vector<std::string> PERFT_POSITIONS{
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",                 // Init,
    "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ",        // Kiwipete
    "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - ",                                   // NoCastle
    "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1",         // Position 4
    "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8  ",              // Position 5
    "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10 " // Position 6
};

int main() {
    printf("MEASURING PERFT\n");

    size_t total_time = 0;
    size_t total_nodes = 0;
    for (uint i = 0; i < PERFT_POSITIONS.size(); i++) {
        Board board = Chess::Import::FEN(PERFT_POSITIONS[i]);
        printf("%d/%zu\n", i + 1, PERFT_POSITIONS.size());
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        const auto count = Chess::Perft::RunFromPosition(board, 5);
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        size_t time = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
        total_time += time;
        total_nodes += count;
    }

    printf("Nodes: %zu, Time: %zu, NPS: %zu\n", total_nodes, total_time,
           total_nodes / total_time * 1000);
    printf("FINISHED  PERFT\n");
    return 0;
}
