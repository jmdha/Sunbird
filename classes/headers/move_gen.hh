#ifndef MOVEGEN
#define MOVEGEN

#include <vector>
#include <strings.h>

#include "board.hh"
#include "bit_shifts.hh"
#include "constants.hh"
#include "move.hh"

class MoveGen {
public:
    MoveGen(Color color);
    int GetAllMoves     (Move* moves, Board board, U64 (*attackedSquares)[2]);
    int GetPawnMoves    (Move* moves, int startIndex, Board board, bool isKingSafe, U64 (*attackedSquares)[2]);
    int GetRookMoves    (Move* moves, int startIndex, Board board, bool isKingSafe, U64 (*attackedSquares)[2]);
    int GetBishopMoves  (Move* moves, int startIndex, Board board, bool isKingSafe, U64 (*attackedSquares)[2]);
    int GetQueenMoves   (Move* moves, int startIndex, Board board, bool isKingSafe, U64 (*attackedSquares)[2]);
    int GetKnightMoves  (Move* moves, int startIndex, Board board, bool isKingSafe, U64 (*attackedSquares)[2]);
    int GetKingMoves    (Move* moves, int startIndex, Board board, bool isKingSafe, U64 (*attackedSquares)[2], U64 priorAttacks[2]);
    U64 GetAttackSquares(Board board, Square fromSquare);

    bool IsKingSafe(Board board, U64 tempOccuracyBoard, U64 tempEnemyBoard, U64 tempKingBoard);
    bool IsKingSafe(Board board, U64 tempOccuracyBoard, U64 tempEnemyBoard);
    bool IsKingSafe(Board board, U64 tempOccuracyBoard);
    bool IsKingSafe(Board board);

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

    int GetMoves(Move* moves, int startIndex, Board board, U64 pieces, Direction direction, PieceType type, bool isKingSafe, U64 (*attackedSquares)[2]);
    void GeneratePawnMoves();
    void GenerateKnightMoves();
    void GenerateKingMoves();
    
    
    bool IsSafeMove(Board board, Square square);

    inline void AppendMove(Move* moves, int index, int* moveCount, Move move);
};

inline void MoveGen::AppendMove(Move* moves, int index, int* moveCount, Move move) {
    moves[index] = move;
    (*moveCount)++;
}

#endif
