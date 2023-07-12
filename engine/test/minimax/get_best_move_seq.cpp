#include <stdexcept>
#include <string>
#include <cstdlib>

#include <chess/board.hpp>
#include <chess/board_importer.hpp>
#include <engine/minimax.hpp>

int main(int, char* argv[]) {
    BitShifts::Init();
    std::string fen = std::string(argv[1]);
    Board board = BoardImporter::ImportMoveSequence(fen);
    std::string expectedMove = argv[3];    

    MiniMax minimax = MiniMax(std::make_shared<Board>(board));
    Move move = minimax.GetBestMove(std::atoi(argv[2]));

    if (expectedMove == move.ToString())
        exit(EXIT_SUCCESS);
    else
        throw std::logic_error("Incorrect Move | Expected " + expectedMove + " - Actual " + move.ToString());
}

