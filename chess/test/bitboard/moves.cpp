#include <chess/internal/bitboard.hpp>
#include <chess/internal/utilities.hpp>
#include <cstdlib>
#include <sstream>
#include <string>

using namespace Chess;

int main(int, char *argv[]) {
    PieceType type = Utilities::GetPieceType((PieceChar)argv[1][0]);
    Square square = Utilities::GetSquare(argv[2][0], argv[2][1]);
    BB expected;
    std::stringstream(std::string(argv[3])) >> expected;

    BB actual = Attacks(square, type);

    if (expected == actual)
        exit(EXIT_SUCCESS);
    else {
        printf("Expected: \"%lu\" Found: \"%lu\"\n", expected, actual);
        exit(EXIT_FAILURE);
    }
}
