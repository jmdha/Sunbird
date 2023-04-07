#include <stdexcept>
#include <string>
#include <cstdlib>

#include "constants.hh"
#include "board.hh"
#include "board_importer.hh"
#include "bishop_gen.hh"
#include "bit_shifts.hh"

int main(int, char* argv[]) {
    BitShifts::Init();
    Board board = BoardImporter::ImportFEN((std::string) argv[2]);
    BishopGen moveGens[2] = { BishopGen(Color::White), BishopGen(Color::Black) };
    
    std::array<Move, MAXMOVECOUNT> moves;
    U64 attackedSquares = board.GenerateAttackSquares(board.GetOppColor());
    int moveCount = moveGens[(int) board.GetColor()].GetALlMoves(moves, std::make_shared<Board>(board), attackedSquares, (bool) std::atoi(argv[3]), 0);

    if (moveCount == std::atoi(argv[1]))
        exit(EXIT_SUCCESS);
    else
        throw std::logic_error("Incorrect move count | Expected " + (std::string)argv[1] + " - Actual " + std::to_string(moveCount));
}