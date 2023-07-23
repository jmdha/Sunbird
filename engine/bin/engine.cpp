#include <iostream>

#include <chess/board.hpp>
#include <chess/import.hpp>
#include <engine/negamax.hpp>

int main(int, char* argv[]) {
    Board board = Import::MoveSequence(argv[2]);
    Move move = Engine::Negamax::GetBestMove(board);
    printf("%s\n", move.ToString().c_str());
}
