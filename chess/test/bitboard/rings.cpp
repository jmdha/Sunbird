#include <chess/internal/bitboard.hpp>
#include <chess/internal/utilities.hpp>
#include <cstdlib>
#include <sstream>
#include <string>

using namespace Chess;

int main(int, char *argv[]) {
    Square square = Utilities::GetSquare(argv[1][0], argv[1][1]);
    int offset = std::stoi(argv[2]);
    BB expected;
    std::stringstream(std::string(argv[3])) >> expected;

    BB actual = Ring(square, offset);

    if (expected == actual)
        exit(EXIT_SUCCESS);
    else {
        printf("Expected: \"%lu\" Found: \"%lu\"\n", expected, actual);
        exit(EXIT_FAILURE);
    }
}
