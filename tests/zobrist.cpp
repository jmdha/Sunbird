#include "zobrist.hpp"
#include "third_party/doctest.h"
#include "types.hpp"
#include <unordered_set>

TEST_CASE("ZOBRIST_ALL_UNIQUE") {
    std::unordered_set<Hash> hashes;
    for (std::size_t i = 0; i < HASH_COUNT; i++)
        hashes.emplace(HASHES[i]);
    CHECK_EQ(hashes.size(), HASH_COUNT);
}
