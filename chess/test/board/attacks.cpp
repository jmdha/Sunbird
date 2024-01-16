#include <chess/board.hpp>
#include <chess/import.hpp>
#include <cstdlib>
#include <sstream>
#include <string>

using namespace Chess;

int main(int, char *argv[]) {
    std::string FEN = std::string(argv[1]);
    BB expected;
    std::stringstream(std::string(argv[2])) >> expected;
    Board board = Import::FEN(FEN);

    BB actual = board.Pos().GenerateAttackSquares(board.Pos().GetTurn());

    if (expected == actual)
        exit(EXIT_SUCCESS);
    else {
        printf("Expected %lu actual %lu\n", expected, actual);
        exit(EXIT_FAILURE);
    }
}
