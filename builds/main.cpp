#include <iostream>

#include "../classes/headers/bit_board.hh"
#include "../classes/headers/board_importer.hh"
#include "../classes/headers/minimax.hh"
#include "../classes/headers/move_gen.hh"
#include "../classes/headers/perft.hh"

int main(int argc, char* argv[]) {
    BitShifts::Init();
    BitBoard board = BitBoard();
    BoardImporter::ImportFEN(&board, "rnbqkbnr/pp2pppp/8/2pp2N1/8/8/PPPPPPPP/RNBQKB1R w KQkq - 0 3");
    MiniMax max = MiniMax(&board);
    Move move = max.GetBestMove(5);
    printf("Move found %s\n", move.ToString().c_str());
    printf("Moves made %llu\n", board.totalMoves);
}
