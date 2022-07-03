#include <iostream>

#include "../classes/headers/bit_board.hh"
#include "../classes/headers/board_importer.hh"
#include "../classes/headers/minimax.hh"
#include "../classes/headers/move_gen.hh"
#include "../classes/headers/perft.hh"

int main(int argc, char* argv[]) {
    BitShifts::Init();
    int time = std::atoi(argv[1]);
    BitBoard board = BitBoard();
    BoardImporter::ImportMoveSequence(&board, argv[2]);
    MiniMax max = MiniMax(&board);
    Move move = max.GetBestMove(2);
    printf("%s\n", move.ToString().c_str());
}
