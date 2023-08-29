#include <engine/internal/tt.hpp>
#include <engine/internal/values.hpp>

namespace Chess::Engine::TT {

struct Entry {
    uint64_t key = 0;
    int value = 0;
    Move move = Move();
    uint8_t depth = 0;
    int8_t type = ProbeFail;
};

struct Bucket {
    static const int COUNT = 2;
    std::array<Entry, COUNT> entries;
    Entry &operator[](size_t i) { return entries[i]; }
    const Entry &operator[](size_t i) const { return entries[i]; }
};

// Sized to cacheline (maybe...)
static_assert(sizeof(Bucket) == 32);

size_t count = 0;
Bucket *tt = nullptr;

void Init(size_t tableSize) {
    tableSize *= 1024 * 1024;
    count = tableSize / sizeof(Bucket);
    tt = new Bucket[count];
}

void Clean() {
    if (count != 0) {
        count = 0;
        delete[] tt;
    }
}

size_t HashFull() {
    size_t hashfull = 0;

    for (size_t i = 0; i < count; i++)
        for (size_t t = 0; t < Bucket::COUNT; t++)
            if (tt[i][t].key != 0)
                hashfull++;

    return hashfull * 1000 / count / Bucket::COUNT;
}

namespace {
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
} // namespace

Result Probe(uint64_t key, int depth, int searchDepth, int alpha, int beta) {
    Result result{.score = ProbeFail};
    const Bucket &bucket = tt[key % count];
    for (size_t i = 0; i < Bucket::COUNT; i++) {
        const Entry &entry = bucket[i];
        if (entry.key != key)
            continue;

        result.move = entry.move;

        // Only a single copy is stored of each position
        // As such, if one is found but is of low depth
        // there exists none
        if (entry.depth < depth)
            break;

        const int score = EvalRetrieve(entry.value, searchDepth);
        if (entry.type == ProbeExact)
            result.score = score;
        else if (entry.type == ProbeUpper && score <= alpha)
            result.score = score;
        else if (entry.type == ProbeLower && score >= beta)
            result.score = score;
        break;
    }
    return result;
}

Move ProbeMove(uint64_t key) {
    Bucket &bucket = tt[key % count];
    for (size_t i = 0; i < Bucket::COUNT; i++)
        if (const Entry &entry = bucket[i]; entry.key == key)
            return entry.move;
    return Move();
}

void Clear() {
    for (size_t i = 0; i < count; i++)
        tt[i] = Bucket();
}

void StoreEval(uint64_t key, int depth, int searchDepth, int value, int evalType, Move move) {
    // This check should not be needed
    // It is a temp fix to handle three-fold repitetion
    // If it this can be handled some other way, remove this
    if (searchDepth > 0 && value == 0)
        return;
    Bucket &bucket = tt[key % count];

    // First pass check if key already stored
    // Avoids duplicate storage
    for (int i = 0; i < Bucket::COUNT; i++) {
        Entry &entry = bucket[i];
        if (entry.type == ProbeFail ||   // Unused entry
            i == Bucket::COUNT - 1 ||    // Last entry
            entry.key == key) [[likely]] // Entry override
        {
            entry.type = evalType;
            entry.value = EvalStore(value, searchDepth);
            entry.depth = depth;
            entry.key = key;
            entry.move = move;

            // Found entry
            // Sort bucket
            while (i > 0 && entry.depth > bucket[i--].depth)
                std::swap(entry, bucket[i]); // note decrement in loop condition
            break;
        }
    }
}
} // namespace Chess::Engine::TT
