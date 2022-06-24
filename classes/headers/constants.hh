#ifndef CONSTANTS
#define CONSTANTS

#define U64 unsigned long long int
#define C64(x) (U64)1 << (U64) x

#define WIDTH 8
#define HEIGHT 8
#define SQUARECOUNT 64

#define PIECECOUNT 6
#define COLORCOUNT 2

#define PAWNROWWHITE 1
#define PAWNROWBLACK 6

#define BACKROWWHITE 0
#define BACKROWBLACK 7

enum class Color : int {
    White,
    Black,
    None
};

enum class PieceType : int {
    Pawn,
    Knight,
    Bishop,
    Rook,
    Queen,
    King,
    None
};

enum class PieceValue : int {
    Pawn = 10,
    Knight = 30,
    Bishop = 30,
    Rook = 50,
    Queen = 90,
    King = 20000,
    Inf = 30000,
    None
};

enum class PieceChar : char {
    PawnWhite = 'P',
    PawnBlack = 'p',
    KnightWhite = 'N',
    KnightBlack = 'n',
    BishopWhite = 'B',
    BishopBlack = 'b',
    RookWhite = 'R',
    RookBlack = 'r',
    QueenWhite = 'Q',
    QueenBlack = 'q',
    KingWhite = 'K',
    KingBlack = 'k',
    None = 'O'
};

enum class Square : int {
    A1, B1, C1, D1, E1, F1, G1, H1,
    A2, B2, C2, D2, E2, F2, G2, H2,
    A3, B3, C3, D3, E3, F3, G3, H3,
    A4, B4, C4, D4, E4, F4, G4, H4,
    A5, B5, C5, D5, E5, F5, G5, H5,
    A6, B6, C6, D6, E6, F6, G6, H6,
    A7, B7, C7, D7, E7, F7, G7, H7,
    A8, B8, C8, D8, E8, F8, G8, H8,
};

enum class Row : U64 {
    Row1 = 0xff,
    Row2 = 0xff00,
    Row3 = 0xff0000,
    Row4 = 0xff000000,
    Row5 = 0xff00000000,
    Row6 = 0xff0000000000,
    Row7 = 0xff000000000000,
    Row8 = 0xff00000000000000
};

enum class Direction : int {
    North = 8,
    East = 1,
    South = -North,
    West = -East,

    NorthEast = North + East,
    NorthWest = North + West,
    SouthEast = South + East,
    SouthWest = South + West
};

enum class Ray : U64 {
    North = 0x101010101010100,
    East = 0xfe,
    South = 0x1010101010101,
    West = 0x7f
};

enum class NotEdge : U64 {
    North = 0xffffffffffffff,
    East = 0x7f7f7f7f7f7f7f7f,
    South = 0xffffffffffffff00,
    West = 0xfefefefefefefefe
};

enum class NotEdgeKnight : U64 {
    North = 0xffffffffffff,
    East = 0x3f3f3f3f3f3f3f3f,
    South = 0xffffffffffff0000,
    West = 0xfcfcfcfcfcfcfcfc
};

#endif
