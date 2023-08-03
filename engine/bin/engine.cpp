#include <iostream>

#include <chess/board.hpp>
#include <chess/import.hpp>
#include <engine/negamax.hpp>

using namespace Chess;

int main(int, char* argv[]) {
    Board board = Import::MoveSequence(argv[2]);
    auto move = Engine::Negamax::GetBestMove(board, std::stoi(argv[1]));
    if (std::holds_alternative<Move>(move))
        printf("%s\n", std::get<Move>(move).ToString().c_str());
}
