#include <stdexcept>
#include <string>
#include <cstdlib>

#include "board_importer.hh"

int main(int argc, char* argv[]) {
    std::string FEN = std::string(argv[1]);
    Board expectedBoard = Board();
    expectedBoard.Initialize();

    Board* boardFEN = new Board();

    BoardImporter::ImportFEN(boardFEN, FEN);

    for (int x = 0; x < WIDTH; x++) 
        for (int y = 0; y < HEIGHT; y++)
            if (expectedBoard.GetType(Utilities::GetSquare(x, y)) != 
                boardFEN->GetType(Utilities::GetSquare(x, y)))
                throw std::logic_error("Unexpected PieceGen | Expected "
                                + std::to_string((char)expectedBoard.GetType(Utilities::GetSquare(x, y))) + 
                " - Actual "    + std::to_string((char)boardFEN->GetType(Utilities::GetSquare(x, y))) + 
                + " | Expected "+ ((char)expectedBoard.GetType(Utilities::GetSquare(x, y))) + 
                " - Actual "    + ((char)boardFEN->GetType(Utilities::GetSquare(x, y))) + 
                " | At pos "    + std::to_string(x) + "," + std::to_string(y));
            


    exit(EXIT_SUCCESS);  
}