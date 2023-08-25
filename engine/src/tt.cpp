#include <engine/internal/tt.hpp>

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

ProbeResult Probe(uint64_t key, int depth, int alpha, int beta) {
    ProbeResult result = {false, 0};
    Entry &entry = tt[key % size];

    if (entry.key == key) {
        if (entry.depth >= depth) {
        result.found = true;
        if (entry.flag == Flag::Exact)
            result.score = entry.score;
        else if (entry.flag == Flag::Upper && entry.score <= alpha)
            result.score = alpha;
        else if (entry.flag == Flag::Lower && entry.score > beta)
            result.score = beta;
        }
        result.move = entry.move;
    }

    return result;
}

void Save(uint64_t key, int depth, Flag flag, int score, Move move) {
    Entry *entry = &tt[key % size];
    entry->key = key;
    entry->depth = depth;
    entry->score = score;
    entry->flag = flag;
    entry->move = move;
}
} // namespace Chess::Engine::TT
