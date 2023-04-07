#ifndef MOVE_GEN
#define MOVE_GEN

#include <vector>
#include <strings.h>
#include <memory>

#include "board.hh"
#include "bit_shifts.hh"
#include "constants.hh"
#include "move.hh"
#include "piece_gen.hh"

class MoveGen {
public:
    explicit MoveGen(Color color);
    U8 GetAllMoves   (std::array<Move, MAXMOVECOUNT> &moves, const std::shared_ptr<Board> &board, U64 attackedSquares);
    U8 GetAttackMoves(std::array<Move, MAXMOVECOUNT> &moves, const std::shared_ptr<Board> &board, U64 attackedSquares);
    U8 GetQuietMoves(std::array<Move, MAXMOVECOUNT> &moves, const std::shared_ptr<Board> &board, U64 attackedSquares);

private:
    std::unique_ptr<PieceGen> pieceGen[PIECECOUNT];
};

#endif // MOVE_GEN
