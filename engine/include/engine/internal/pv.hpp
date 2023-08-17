#ifndef CHESS_ENGINE_PV
#define CHESS_ENGINE_PV

#include "chess/internal/move.hpp"
namespace Chess::Engine {
struct PV {
    std::array<Move, MAX_PLY> _moves;
    int _count = 0;
};
}

#endif
