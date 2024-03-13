#pragma once

#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <stdexcept>
#include <string>

// CONSTANTS

static constexpr size_t WIDTH           = 8;   // Width of chess board
static constexpr size_t HEIGHT          = 8;   // Height of chess board
static constexpr size_t SQUARE_COUNT    = 64;  // Number of squares on chess board
static constexpr size_t COLOR_COUNT     = 2;   // Number of colors
static constexpr size_t PIECE_COUNT     = 6;   // Amount of pieces
static constexpr size_t DIRECTION_COUNT = 8;   // |Cardinal Directions|
static constexpr size_t MAX_PLY         = 512; // Max number of moves in a game
static constexpr size_t MAX_MOVES       = 256; // Max number of moves for a position
static const std::string FEN_START = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";

// TYPES

using BB = uint64_t;
enum Color { WHITE, BLACK, COLOR_NONE };
enum Piece { PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING, PIECE_NONE };
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
enum class Castling : uint16_t { None, King, Queen, All };
enum class Row : BB {
    Row1 = 0xff,
    Row2 = 0xff00,
    Row3 = 0xff0000,
    Row4 = 0xff000000,
    Row5 = 0xff00000000,
    Row6 = 0xff0000000000,
    Row7 = 0xff000000000000,
    Row8 = 0xff00000000000000,
    None = 0x0
};
enum class Column : BB {
    A    = 0x101010101010101,
    B    = 0x202020202020202,
    C    = 0x404040404040404,
    D    = 0x808080808080808,
    E    = 0x1010101010101010,
    F    = 0x2020202020202020,
    G    = 0x4040404040404040,
    H    = 0x8080808080808080,
    None = 0x0
};
// clang-format off
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
// clang-format on

// CONVERSIONS

