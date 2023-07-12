#ifndef PERFT
#define PERFT

#include <string>
#include <memory>
#include <utility>

#include "internal/constants.hpp"
#include "board.hpp"
#include "move.hpp"
#include "move_gen.hpp"

class Perft {
public:
    explicit Perft(std::shared_ptr<Board> board) : board(std::move(board)), moveGens{ MoveGen(Color::White), MoveGen(Color::Black) } {};

    int RunFromPosition(int depth);
    int PerftDivide(int depth);

private:
    std::shared_ptr<Board> board;
    MoveGen moveGens[2];

    unsigned int moveTypeCount[16] = { 0 };

    int Run(int depth);

};
#endif // PERFT
