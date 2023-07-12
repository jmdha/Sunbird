#ifndef MOVE_GEN
#define MOVE_GEN

#include <vector>
#include <strings.h>
#include <memory>

#include "internal/bit_shifts.hpp"
#include "internal/constants.hpp"
#include "internal/piece_gen.hpp"
#include "board.hpp"
#include "move.hpp"

class MoveGen {
public:
    explicit MoveGen(Color color);
    U8 GetAllMoves   (std::array<Move, MAXMOVECOUNT> &moves, const std::shared_ptr<Board> &board);
    U8 GetAttackMoves(std::array<Move, MAXMOVECOUNT> &moves, const std::shared_ptr<Board> &board);
    U8 GetQuietMoves(std::array<Move, MAXMOVECOUNT> &moves, const std::shared_ptr<Board> &board);

private:
    std::unique_ptr<PieceGen> pieceGen[PIECECOUNT];
};

#endif // MOVE_GEN
