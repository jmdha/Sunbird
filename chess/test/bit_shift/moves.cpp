#include <cstdlib>
#include <sstream>
#include <stdexcept>
#include <string>

#include <chess/internal/bit_shift.hpp>
#include <chess/internal/constants.hpp>
#include <chess/internal/utilities.hpp>

using namespace Chess;

int main(int, char *argv[]) {
    PieceType type = Utilities::GetPieceType((PieceChar)argv[1][0]);
    Square square = Utilities::GetSquare(argv[2][0], argv[2][1]);
    U64 expected;
    std::stringstream(std::string(argv[3])) >> expected;

    U64 actual = BitShift::MOVES[(U8)type][(U8)square];

    if (expected == actual)
        exit(EXIT_SUCCESS);
    else {
        printf("Expected: \"%lu\" Found: \"%lu\"\n", expected, actual);
        exit(EXIT_FAILURE);
    }
}
