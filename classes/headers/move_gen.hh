#ifndef MOVE_GEN
#define MOVE_GEN

#include <vector>
#include <strings.h>

#include "board.hh"
#include "bit_shifts.hh"
#include "constants.hh"
#include "move.hh"

class MoveGen {
public:
    MoveGen(Color color);
    U8 GetAllMoves     (std::array<Move, MAXMOVECOUNT> *moves, Board *board, U64 attackedSquares);
    U8 GetPawnMoves    (std::array<Move, MAXMOVECOUNT> *moves, int startIndex, Board *board, bool isKingSafe, U64 attackedSquares);
    U8 GetRookMoves    (std::array<Move, MAXMOVECOUNT> *moves, int startIndex, Board *board, bool isKingSafe, U64 attackedSquares);
    U8 GetBishopMoves  (std::array<Move, MAXMOVECOUNT> *moves, int startIndex, Board *board, bool isKingSafe, U64 attackedSquares);
    U8 GetQueenMoves   (std::array<Move, MAXMOVECOUNT> *moves, int startIndex, Board *board, bool isKingSafe, U64 attackedSquares);
    U8 GetKnightMoves  (std::array<Move, MAXMOVECOUNT> *moves, int startIndex, Board *board, bool isKingSafe, U64 attackedSquares);
    U8 GetKingMoves    (std::array<Move, MAXMOVECOUNT> *moves, int startIndex, Board *board, bool isKingSafe, U64 attackedSquares);
    U64 GetAttackSquares(Board *board);

    bool IsKingSafe(Board *board, U64 tempOccuracyBoard, U64 tempEnemyBoard, U64 tempKingBoard);
    bool IsKingSafe(Board *board, U64 tempOccuracyBoard, U64 tempEnemyBoard);
    bool IsKingSafe(Board *board, U64 tempOccuracyBoard);
    bool IsKingSafe(Board *board);

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

    U64 pawnSingleMove[64] = { 0 };
    U64 pawnDoubleMove[64] = { 0 };

    U8 GetMoves(std::array<Move, MAXMOVECOUNT> *moves, int startIndex, Board *board, U64 pieces, Direction direction, PieceType type, bool isKingSafe);
    void GeneratePawnMoves();

    U64 GetSlidingAttacks(const Board *board, const U64 pieceIndex, const Direction directions[], const int directionCount) const;

    inline void AppendMove(std::array<Move, MAXMOVECOUNT> *moves, int index, U8* moveCount, Move move);
    inline void AppendAttack(U64 (*attackedSquares)[2], U64 newAttack);
};

inline void MoveGen::AppendMove(std::array<Move, MAXMOVECOUNT> *moves, int index, U8* moveCount, Move move) {
    moves->at(index) = move;
    (*moveCount)++;
}

inline void MoveGen::AppendAttack(U64 (*attackedSquares)[2], U64 newAttack) {
    if ((*attackedSquares)[0] & newAttack)
        (*attackedSquares)[1] |= newAttack;
    else
        (*attackedSquares)[0] |= newAttack;
}

#endif // MOVE_GEN
