#include <engine/internal/tt.hpp>
#include <engine/internal/values.hpp>

namespace Chess::Engine::TT {

size_t size = 0;
Entry *tt = nullptr;
void Init(size_t tableSize) {
    tableSize *= 1024 * 1024;
    size = tableSize / sizeof(Entry);
    tt = new Entry[size];
}

void Clean() {
    if (size != 0) {
        size = 0;
        delete[] tt;
    }
}

namespace {
Entry* Get(uint64_t key) {
    return &tt[key % size];
}
int EvalStore(int score, int ply) {
    if (score != Values::INF)
        return score;
    else {
        const int sign = (score > 0) ? 1 : -1;
        return (score * sign + ply) * sign;
    }
}
int EvalRetrieve(int score, int ply) {
    if (score != Values::INF)
        return score;
    else {
        const int sign = (score > 0) ? 1 : -1;
        return (score * sign - ply) * sign;
    }
}
}

Entry* Probe(uint64_t key) {
    return Get(key);
}

int ProbeEval(uint64_t key, int depth, int searchDepth, int alpha, int beta) {
    Entry* entry = Get(key);

    if (entry->key == key && entry->depth >= depth) {
        const int score = EvalRetrieve(entry->value, searchDepth);
        
        if (entry->type == ProbeExact)
            return score;
        else if (entry->type == ProbeUpper && score <= alpha)
            return score;
        else if (entry->type == ProbeLower && score >= beta)
            return score;
    }

    return ProbeFail;
}

Move ProbeMove(uint64_t key) {
    return Get(key)->move;
}

void StoreEval(uint64_t key, int depth, int searchDepth, int value,
               int evalType, Move move) {
    Entry *entry = Get(key);
    entry->type = evalType;
    entry->value = EvalStore(value, searchDepth);
    entry->depth = depth;
    entry->key = key;
    entry->move = move;
}

size_t HashFull() {
    size_t hashfull = 0;
    for (size_t i = 0; i < size; i++) {
        if (tt[i].key != 0)
            hashfull++;
    }
    return hashfull * 1000 / size;
}

void Clear() {
    for (size_t i = 0; i < size; i++)
        tt[i] = Entry();
}
} // namespace Chess::Engine::TT
