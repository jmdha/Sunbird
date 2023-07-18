#include <stdexcept>
#include <string>
#include <cstdlib>

#include <chess/board.hpp>
#include <chess/import.hpp>
#include <engine/minimax.hpp>

int main(int, char* argv[]) {
    std::string fen = std::string(argv[1]);
    Board board = Import::MoveSequence(fen);
    std::string expectedMove = argv[3];    

    MiniMax minimax = MiniMax();
    Move move = minimax.GetBestMove(board, std::atoi(argv[2]));

    if (expectedMove == move.ToString())
        exit(EXIT_SUCCESS);
    else
        throw std::logic_error("Incorrect Move | Expected " + expectedMove + " - Actual " + move.ToString());
}

