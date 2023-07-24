#include <stdexcept>
#include <string>
#include <cstdlib>

#include <chess/internal/constants.hpp>
#include <chess/internal/utilities.hpp>

using namespace Chess;

int main(int, char* argv[]) {
    Color expectedColor = (Color) std::atoi(argv[1]);
    PieceChar piece = (PieceChar) argv[2][0];

    Color actualColor = (Color) Utilities::GetPieceColor(piece);

    if (expectedColor == actualColor)
        exit(EXIT_SUCCESS);
    else
        throw std::logic_error("Unexpected Color | Expected " + std::to_string((int)expectedColor) + " - Actual " + std::to_string((int)actualColor));
}
