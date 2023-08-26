#ifndef CHESS_ENGINE_PV
#define CHESS_ENGINE_PV

#include "chess/internal/move.hpp"
#include <optional>
#include <vector>
namespace Chess::Engine {
struct PV {
    PV() : ply(0) {}
    PV(size_t ply) : ply(ply) {}
    PV(size_t ply, std::vector<Move> moves) : ply(ply), moves(moves) {}
    std::vector<Move> moves;
    size_t ply;
};
} // namespace Chess::Engine

#endif
