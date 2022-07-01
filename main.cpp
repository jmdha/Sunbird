#include <iostream>

#include "classes/headers/bit_board.hh"
#include "classes/headers/board_importer.hh"
#include "classes/headers/minimax.hh"
#include "classes/headers/move_gen.hh"
#include "classes/headers/perft.hh"

int main(int argc, char* argv[]) {
    BitBoard board = BitBoard();
    BoardImporter::ImportFEN(&board, (std::string) "3r4/8/8/8/8/8/8/3K4 w - - 0 1");
    int expectedBool = std::atoi("1");
    BitShifts::Init();

    MoveGen gen = MoveGen(board.GetColor());
    int kingSafe = (int) gen.IsKingSafe(board);


    if (expectedBool == kingSafe)
        exit(EXIT_SUCCESS);
    else
        throw std::logic_error("Incorrect move count | Expected " + std::to_string(expectedBool) + " - Actual " + std::to_string(kingSafe));
    /*BitBoard board = BitBoard();
    board.Initialize();
    BitShifts::Init();
    BitShifts bt;
    
    MiniMax mini = MiniMax(&board);
    Move move = mini.GetBestMove(6);
    printf("Done\n");*/
}
