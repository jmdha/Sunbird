#include <iostream>

#include "board.hh"
#include "board_importer.hh"
#include "minimax.hh"

int main(int, char* argv[]) {
    BitShifts::Init();
    Board board = BoardImporter::ImportMoveSequence(argv[2]);
    MiniMax max = MiniMax(std::make_shared<Board>(board));
    Move move = max.GetBestMove();
    printf("%s\n", move.ToString().c_str());
}
