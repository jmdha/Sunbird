#ifndef PERFT
#define PERFT

#include <string>

#include "constants.hh"
#include "bit_board.hh"
#include "move_gen.hh"
#include "move.hh"

class Perft {
public:
    Perft(BitBoard* board);
    int RunFromMove(Move move, int depth);

private:
    BitBoard* board;
    MoveGen* moveGens[2];

    int Run(int depth);

};
#endif
