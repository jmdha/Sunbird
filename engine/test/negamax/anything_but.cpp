#include <cstdlib>
#include <stdexcept>
#include <string>

#include <chess/board.hpp>
#include <chess/import.hpp>
#include <engine/negamax.hpp>

int main(int, char *argv[]) {
    std::string fen = std::string(argv[1]);
    Board board = Import::FEN(fen);
    std::string expectedMove = argv[3];

    Move move = Engine::Negamax::GetBestMove(board, std::atoi(argv[2]));

    if (expectedMove != move.ToString())
        exit(EXIT_SUCCESS);
    else
        throw std::logic_error("Incorrect Move | Expected " + expectedMove + " - Actual " +
                               move.ToString());
}
