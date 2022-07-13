#include <iostream>

#include "../classes/headers/bit_board.hh"
#include "../classes/headers/board_importer.hh"
#include "../classes/headers/minimax.hh"
#include "../classes/headers/move_gen.hh"
#include "../classes/headers/perft.hh"

int main(int argc, char* argv[]) {
    BitShifts::Init();
    BitBoard board = BitBoard();
    BoardImporter::ImportMoveSequence(&board, "g1h3 d7d5 h3g5 e7e5 g5f7 e8f7 b1c3 f8c5 c3b5 b8c6 b5c3 c8g4 f2f4 h7h5");
    MiniMax max = MiniMax(&board);
    Move move = max.GetBestMove(4);
    printf("%s\n", move.ToString().c_str());
}
