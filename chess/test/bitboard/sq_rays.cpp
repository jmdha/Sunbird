#include <cstdlib>
#include <sstream>
#include <stdexcept>
#include <string>

#include <chess/internal/bitboard.hpp>
#include <chess/internal/constants.hpp>
#include <chess/internal/utilities.hpp>

using namespace Chess;

int main(int, char *argv[]) {
    Square from = Utilities::GetSquare(argv[1][0], argv[1][1]);
    Square to = Utilities::GetSquare(argv[2][0], argv[2][1]);
    BB expected;
    std::stringstream(std::string(argv[3])) >> expected;

    BB actual = Ray(from, to);

    if (expected == actual)
        exit(EXIT_SUCCESS);
    else {
        printf("Expected: \"%lu\" Found: \"%lu\"\n", expected, actual);
        exit(EXIT_FAILURE);
    }
}
