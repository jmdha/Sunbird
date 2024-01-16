#include <chess/board.hpp>
#include <chess/export.hpp>
#include <chess/import.hpp>
#include <cstdlib>
#include <string>

using namespace Chess;

int main(int, char *argv[]) {
    std::string expectedFEN = std::string(argv[1]);
    Board board = Import::FEN(expectedFEN);

    std::string actualFEN = Export::FEN(board.Pos());

    if (expectedFEN == actualFEN)
        exit(EXIT_SUCCESS);
    else {
        printf("Expected \"%s\" found \"%s\"", expectedFEN.c_str(), actualFEN.c_str());
        exit(EXIT_FAILURE);
    }
}
