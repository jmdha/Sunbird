#include <engine/internal/tt.hpp>

namespace Chess::Engine::TT {

size_t size = 0;
Bucket *tt = nullptr;
void Init(size_t tableSize) {
    tableSize *= 1024 * 1024;
    size = tableSize / sizeof(Bucket);
    tt = new Bucket[size];
}

void Clean() {
    if (size != 0) {
        size = 0;
        delete[] tt;
    }
}

ProbeResult Probe(uint64_t key, int depth, int alpha, int beta) {
    ProbeResult result = {false, 0};
    Bucket *bucket = &tt[key % size];

    for (auto *entry : {&(bucket->cold), &(bucket->hot)}) {
        if (entry->key == key) {
            if (entry->depth >= depth) {
                result.found = true;
                if (entry->flag == Flag::Exact)
                    result.score = entry->score;
                else if (entry->flag == Flag::Upper && entry->score <= alpha)
                    result.score = alpha;
                else if (entry->flag == Flag::Lower && entry->score >= beta)
                    result.score = beta;
            }
            result.move = entry->move;
            break;
        }
    }

    return result;
}
const Move *ProbeMove(uint64_t key) {
    Bucket *bucket = &tt[key % size];
    for (auto *entry : {&(bucket->cold), &(bucket->hot)}) {
        if (entry->key == key && entry->flag == Flag::Exact)
            return &entry->move;
    }
    return nullptr;
}

void Save(uint64_t key, int depth, Flag flag, int score, Move move) {
    Bucket &bucket = tt[key % size];
    if (depth >= bucket.cold.depth || bucket.cold.flag == Flag::None) {
        bucket.cold.key = key;
        bucket.cold.depth = depth;
        bucket.cold.score = score;
        bucket.cold.flag = flag;
        bucket.cold.move = move;
    }
    bucket.hot.key = key;
    bucket.hot.depth = depth;
    bucket.hot.score = score;
    bucket.hot.flag = flag;
    bucket.hot.move = move;
}

size_t HashFull() {
    size_t hashfull = 0;
    for (size_t i = 0; i < size; i++) {
        if (tt[i].cold.flag != Flag::None)
            hashfull++;
        if (tt[i].hot.flag != Flag::None)
            hashfull++;
    }
    return hashfull * 1000 / size;
}
} // namespace Chess::Engine::TT
