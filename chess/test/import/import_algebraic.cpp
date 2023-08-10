#include <stdexcept>
#include <string>
#include <cstdlib>

#include <chess/import.hpp>
#include <chess/export.hpp>

using namespace Chess;

int main(int, char* argv[]) {
    std::string moves = std::string(argv[1]);
    std::string expectedFEN = std::string(argv[2]);

    Board board = Import::Algebraic(moves);

    std::string actualFEN = Export::FEN(board.Pos());

    if (expectedFEN == actualFEN)
        exit(EXIT_SUCCESS);  
    else {
        printf("Expected \"%s\" found \"%s\"", expectedFEN.c_str(), actualFEN.c_str());
        exit(EXIT_FAILURE);
    }
}

