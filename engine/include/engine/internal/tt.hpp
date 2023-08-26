#ifndef CHESS_ENGINE_TRANSPOSITION_TABLE
#define CHESS_ENGINE_TRANSPOSITION_TABLE

#include <chess/internal/move.hpp>
#include <optional>
namespace Chess::Engine::TT {
enum struct Flag {
    None,
    Exact,
    Upper,
    Lower,
};

struct Entry {
    uint64_t key;
    uint8_t depth;
    Flag flag = Flag::None;
    int score;
    Move move;
};

struct Bucket {
    Entry cold;
    Entry hot;
};

void Init(size_t tableSize);
void Clean();

struct ProbeResult {
    bool found;
    int score;
    Move move;
};

ProbeResult Probe(uint64_t key, int depth, int alpha, int beta);
const Move* ProbeMove(uint64_t key);
void Save(uint64_t key, int depth, Flag flag, int score, Move move);

size_t HashFull();

} // namespace Chess::Engine::TT

#endif
