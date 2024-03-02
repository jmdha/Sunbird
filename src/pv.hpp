#ifndef CHESS_ENGINE_PV
#define CHESS_ENGINE_PV

#include "move.hpp"
#include <vector>

namespace Engine {
struct PV {
public:
    PV() : _ply(0) {}
    PV(size_t ply) : _ply(ply) {}
    PV(size_t ply, std::vector<Move> moves) : _moves(moves), _ply(ply) {}

    inline bool empty() const { return _moves.empty(); }
    inline size_t size() const { return _moves.size(); }
    inline size_t ply() const { return _ply; }

    inline Move &operator[](size_t i) { return _moves[i]; }
    inline const Move &operator[](size_t i) const { return _moves[i]; }

private:
    std::vector<Move> _moves;
    size_t _ply;
};
} // namespace Engine

#endif
