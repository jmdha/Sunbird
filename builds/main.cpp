#include <iostream>

#include "../classes/headers/bit_board.hh"
#include "../classes/headers/board_importer.hh"
#include "../classes/headers/minimax.hh"
#include "../classes/headers/move_gen.hh"
#include "../classes/headers/perft.hh"

int main(int argc, char* argv[]) {
    BitShifts::Init();
    BitBoard board = BitBoard();
    BoardImporter::ImportFEN(&board, "r7/1r5p/1p6/8/1kpp4/2b5/8/q1K5 w - - 0 69");
    MiniMax max = MiniMax(&board);
    Move move = max.GetBestMove(12);
    printf("%s\n", move.ToString().c_str());
}
