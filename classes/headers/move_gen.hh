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
    MoveGen(Color color);
    std::vector<Move> GetAllMoves(BitBoard board);
    std::vector<Move> GetPawnMoves(BitBoard board);
    std::vector<Move> GetRookMoves(BitBoard board);
    std::vector<Move> GetBishopMoves(BitBoard board);
    std::vector<Move> GetQueenMoves(BitBoard board);

private:
    Color color;
    Color oppColor;
    Direction up;
    Direction upRight;
    Direction upLeft;
    Row doubleRank;

    std::vector<Move> GetMoves(BitBoard board, U64 pieces, Direction direction);
};
#endif
