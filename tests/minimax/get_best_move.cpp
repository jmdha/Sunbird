#include <stdexcept>
#include <string>
#include <cstdlib>

#include "board.hh"
#include "board_importer.hh"
#include "minimax.hh"

int main(int argc, char* argv[]) {
    BitShifts::Init();
    std::string fen = std::string(argv[1]);
    Board board = Board();
    BoardImporter::ImportFEN(&board, fen);
    std::string expectedMove = argv[3];    

    MiniMax minimax = MiniMax(&board);
    Move move = minimax.GetBestMove(std::atoi(argv[2]));

    if (expectedMove == move.ToString())
        exit(EXIT_SUCCESS);
    else
        throw std::logic_error("Incorrect Move | Expected " + expectedMove + " - Actual " + move.ToString());
}