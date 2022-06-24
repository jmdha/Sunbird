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
    int GetAllMoves(Move* moves, BitBoard board);
    int GetPawnMoves(Move* moves, int startIndex, BitBoard board);
    int GetRookMoves(Move* moves, int startIndex, BitBoard board);
    int GetBishopMoves(Move* moves, int startIndex, BitBoard board);
    int GetQueenMoves(Move* moves, int startIndex, BitBoard board);
    int GetKnightMoves(Move* moves, int startIndex, BitBoard board);
    int GetKingMoves(Move* moves, int startIndex, BitBoard board);

private:
    Color color;
    Color oppColor;
    Direction up;
    Direction upRight;
    Direction upLeft;
    Row doubleRank;

    U64 knightMoves[64] = {};
    U64 kingMoves[64] = {};

    int GetMoves(Move* moves, int startIndex, BitBoard board, U64 pieces, Direction direction, PieceType type);
    void GenerateKnightMoves();
    void GenerateKingMoves();
};
#endif
