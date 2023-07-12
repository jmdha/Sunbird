#include <stdexcept>
#include <string>
#include <cstdlib>

#include <chess/board.hpp>
#include <chess/import.hpp>
#include <chess/export.hpp>

int main(int, char* argv[]) {
    std::string expectedFEN = std::string(argv[1]);
    Board board = Import::FEN(expectedFEN);

    std::string actualFEN = Export::FEN(board);
    
    if (expectedFEN == actualFEN)
        exit(EXIT_SUCCESS);  
    else {
        printf("Expected \"%s\" found \"%s\"", expectedFEN.c_str(), actualFEN.c_str());
        exit(EXIT_FAILURE);
    }
}
