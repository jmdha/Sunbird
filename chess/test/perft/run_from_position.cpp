#include <stdexcept>
#include <string>
#include <cstdlib>

#include <chess/board.hpp>
#include <chess/import.hpp>
#include <chess/perft.hpp>

int main(int, char* argv[]) {
    BitShifts::Init();
    int expectedLeafCount = std::atoi(argv[1]);

    std::string FEN = std::string(argv[2]);
    Board board = Import::FEN(FEN);

    int depth = std::atoi(argv[3]);

    Perft perft = Perft(std::make_shared<Board>(board));
    int actualLeafCount = perft.RunFromPosition(depth);

    if (expectedLeafCount == actualLeafCount)
        exit(EXIT_SUCCESS);
    else
        throw std::logic_error("Incorrect leaf count | Expected " + std::to_string(expectedLeafCount) + " - Actual " + std::to_string(actualLeafCount));
}
