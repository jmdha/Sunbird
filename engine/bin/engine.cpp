#include <iostream>

#include <chess/board.hpp>
#include <chess/import.hpp>
#include "engine/minimax.hpp"

int main(int, char* argv[]) {
    Board board = Import::MoveSequence(argv[2]);
    MiniMax max = MiniMax();
    Move move = max.GetBestMove(board);
    printf("%s\n", move.ToString().c_str());
}
