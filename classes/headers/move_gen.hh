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
    explicit MoveGen(Color color);
    U8 GetAllMoves     (std::array<Move, MAXMOVECOUNT> *moves, Board *board, U64 attackedSquares);

    U8 GetPawnMoves(std::array<Move, MAXMOVECOUNT> *moves, int startIndex, Board *board, bool isKingSafe);

    U8 GetRookMoves(std::array<Move, MAXMOVECOUNT> *moves, int startIndex, Board *board, bool isKingSafe);

    U8 GetBishopMoves(std::array<Move, MAXMOVECOUNT> *moves, int startIndex, Board *board, bool isKingSafe);

    U8 GetQueenMoves(std::array<Move, MAXMOVECOUNT> *moves, int startIndex, Board *board, bool isKingSafe);

    U8 GetKnightMoves(std::array<Move, MAXMOVECOUNT> *moves, int startIndex, Board *board, bool isKingSafe);
    U8 GetKingMoves    (std::array<Move, MAXMOVECOUNT> *moves, int startIndex, Board *board, bool isKingSafe, U64 attackedSquares);
    U64 GetAttackSquares(Board *board);

    bool IsKingSafe(Board *board, U64 tempOccuracyBoard, U64 tempEnemyBoard, U64 tempKingBoard);
    inline bool IsKingSafe(Board *board, U64 tempOccuracyBoard, U64 tempEnemyBoard);
    inline bool IsKingSafe(Board *board, U64 tempOccuracyBoard);
    inline bool IsKingSafe(Board *board);

private:
    Color color;
    Color oppColor;
    Direction up;
    Row doubleRank;
    Row enPassantRank;
    NotEdge notPromotionRank;
    CastlingBlockSquares castlingBlock[2]{};
    CastlingAttackSquares castlingAttack[2]{};

    U64 pawnSingleMove[64] = { 0 };
    U64 pawnDoubleMove[64] = { 0 };

    U8 GetMoves(std::array<Move, MAXMOVECOUNT> *moves, int startIndex, Board *board, U64 pieces, Direction direction, bool isKingSafe);
    void GeneratePawnMoves();

    static U64 GetAttacks(const Board *board, U8 pos, PieceType type);

    static inline void AppendMove(std::array<Move, MAXMOVECOUNT> *moves, int index, U8* moveCount, Move move);
};

inline bool MoveGen::IsKingSafe(Board *board, U64 tempOccuracyBoard, U64 tempEnemyBoard) {
    return IsKingSafe(board, tempOccuracyBoard, tempEnemyBoard, board->GetPiecePos(color, PieceType::King));
}

inline bool MoveGen::IsKingSafe(Board *board, U64 tempOccuracyBoard) {
    return IsKingSafe(board, tempOccuracyBoard, board->GetColorBB(oppColor));
}

inline bool MoveGen::IsKingSafe(Board *board) {
    return IsKingSafe(board, board->GetOccupiedBB());
}

inline void MoveGen::AppendMove(std::array<Move, MAXMOVECOUNT> *moves, int index, U8* moveCount, Move move) {
    moves->at(index) = move;
    (*moveCount)++;
}

#endif // MOVE_GEN
