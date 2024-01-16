#include <chess/internal/utilities.hpp>
#include <cstdlib>
#include <stdexcept>
#include <string>

using namespace Chess;

int main(int, char *argv[]) {
    PieceType expectedPieceType = (PieceType)std::atoi(argv[1]);
    PieceChar pieceChar = (PieceChar)argv[2][0];

    PieceType actualPieceType = (PieceType)Utilities::GetPieceType(pieceChar);

    if (expectedPieceType == actualPieceType)
        exit(EXIT_SUCCESS);
    else
        throw std::logic_error("Unexpected PieceType | Expected " +
                               std::to_string((int)expectedPieceType) + " - Actual " +
                               std::to_string((int)actualPieceType));
}
