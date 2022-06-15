#ifndef CONSTANTS
#define CONSTANTS

#define WIDTH 8
#define HEIGHT 8

#define PAWNROWWHITE 1
#define PAWNROWBLACK 6

#define BACKROWWHITE 0
#define BACKROWBLACK 7

enum class Color : int {
    None,
    White,
    Black
};

enum class PieceType : int {
    None,
    Pawn,
    Knight,
    Bishop,
    Rook,
    Queen,
    King
};

enum class PieceValue : int {
    None,
    Pawn = 10,
    Knight = 30,
    Bishop = 30,
    Rook = 50,
    Queen = 90,
    King = 99999
};

enum class PieceChar : char {
    None,
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
    KingBlack = 'k'
};

#endif