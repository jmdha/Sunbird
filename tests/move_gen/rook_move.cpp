#include <stdexcept>
#include <string>
#include <stdlib.h>

#include "../../classes/headers/constants.hh"
#include "../../classes/headers/board.hh"
#include "../../classes/headers/board_importer.hh"
#include "../../classes/headers/move_gen.hh"

int main(int argc, char* argv[]) {
    BitShifts::Init();
    Board board = Board();
    BoardImporter::ImportFEN(&board, (std::string) argv[2]);
    MoveGen moveGens[2] = { MoveGen(Color::White), MoveGen(Color::Black) };
    
    std::array<Move, MAXMOVECOUNT> moves;
    U64 attackedSquares = moveGens[(int) Utilities::GetOppositeColor(board.GetColor())].GetAttackSquares(&board);
    int moveCount = moveGens[(int) board.GetColor()].GetRookMoves(&moves, 0, &board, (bool)std::atoi(argv[3]), attackedSquares);

    if (moveCount == std::atoi(argv[1]))
        exit(EXIT_SUCCESS);
    else
        throw std::logic_error("Incorrect move count | Expected " + (std::string)argv[1] + " - Actual " + std::to_string(moveCount));
}