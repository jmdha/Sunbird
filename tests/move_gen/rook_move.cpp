#include <stdexcept>
#include <string>
#include <cstdlib>

#include "constants.hh"
#include "board.hh"
#include "board_importer.hh"
#include "move_gen.hh"
#include "rook_gen.hh"

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