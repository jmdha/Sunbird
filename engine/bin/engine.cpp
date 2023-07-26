#include <iostream>

#include <chess/board.hpp>
#include <chess/import.hpp>
#include <engine/negamax.hpp>

using namespace Chess;

int main(int, char* argv[]) {
    Board board = Import::MoveSequence(argv[2]);
    auto move = Engine::Negamax::GetBestMove(board, 5);
    if (move.first.has_value())
        printf("%s\n", move.first.value().ToString().c_str());
    else if (move.second == 0)
        printf("draw\n");
    else
        printf("in checkmate\n");
}
