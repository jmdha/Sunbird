#include "chess/internal/constants.hpp"
#include <iostream>

#include <chess/board.hpp>
#include <chess/import.hpp>
#include <engine/negamax.hpp>

using namespace Chess;

int main(int, char* argv[]) {
    int timeLimit = std::stoi(argv[1]);
    Board board = Import::MoveSequence(argv[2]);
    auto move = Engine::Negamax::GetBestMoveTime(board, 0.05 * timeLimit);
    if (move.GetType() == MoveType::SPECIAL_DRAW)
        printf("draw\n");
    else if (move.GetType() == MoveType::SPECIAL_CHECKMATE)
        printf("in checkmate\n");
    else
        printf("%s\n", move.ToString().c_str());
}
