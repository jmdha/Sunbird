#include <stdexcept>
#include <string>
#include <cstdlib>

#include <chess/internal/constants.hpp>
#include <chess/internal/knight_gen.hpp>
#include <chess/board.hpp>
#include <chess/import.hpp>
#include <chess/move_gen.hpp>

int main(int, char* argv[]) {
    BitShifts::Init();
    Board board = Import::FEN((std::string) argv[2]);
    KnightGen moveGens[2] = { KnightGen(Color::White), KnightGen(Color::Black) };
    
    std::array<Move, MAXMOVECOUNT> moves;
    int moveCount = moveGens[(int) board.GetColor()].GetALlMoves(moves, std::make_shared<Board>(board), 0);

    if (moveCount == std::atoi(argv[1]))
        exit(EXIT_SUCCESS);
    else
        throw std::logic_error("Incorrect move count | Expected " + (std::string)argv[1] + " - Actual " + std::to_string(moveCount));
}
