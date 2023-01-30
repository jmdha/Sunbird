#include <iostream>

#include "board.hh"
#include "board_importer.hh"
#include "minimax.hh"

int main(int argc, char* argv[]) {
    BitShifts::Init();
    Board board = Board();
    BoardImporter::ImportMoveSequence(&board, argv[2]);
    MiniMax max = MiniMax(&board);
    Move move = max.GetBestMove();
    printf("%s\n", move.ToString().c_str());
}
