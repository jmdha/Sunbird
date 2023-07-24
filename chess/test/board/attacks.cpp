#include <stdexcept>
#include <string>
#include <cstdlib>

#include <string>
#include <sstream>

#include <chess/internal/constants.hpp>
#include <chess/board.hpp>
#include <chess/import.hpp>

using namespace Chess;

int main(int, char* argv[]) {
    std::string FEN = std::string(argv[1]);
    U64 expected;
    std::stringstream(std::string(argv[2])) >> expected;
    Board board = Import::FEN(FEN);

    U64 actual = board.GenerateAttackSquares(board.GetColor());

    if (expected == actual)
        exit(EXIT_SUCCESS);
    else {
        printf("Expected %llu actual %llu\n", expected, actual);
        exit(EXIT_FAILURE);
    }
}
