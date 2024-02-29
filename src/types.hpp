#pragma once

#include <array>
#include <cassert>
#include <cstdint>
#include <string>

static const std::string START_FEN = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

// Width of a chess board
#define WIDTH 8
// Height of a chess board
#define HEIGHT 8
// Total number of squares on a chess board
#define SQUARE_COUNT (WIDTH * HEIGHT)

// How many colors there are
#define COLOR_COUNT 2
// How many types of pieces there are
#define PIECE_COUNT 6

// Maximum number of possible moves at any given position
#define MAX_MOVES 256
// Maximum number of moves from start pos
#define MAX_PLY 256

// A bitmask of a chess board, where a one corresponds to the index of the
// square
typedef uint64_t BB;
// A semi-unique hash of a chess position
typedef uint64_t Hash;

enum Color { WHITE, BLACK, COLOR_NONE };
enum Piece { PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING, PIECE_NONE };
enum Column { COL_A, COL_B, COL_C, COL_D, COL_E, COL_F, COL_G, COL_H, COL_NONE };
enum Row { ROW_1, ROW_2, ROW_3, ROW_4, ROW_5, ROW_6, ROW_7, ROW_8, ROW_NONE };
enum class Castling { None, King, Queen, Both };
enum Direction {
    NORTH,
    EAST,
    SOUTH,
    WEST,

    NORTH_EAST,
    NORTH_WEST,
    SOUTH_EAST,
    SOUTH_WEST,
    DIRECTION_NONE
};
#define DIRECTION_COUNT 8
constexpr std::array<Direction, DIRECTION_COUNT> DIRECTIONS = {NORTH,      EAST,       SOUTH,
                                                               WEST,       NORTH_EAST, NORTH_WEST,
                                                               SOUTH_EAST, SOUTH_WEST};

constexpr std::array<Piece, PIECE_COUNT> PIECES   = {PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING};
static const std::string PIECE_CHARS[COLOR_COUNT] = {"PNBRQK", "pnbrqk"};
// clang-format off

// Squares of a chess board
enum Square {
    A1,B1,C1,D1,E1,F1,G1,H1,
    A2,B2,C2,D2,E2,F2,G2,H2,
    A3,B3,C3,D3,E3,F3,G3,H3,
    A4,B4,C4,D4,E4,F4,G4,H4,
    A5,B5,C5,D5,E5,F5,G5,H5,
    A6,B6,C6,D6,E6,F6,G6,H6,
    A7,B7,C7,D7,E7,F7,G7,H7,
    A8,B8,C8,D8,E8,F8,G8,H8,
    SQUARE_NONE
};

constexpr std::array<Square, SQUARE_COUNT> SQUARES {
    A1,B1,C1,D1,E1,F1,G1,H1,
    A2,B2,C2,D2,E2,F2,G2,H2,
    A3,B3,C3,D3,E3,F3,G3,H3,
    A4,B4,C4,D4,E4,F4,G4,H4,
    A5,B5,C5,D5,E5,F5,G5,H5,
    A6,B6,C6,D6,E6,F6,G6,H6,
    A7,B7,C7,D7,E7,F7,G7,H7,
    A8,B8,C8,D8,E8,F8,G8,H8,
};

// The name of each square index
static inline std::array<std::string, SQUARE_COUNT> SQUARE_NAMES = {
    "a1","b1","c1","d1","e1","f1","g1","h1",
    "a2","b2","c2","d2","e2","f2","g2","h2",
    "a3","b3","c3","d3","e3","f3","g3","h3",
    "a4","b4","c4","d4","e4","f4","g4","h4",
    "a5","b5","c5","d5","e5","f5","g5","h5",
    "a6","b6","c6","d6","e6","f6","g6","h6",
    "a7","b7","c7","d7","e7","f7","g7","h7",
    "a8","b8","c8","d8","e8","f8","g8","h8"
};
// clang-format on

// Converts a character to its corresponding piece
constexpr static inline Piece ToPiece(char c) {
    c = tolower(c);
    switch (c) {
    case 'p': return PAWN;
    case 'n': return KNIGHT;
    case 'b': return BISHOP;
    case 'r': return ROOK;
    case 'q': return QUEEN;
    case 'k': return KING;
    default: return PIECE_NONE;
    }
}

// Converts a square to the column in which it besides
constexpr static inline Column ToCol(Square sq) {
    assert(sq != SQUARE_NONE);
    return static_cast<Column>(sq % 8);
}

// Converts a char index to column
constexpr static inline Column ToCol(char c) {
    c = tolower(c);
    assert(c >= 97 && c <= 104);
    return static_cast<Column>(c - 97);
}

// Converts a square to the row in which it besides
constexpr static inline Row ToRow(Square sq) {
    assert(sq != SQUARE_NONE);
    return static_cast<Row>((sq - sq % 8) / 8);
}

// Converts a row index to column
constexpr static inline Row ToRow(char c) {
    c = tolower(c);
    assert(c >= 49 && c <= 56);
    return static_cast<Row>(c - 49);
}

constexpr static inline Square ToSquare(Column col, Row row) {
    assert(col != COL_NONE);
    assert(row != ROW_NONE);
    return static_cast<Square>(8 * row + col);
}

// Converts a square to a bitboards where only that square is a one
constexpr static inline BB ToBB(Square sq) {
    assert(sq != SQUARE_NONE);
    return (static_cast<BB>(1) << static_cast<int>(sq));
}

constexpr static inline Color operator!(Color color) {
    assert(color != COLOR_NONE);
    return static_cast<Color>(1 ^ static_cast<int>(color));
}

constexpr static inline BB operator&(BB bb, Square sq) { return bb & ToBB(sq); }
constexpr static inline BB operator|(BB bb, Square sq) { return bb | ToBB(sq); }
constexpr static inline BB operator^(BB bb, Square sq) { return bb ^ ToBB(sq); }
constexpr static inline BB operator&(Square sq, BB bb) { return bb & sq; }
constexpr static inline BB operator|(Square sq, BB bb) { return bb | sq; }
constexpr static inline BB operator^(Square sq, BB bb) { return bb ^ sq; }
constexpr static inline BB &operator&=(BB &bb, Square sq) { return bb &= ToBB(sq); }
constexpr static inline BB &operator|=(BB &bb, Square sq) { return bb |= ToBB(sq); }
constexpr static inline BB &operator^=(BB &bb, Square sq) { return bb ^= ToBB(sq); }

constexpr static inline Castling operator&(Castling a, Castling b) {
    return static_cast<Castling>(static_cast<size_t>(a) & static_cast<size_t>(b));
}
constexpr static inline Castling operator|(Castling a, Castling b) {
    return static_cast<Castling>(static_cast<size_t>(a) | static_cast<size_t>(b));
}
constexpr static inline Castling operator^(Castling a, Castling b) {
    return static_cast<Castling>(static_cast<size_t>(a) ^ static_cast<size_t>(b));
}
constexpr static inline Castling &operator&=(Castling &a, Castling b) { return a = a & b; }
constexpr static inline Castling &operator|=(Castling &a, Castling b) { return a = a | b; }
constexpr static inline Castling &operator^=(Castling &a, Castling b) { return a = a ^ b; }
