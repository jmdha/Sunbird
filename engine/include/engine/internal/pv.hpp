#ifndef CHESS_ENGINE_PV
#define CHESS_ENGINE_PV

#include "chess/internal/move.hpp"
namespace Chess::Engine {
struct PV {
    PV() : _ply(0) {}
    PV(size_t ply) : _ply(ply) {}
    std::array<Move, MAX_PLY> _moves;
    int _count = 0;
    const size_t _ply;
};
}

#endif
