#ifndef CHESS_ENGINE_TRANSPOSITION_TABLE
#define CHESS_ENGINE_TRANSPOSITION_TABLE

#include <chess/internal/move.hpp>
#include <optional>
namespace Chess::Engine::TT {
static const int ProbeFail = -1;
static const int ProbeExact = 0;
static const int ProbeLower = 1;
static const int ProbeUpper = 2;

struct Entry {
    uint64_t key = 0;
    int value = 0;
    Move move = Move();
    uint8_t depth = 0;
    uint8_t type = -1;
};

void Init(size_t tableSize);
void Clean();

Entry *Probe(uint64_t key);
int ProbeEval(uint64_t key, int depth, int searchDepth, int alpha, int beta);
Move ProbeMove(uint64_t key);

void StoreEval(uint64_t key, int depth, int searchDepth, int value,
               int evalType, Move move);

size_t HashFull();
void Clear();

} // namespace Chess::Engine::TT

#endif
