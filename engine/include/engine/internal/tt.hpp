#ifndef CHESS_ENGINE_TRANSPOSITION_TABLE
#define CHESS_ENGINE_TRANSPOSITION_TABLE

#include <chess/internal/move.hpp>
namespace Chess::Engine::TT {
#define DEFAULT_HASH_SIZE 16

static const int ProbeFail = -1;
static const int ProbeExact = 0;
static const int ProbeLower = 1;
static const int ProbeUpper = 2;

struct Result {
    int score = -1;
    Move move = Move();
};

// startup / cleanup

void Init(size_t tableSize = DEFAULT_HASH_SIZE);
void Clean();

// access

size_t HashFull();

Result Probe(uint64_t key, int depth, int searchDepth, int alpha, int beta);
Move ProbeMove(uint64_t key);

// modifiers

void Clear();
void StoreEval(uint64_t key, int depth, int searchDepth, int value, int evalType, Move move);

} // namespace Chess::Engine::TT

#endif
