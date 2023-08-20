#ifndef CHESS_ENGINE_PV
#define CHESS_ENGINE_PV

#include "chess/internal/move.hpp"
#include <optional>
namespace Chess::Engine {
struct PV {
    PV() : _ply(0) {}
    PV(size_t ply) : _ply(ply) {}
    PV(const PV &pv) : _moves(pv._moves), _count(pv._count), _ply(pv._ply) {}
    std::array<Move, MAX_PLY> _moves;
    int _count = 0;
    size_t _ply;
};
}

#endif
