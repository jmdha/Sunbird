#include <stdexcept>
#include <string>
#include <stdlib.h>

#include "../../classes/headers/constants.hh"
#include "../../classes/headers/utilities.hh"

int main(int argc, char* argv[]) {
    Square expectedSquare = (Square) std::atoi((argv[1]));
    int x = std::atoi(argv[2]);
    int y = std::atoi(argv[3]);

    Square actualSquare = Utilities::GetSquare(x, y);

    if (expectedSquare == actualSquare)
        exit(EXIT_SUCCESS);
    else
        throw std::logic_error("Unexpected Square | Expected " + std::to_string((int)expectedSquare) + " - Actual " + std::to_string((int)actualSquare));
}