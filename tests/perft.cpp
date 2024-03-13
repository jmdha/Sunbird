#include "board.hpp"
#include "import.hpp"
#include "move_gen.hpp"
#include "move_list.hpp"
#include "third_party/doctest.h"
#include <chrono>

size_t Perft(Board &board, int depth) {
    if (depth == 0) return 1;
    MoveList moves = GenerateMovesAll(board.Pos(), board.Pos().GetTurn());

    size_t nodes = 0;
    for (const auto &move : moves) {
        board.MakeMove(move);
        if (board.Pos().IsKingSafe(~board.Pos().GetTurn())) nodes += Perft(board, depth - 1);
        board.UndoMove();
    }

    return nodes;
}

struct Instance {
    std::string FEN;
    size_t depth;
    size_t nodes;
};

TEST_CASE("PERFT") {
    const Instance instances[] = {
        {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 1, 20},
        {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 2, 400},
        {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 3, 8'902},
        {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 4, 197'281},
        {"rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1", 5, 4'865'609},
        {"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ", 1, 48},
        {"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ", 2, 2'039},
        {"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ", 3, 97'862},
        {"r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ", 4, 4'085'603},
        {"8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - ", 1, 14},
        {"8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - ", 2, 191},
        {"8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - ", 3, 2'812},
        {"8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - ", 4, 43'238},
        {"8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - ", 5, 674'624},
        {"8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - ", 6, 11'030'083},
        {"r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 1, 6},
        {"r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 2, 264},
        {"r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 3, 9'467},
        {"r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 4, 422'333},
        {"r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1", 5, 15'833'292},
        {"rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", 1, 44},
        {"rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", 2, 1'486},
        {"rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", 3, 62'379},
        {"rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8", 4, 2'103'487},
        {"r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 1", 1, 46},
        {"r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 1", 2, 2'079},
        {"r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 1", 3, 89'890},
        {"r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 1", 4, 3'894'594},
        {"r4k2/pp2p3/1n3b1p/5QnP/8/2N5/PPP3P1/4R2K w - - 2 24", 1, 47},
        {"r4k2/pp2p3/1n3b1p/5QnP/8/2N5/PPP3P1/4R2K w - - 2 24", 2, 1'079},
        {"r4k2/pp2p3/1n3b1p/5QnP/8/2N5/PPP3P1/4R2K w - - 2 24", 3, 45'764},
        {"r4k2/pp2p3/1n3b1p/5QnP/8/2N5/PPP3P1/4R2K w - - 2 24", 4, 1'125'017},
        {"3rB2k/3PQRbp/6p1/1p1q1p2/7P/6P1/P4P1K/8 b - - 10 39", 1, 39},
        {"3rB2k/3PQRbp/6p1/1p1q1p2/7P/6P1/P4P1K/8 b - - 10 39", 2, 990},
        {"3rB2k/3PQRbp/6p1/1p1q1p2/7P/6P1/P4P1K/8 b - - 10 39", 3, 32'947},
        {"3rB2k/3PQRbp/6p1/1p1q1p2/7P/6P1/P4P1K/8 b - - 10 39", 4, 950'479},
    };

    for (const auto &instance : instances) {
        Board board = Import::FEN(instance.FEN);

        const auto t1     = std::chrono::high_resolution_clock::now();
        const auto nodes  = Perft(board, instance.depth);
        const auto t2     = std::chrono::high_resolution_clock::now();
        const size_t time = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();

        printf("depth %zu ", instance.depth);
        printf("nodes %zu ", nodes);
        printf("time %zu ms ", time);
        printf("nps %zu ", (nodes * 1000) / std::max(time, static_cast<size_t>(1)));
        printf("fen %s\n", instance.FEN.c_str());
        CHECK_EQ(nodes, instance.nodes);
    }
}
