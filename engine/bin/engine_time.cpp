#include <iostream>

#include <chess/board.hpp>
#include <chess/import.hpp>
#include <engine/negamax.hpp>

using namespace Chess;

int main(int, char* argv[]) {
    int timeLimit = std::stoi(argv[1]);
    Board board = Import::MoveSequence(argv[2]);
    auto move = Engine::Negamax::GetBestMoveTime(board, timeLimit);
    if (move.first.has_value())
        printf("%s\n", move.first.value().ToString().c_str());
    else if (move.second == 0)
        printf("draw\n");
    else
        printf("in checkmate\n");
}