static constexpr BB ToBB(Square sq) {
    assert(sq != SQUARE_NONE);
    return (static_cast<BB>(1) << static_cast<int>(sq));
}
static constexpr Color ToColor(char c) { return static_cast<Color>(!!islower(c)); }
static constexpr Piece ToPiece(char c) {
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

// OVERRIDES

static constexpr BB operator&(BB bb, Square sq) {
    assert(sq != SQUARE_NONE);
    return bb & ToBB(sq);
}
static constexpr BB operator|(BB bb, Square sq) {
    assert(sq != SQUARE_NONE);
    return bb | ToBB(sq);
}
static constexpr BB operator^(BB bb, Square sq) {
    assert(sq != SQUARE_NONE);
    return bb ^ ToBB(sq);
}
static constexpr BB operator&(Square sq, BB bb) {
    assert(sq != SQUARE_NONE);
    return bb & sq;
}
static constexpr BB operator|(Square sq, BB bb) {
    assert(sq != SQUARE_NONE);
    return bb | sq;
}
static constexpr BB operator^(Square sq, BB bb) {
    assert(sq != SQUARE_NONE);
    return bb ^ sq;
}
static constexpr BB &operator&=(BB &bb, Square sq) {
    assert(sq != SQUARE_NONE);
    return bb &= ToBB(sq);
}
static constexpr BB &operator|=(BB &bb, Square sq) {
    assert(sq != SQUARE_NONE);
    return bb |= ToBB(sq);
}
static constexpr BB &operator^=(BB &bb, Square sq) {
    assert(sq != SQUARE_NONE);
    return bb ^= ToBB(sq);
}
static constexpr Color operator~(Color color) {
    assert(color == WHITE || color == BLACK);
    return static_cast<Color>(1 ^ static_cast<size_t>(color));
}
static constexpr Castling operator~(Castling val) {
    assert(
        val == Castling::None || val == Castling::King || val == Castling::Queen ||
        val == Castling::All
    );
    switch (val) {
    case Castling::None: return Castling::All;
    case Castling::King: return Castling::Queen;
    case Castling::Queen: return Castling::King;
    case Castling::All: return Castling::None;
    }
    throw std::logic_error("Cannot happen (tm)");
}
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

// COLLECTIONS

static constexpr std::array<Piece, PIECE_COUNT> PIECES = {PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING};
static constexpr std::array<Piece, PIECE_COUNT - 1> NON_PAWNS = {KNIGHT, BISHOP, ROOK, QUEEN, KING};
static const std::array<std::string, COLOR_COUNT> PIECE_CHARS = {"PNBRQK", "pnbrqk"};
// clang-format off
static constexpr std::array<Square, SQUARE_COUNT> SQUARES {
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
static const std::array<std::string, SQUARE_COUNT> SQUARE_NAMES = {
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

static constexpr Square INIT_ROOKPOS[2][2] = {{Square::H1, Square::A1}, {Square::H8, Square::A8}};
static constexpr Square INIT_KINGPOS[2]    = {Square::E1, Square::E8};
static constexpr Square CASTLEPOS_KING[2][2]{{Square::G1, Square::C1}, {Square::G8, Square::C8}};
static constexpr Square CASTLEPOS_ROOK[2][2]{{Square::F1, Square::D1}, {Square::F8, Square::D8}};
static constexpr Row PawnRow[2] = {Row::Row2, Row::Row7};
static constexpr Row PushRow[2] = {Row::Row4, Row::Row5};
static constexpr std::array<Column, 8> COLUMNS{Column::A, Column::B, Column::C, Column::D,
                                               Column::E, Column::F, Column::G, Column::H};
static constexpr std::array<Column, 64> COLUMN_BY_SQUARE = {
    Column::A, Column::B, Column::C, Column::D, Column::E, Column::F, Column::G, Column::H,
    Column::A, Column::B, Column::C, Column::D, Column::E, Column::F, Column::G, Column::H,
    Column::A, Column::B, Column::C, Column::D, Column::E, Column::F, Column::G, Column::H,
    Column::A, Column::B, Column::C, Column::D, Column::E, Column::F, Column::G, Column::H,
    Column::A, Column::B, Column::C, Column::D, Column::E, Column::F, Column::G, Column::H,
    Column::A, Column::B, Column::C, Column::D, Column::E, Column::F, Column::G, Column::H,
    Column::A, Column::B, Column::C, Column::D, Column::E, Column::F, Column::G, Column::H,
    Column::A, Column::B, Column::C, Column::D, Column::E, Column::F, Column::G, Column::H
};
static constexpr std::array<size_t, SQUARE_COUNT> COLUMN_INDEX = {
    0, 1, 2, 3, 4, 5, 6, 7, 0, 1, 2, 3, 4, 5, 6, 7, 0, 1, 2, 3, 4, 5, 6, 7, 0, 1, 2, 3, 4, 5, 6, 7,
    0, 1, 2, 3, 4, 5, 6, 7, 0, 1, 2, 3, 4, 5, 6, 7, 0, 1, 2, 3, 4, 5, 6, 7, 0, 1, 2, 3, 4, 5, 6, 7
};
static constexpr std::array<Direction, 8> DIRECTIONS = {NORTH,      EAST,       SOUTH,
                                                        WEST,       NORTH_EAST, NORTH_WEST,
                                                        SOUTH_EAST, SOUTH_WEST};
static constexpr std::array<BB, 8> EDGES             = {
    static_cast<BB>(Row::Row8),
    static_cast<BB>(Column::H),
    static_cast<BB>(Row::Row1),
    static_cast<BB>(Column::A),

    static_cast<BB>(Row::Row8) | static_cast<BB>(Column::H),
    static_cast<BB>(Row::Row8) | static_cast<BB>(Column::A),
    static_cast<BB>(Row::Row1) | static_cast<BB>(Column::H),
    static_cast<BB>(Row::Row1) | static_cast<BB>(Column::A)
};
static constexpr std::array<std::array<BB, 4>, COLOR_COUNT> CASTLING_BLOCK_SQUARES{
    std::array<BB, 4>{0x0, 0x60, 0xe, 0x6e},
    std::array<BB, 4>{0x0, 0x6000000000000000, 0xe00000000000000, 0x6e00000000000000}
};
static constexpr std::array<std::array<BB, 4>, COLOR_COUNT> CASTLING_ATTACK_SQUARES{
    std::array<BB, 4>{0x0, 0x70, 0x1c, 0x7c},
    std::array<BB, 4>{0x0, 0x7000000000000000, 0x1c00000000000000, 0x7c00000000000000}
};
