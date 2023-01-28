#include <stdexcept>
#include <string>
#include <stdlib.h>

#include "../../classes/headers/constants.hh"
#include "../../classes/headers/board.hh"
#include "../../classes/headers/board_importer.hh"
#include "../../classes/move_gen/headers/move_gen.hh"
#include "../../classes/move_gen/headers/rook_gen.hh"

int main(int argc, char* argv[]) {
    BitShifts::Init();
    Board board = Board();
    BoardImporter::ImportFEN(&board, (std::string) argv[2]);
    RookGen moveGens[2] = { RookGen(Color::White), RookGen(Color::Black) };
    
    std::array<Move, MAXMOVECOUNT> moves;
    U64 attackedSquares = board.GenerateAttackSquares(board.GetOppColor());
    int moveCount = moveGens[(int) board.GetColor()].GetALlMoves(&moves, &board, attackedSquares, (bool) std::atoi(argv[3]), 0);

    if (moveCount == std::atoi(argv[1]))
        exit(EXIT_SUCCESS);
    else
        throw std::logic_error("Incorrect move count | Expected " + (std::string)argv[1] + " - Actual " + std::to_string(moveCount));
}