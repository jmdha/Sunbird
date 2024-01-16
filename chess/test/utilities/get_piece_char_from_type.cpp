#include <chess/internal/utilities.hpp>
#include <cstdlib>
#include <stdexcept>
#include <string>

using namespace Chess;

int main(int, char *argv[]) {
    PieceChar expectedPieceChar = (PieceChar)std::atoi(argv[1]);
    PieceType type = (PieceType)std::atoi(argv[2]);
    Color color = (Color)std::atoi(argv[3]);

    PieceChar actualPieceChar = Utilities::GetPieceChar(type, color);

    if (expectedPieceChar == actualPieceChar)
        exit(EXIT_SUCCESS);
    else
        throw std::logic_error("Unexpected PieceChar | Expected " +
                               std::to_string((int)expectedPieceChar) + " - Actual " +
                               std::to_string((int)actualPieceChar));
}
