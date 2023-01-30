#ifndef MOVE_GEN
#define MOVE_GEN

#include <vector>
#include <strings.h>

#include "board.hh"
#include "bit_shifts.hh"
#include "constants.hh"
#include "move.hh"
#include "piece_gen.hh"

class MoveGen {
public:
    explicit MoveGen(Color color);
    U8 GetAllMoves   (std::array<Move, MAXMOVECOUNT> *moves, Board *board, U64 attackedSquares);
    U8 GetAttackMoves(std::array<Move, MAXMOVECOUNT> *moves, Board *board, U64 attackedSquares);

private:
    Color color;
    PieceGen *pieceGen[PIECECOUNT];
    static void RemoveIllegal(std::array<Move, MAXMOVECOUNT> *moves, U8 *moveCount, Board *board, U64 attackedSquares);
    static bool IsIllegal(Board *board, unsigned long long int attackedSquares, Move move);
};

#endif // MOVE_GEN
