#include <stdexcept>
#include <string>
#include <cstdlib>

#include <chess/internal/constants.hpp>
#include <chess/internal/bishop_gen.hpp>
#include <chess/board.hpp>
#include <chess/board_importer.hpp>
#include <chess/move_gen.hpp>

int main(int, char* argv[]) {
    BitShifts::Init();
    Board board = BoardImporter::ImportFEN((std::string) argv[2]);
    BishopGen moveGens[2] = { BishopGen(Color::White), BishopGen(Color::Black) };
    
    std::array<Move, MAXMOVECOUNT> moves;
    int moveCount = moveGens[(int) board.GetColor()].GetALlMoves(moves, std::make_shared<Board>(board), 0);

    if (moveCount == std::atoi(argv[1]))
        exit(EXIT_SUCCESS);
    else
        throw std::logic_error("Incorrect move count | Expected " + (std::string)argv[1] + " - Actual " + std::to_string(moveCount));
}
