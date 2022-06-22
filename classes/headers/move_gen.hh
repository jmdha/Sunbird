#ifndef MOVEGEN
#define MOVEGEN

#include <vector>
#include <strings.h>

#include "bit_board.hh"
#include "bit_shifts.hh"
#include "constants.hh"
#include "move.hh"

class MoveGen {
public:
    std::vector<Move> GetPawnMoves(Color color, BitBoard board);
    std::vector<Move> GetRookMoves(Color color, BitBoard board);
    std::vector<Move> GetBishopMoves(Color color, BitBoard board);
    std::vector<Move> GetQueenMoves(Color color, BitBoard board);

private:
    std::vector<Move> GetMoves(Color color, BitBoard board, U64 pieces, Direction direction);
};
#endif
