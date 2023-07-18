#include <cstdlib>
#include <sstream>
#include <stdexcept>
#include <string>

#include <chess/internal/bit_shift.hpp>
#include <chess/internal/constants.hpp>
#include <chess/internal/utilities.hpp>

int main(int, char *argv[]) {
    Square square = Utilities::GetSquare(argv[1][0], argv[1][1]);
    Direction dir = (Direction)std::stoi(argv[2]);
    U64 expected;
    std::stringstream(std::string(argv[3])) >> expected;

    U64 actual = BitShift::RAYS[(int)square][(int)Utilities::GetDirectionIndex(dir)];

    if (expected == actual)
        exit(EXIT_SUCCESS);
    else {
        printf("Expected: \"%llu\" Found: \"%llu\"\n", expected, actual);
        exit(EXIT_FAILURE);
    }
}
