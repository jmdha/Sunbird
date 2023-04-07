#ifndef PERFT
#define PERFT

#include <string>
#include <memory>
#include <utility>

#include "constants.hh"
#include "board.hh"
#include "move_gen.hh"
#include "move.hh"

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
