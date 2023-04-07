#ifndef BIT_SHIFTS
#define BIT_SHIFTS

#include <array>
#include <algorithm>
#include <cassert>

#include "constants.hh"
#include "utilities.hh"

class BitShifts {
public:
    static void Init();

    static inline U64 GetRay(U64 square, DirectionIndex direction) { assert(direction != DirectionIndex::None); return rays[square][(int) direction]; };
    static inline U64 GetSqRay(U8 from, U8 to) { return sqRays[from][to]; };
    static inline U64 GetBehind(U64 from, U64 to) { return behind[from][to]; };
    static inline U64 GetAttacks(U64 square, PieceType pieceType) { return pieceAttacks[(int) pieceType][square]; };
    static inline U64 GetBB(U8 sq, PieceType type) { assert(type != PieceType::None); return blockersAndBeyond[(int) type][sq]; };
    static inline U64 GetRing(U8 sq, U8 offset) { return rings[sq][offset]; };
    static inline U64 Shift(U64 b, Direction dir, int times);
private:
    inline static U64 rays[SQUARECOUNT][DIRECTIONCOUNT] = { 0 };
    inline static U64 sqRays[SQUARECOUNT][SQUARECOUNT] = { 0 };
    inline static U64 pieceAttacks[PIECECOUNT][SQUARECOUNT] = { 0 };
    inline static U64 blockersAndBeyond[PIECECOUNT][SQUARECOUNT] = { 0 };
    inline static U64 behind[SQUARECOUNT][SQUARECOUNT] = { 0 };
    inline static U64 rings[SQUARECOUNT][8] = { 0 };

    inline static U64 GenerateRay(U8 square, DirectionIndex direction);
    inline static U64 GenerateSqRay(U8 from, U8 to);
    inline static U64 GenerateBehind(U8 from, U8 to);
    inline static U64 GenerateBB(U8 sq, PieceType type);
    inline static U64 GenerateRing(U8 sq, U8 offset);
};

inline U64 BitShifts::Shift(U64 b, Direction dir, int times) {
    if ((int) dir > 0)
        return b << (int) dir * times;
    else
        return b >> std::abs((int) dir) * times;
}

U64 BitShifts::GenerateRay(U8 square, DirectionIndex direction) {
    U64 bit = C64((U64) square);
    U64 ray = 0;
    while (bit & (U64) notEdges[(int) direction]) {
        bit = Shift(bit, directions[(int) direction], 1);
        ray |= bit;
    }
    return ray;
}

U64 BitShifts::GenerateSqRay(U8 from, U8 to) {
    DirectionIndex dir = Utilities::GetDirectionIndex((Square) from, (Square) to);
    U64 c = C64(from);
    U64 ray = 0;
    while (c & Utilities::NotEdge(directions[(int) dir])) {
        c = Shift(c, directions[(int) dir], 1);
        ray |= c;
    }
    return ray;
}

U64 BitShifts::GenerateBehind(U8 from, U8 to) {
    return rays[to][(int) Utilities::GetDirectionIndex((Square) from, (Square) to)];
}

U64 BitShifts::GenerateBB(unsigned short sq, PieceType type) {
    if ((C64(sq) & EDGE) == 0)
        return GetAttacks(sq, type) & (~EDGE);

    if (C64(sq) & CORNER)
        return GetAttacks(sq, type) & (~CORNER);

    if (C64(sq) & (U64) Row::Row1)
        return GetAttacks(sq, type) & (~( (U64) Row::Row8 | (U64) Column::A | (U64) Column::H));
    if (C64(sq) & (U64) Row::Row8)
        return GetAttacks(sq, type) & (~( (U64) Row::Row1 | (U64) Column::A | (U64) Column::H));
    if (C64(sq) & (U64) Column::A)
        return GetAttacks(sq, type) & (~( (U64) Row::Row1 | (U64) Row::Row8 | (U64) Column::H));
    if (C64(sq) & (U64) Column::H)
        return GetAttacks(sq, type) & (~( (U64) Row::Row1 | (U64) Row::Row8 | (U64) Column::A));
    throw std::logic_error("Unexpected square");
}

U64 BitShifts::GenerateRing(U8 sq, U8 offset) {
    U64 ring = 0;

    for (const auto & dir1 : { Direction::South, Direction::North }) {
        if (dir1 == Direction::South) {
            if (Utilities::GetRowIndex((Square) sq) - offset < 0)
                continue;
        } else if (Utilities::GetRowIndex((Square) sq) + offset > 7)
                continue;
        U64 dot = Shift(C64(sq), dir1, offset);
        ring |= dot;
        for (const auto &dir2: {Direction::West, Direction::East}) {
            for (int i = 0; i <= offset; i++) {
                U64 dd = Shift(dot, dir2, i);
                if (dd & ~(U64) notEdges[(int) Utilities::GetDirectionIndex(dir2)]) {
                    ring |= dd;
                    break;
                }
                ring |= dd;
            }
        }
    }

    for (const auto & dir1 : { Direction::West, Direction::East }) {
        if (dir1 == Direction::West) {
            if (Utilities::GetColumnIndex((Square) sq) - offset < 0)
                continue;
        } else if (Utilities::GetColumnIndex((Square) sq) + offset > 7)
                continue;
        U64 dot = Shift(C64(sq), dir1, offset);
        ring |= dot;
        for (const auto &dir2: {Direction::South, Direction::North}) {
            for (int i = 0; i <= offset; i++) {
                U64 dd = Shift(dot, dir2, i);
                if (dd & ~(U64) notEdges[(int) Utilities::GetDirectionIndex(dir2)]) {
                    ring |= dd;
                    break;
                }
                ring |= dd;
            }
        }
    }


    return ring;
}

#endif // BIT_SHIFTS
