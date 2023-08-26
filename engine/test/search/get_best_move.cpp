#include "engine/internal/tt.hpp"
#include <cstdlib>
#include <stdexcept>
#include <string>

#include <chess/board.hpp>
#include <chess/import.hpp>
#include <engine/search.hpp>

using namespace Chess;

int main(int, char *argv[]) {
    Engine::TT::Init(128);
    std::string fen = std::string(argv[1]);
    Board board = Import::FEN(fen);
    std::string expectedMove = argv[3];

    auto move = Engine::Search::GetBestMoveTime(board, std::atoi(argv[2]));
    Engine::TT::Clean();

    if (expectedMove == std::get<Move>(move).ToString())
        exit(EXIT_SUCCESS);
    else
        throw std::logic_error("Incorrect Move | Expected " + expectedMove + " - Actual " +
                               std::get<Move>(move).ToString());
}
