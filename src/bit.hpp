#pragma once

#include "types.hpp"
#include <cassert>
#include <cstdint>

/**
 * @brief Returns the number of trailing 0-bits in x, starting from the least significant bit
 * @param x A non-zero unsigned 64 bit number
 * @example ...01 -> 0
 * @example ...10 -> 1
 */
constexpr inline Square lsb(uint64_t x) {
    assert(x != 0);
#if defined(__GNUC__)
    return static_cast<Square>(__builtin_ctzll(x));
#else
    throw std::logic_error("Not implemented.");
#endif
}

/**
 * @brief Returns the number of trailing 0-bits in x, starting from the least significant bit, and
 * removes said bit
 * @param x A non-zero unsigned 16 bit number
 * @example ...01 -> 0
 * @example ...10 -> 1
 */
constexpr inline Square lsb_pop(uint64_t &x) {
    assert(x != 0);
    const int t = lsb(x);
    x ^= static_cast<uint64_t>(1) << t;
    return static_cast<Square>(t);
}

/**
 * @brief Returns the number of leading 0-bits in x, starting from the most significant bit
 * @param x A non-zero unsigned 64 bit number
 * @example 10... -> 0
 * @example 01... -> 1
 */
constexpr inline Square msb(uint64_t x) {
    assert(x != 0);
#if defined(__GNUC__)
    static_assert(sizeof(unsigned long long) == 8);
    return static_cast<Square>(63 ^ __builtin_clzll(x));
#else
    throw std::logic_error("Not implemented.");
#endif
}

/**
 * @brief Returns the number of 1-bits in x
 * @param x An unsigned 64 bit number
 * @example 100... -> 1
 * @example 110... -> 2
 */
constexpr inline int popcount(uint64_t x) {
#if defined(__GNUC__)
    return __builtin_popcountll(x);
#else
    throw std::logic_error("Not implemented.");
#endif
}
