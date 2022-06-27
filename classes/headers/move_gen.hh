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
    int GetAllMoves(Move* moves, BitBoard board, U64* attackedSquares);
    int GetPawnMoves(Move* moves, int startIndex, BitBoard board, U64* attackedSquares);
    int GetRookMoves(Move* moves, int startIndex, BitBoard board, U64* attackedSquares);
    int GetBishopMoves(Move* moves, int startIndex, BitBoard board, U64* attackedSquares);
    int GetQueenMoves(Move* moves, int startIndex, BitBoard board, U64* attackedSquares);
    int GetKnightMoves(Move* moves, int startIndex, BitBoard board, U64* attackedSquares);
    int GetKingMoves(Move* moves, int startIndex, BitBoard board, U64* attackedSquares, U64 priorAttacks);

private:
    Color color;
    Color oppColor;
    Direction up;
    Direction upRight;
    Direction upLeft;
    Row doubleRank;
    Row enPassantRank;
    NotEdge notPromotionRank;
    CastlingBlockSquares castlingBlock[2];
    CastlingAttackSquares castlingAttack[2];

    U64 pawnSingleMove[64] = {};
    U64 pawnDoubleMove[64] = {};
    U64 pawnCaptureMoves[64] = {};
    U64 knightMoves[64] = {};
    U64 kingMoves[64] = {};

    int GetMoves(Move* moves, int startIndex, BitBoard board, U64 pieces, Direction direction, PieceType type, U64* attackedSquares);
    void GeneratePawnMoves();
    void GenerateKnightMoves();
    void GenerateKingMoves();
};
#endif
