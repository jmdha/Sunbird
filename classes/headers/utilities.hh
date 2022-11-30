#ifndef UTILITIES
#define UTILITIES

#include <strings.h>
#include <stdexcept>
#include <math.h>
#include <bitset>

#include "constants.hh"
namespace Utilities {
    static inline PieceChar GetPieceChar(char c) {
        return (PieceChar) c;
    }

    static inline PieceChar GetPieceChar(PieceType type, Color color) {
        switch(type) {
        case PieceType::Bishop:
            return (color == Color::White) ? PieceChar::BishopWhite : PieceChar::BishopBlack;
        case PieceType::King:
            return (color == Color::White) ? PieceChar::KingWhite : PieceChar::KingBlack;
        case PieceType::Knight:
            return (color == Color::White) ? PieceChar::KnightWhite : PieceChar::KnightBlack;
        case PieceType::Pawn:
            return (color == Color::White) ? PieceChar::PawnWhite : PieceChar::PawnBlack;
        case PieceType::Queen:
            return (color == Color::White) ? PieceChar::QueenWhite : PieceChar::QueenBlack;
        case PieceType::Rook:
            return (color == Color::White) ? PieceChar::RookWhite : PieceChar::RookBlack;
        default:
            return PieceChar::None;
        }
    }

    static inline PieceType GetPieceType(PieceChar pieceChar) {
        switch (pieceChar)
        {
        case PieceChar::BishopBlack:
        case PieceChar::BishopWhite:
            return PieceType::Bishop;

        case PieceChar::KingBlack :
        case PieceChar::KingWhite:
            return PieceType::King;

        case PieceChar::KnightBlack:
        case PieceChar::KnightWhite:
            return PieceType::Knight;

        case PieceChar::PawnBlack:
        case PieceChar::PawnWhite :
            return PieceType::Pawn;

        case PieceChar::QueenBlack:
        case PieceChar::QueenWhite:
            return PieceType::Queen;

        case PieceChar::RookBlack:
        case PieceChar::RookWhite:
            return PieceType::Rook;

        default:
            return PieceType::None;
        }
    }

    static inline Color GetPieceColor(PieceChar pieceChar) {
        if (std::isupper((char) pieceChar))
            return Color::White;
        else
            return Color::Black;
    }

    static inline Square GetSquare(int x, int y) {
        return (Square) (x + 8 * y);
    }

    static inline Square GetSquare(char row, char column) {
        return GetSquare((int) row - 97, (int) column - 49);
    }

    static inline int GetPieceValue(PieceType type) {
        switch(type) {
        case PieceType::Bishop:
            return (int) PieceValue::Bishop;
        case PieceType::King:
            return (int) PieceValue::King;
        case PieceType::Knight:
            return (int) PieceValue::Knight;
        case PieceType::Pawn:
            return (int) PieceValue::Pawn;
        case PieceType::Queen:
            return (int) PieceValue::Queen;
        case PieceType::Rook:
            return (int) PieceValue::Rook;
        default:
            return (int) PieceValue::None;
        }
    }

    static inline Color GetOppositeColor(Color color) {
        if (color == Color::White)
            return Color::Black;
        else
            return Color::White;
    }

    // Returns the amount of bits set to 1
    // e.g. 0110 => 2 and 1000 => 1
    // This implementation is from https://www.chessprogramming.org/Population_Count
    // Specifically the one titled "Brian Kerninghan"
    static inline int PopCount(U64 x) {
        int count = 0;
        while (x) {
            count++;
            x &= x - 1;
        }
        return count;
    }

    static inline int LSB(U64 x) {
        U64 lsb = ffsll(x) - 1;
        return lsb;
    }

    // Returns the least significant set bit, and pops it
    // e.g. 0110 => 1 and 1000 => 3 and 1001 => 0
    static inline int LSB_Pop(U64* x) {
        U64 lsb = LSB(*x);
        *x ^= (C64(0) << lsb);
        return lsb;
    }

    static inline int MSB(U64 x) {
        int index = 0;
        while (1 < x) {
            x = x >> 1;
            index++;
        }
        return index;
    }

    static inline U64 NotEdge(Direction dir) {
        switch (dir)
        {
        case Direction::North:
            return (U64) NotEdge::North;
        case Direction::East:
            return (U64) NotEdge::East;
        case Direction::South:
            return (U64) NotEdge::South;
        case Direction::West:
            return (U64) NotEdge::West;
        case Direction::NorthEast:
            return (U64) NotEdge::North & (U64) NotEdge::East;
        case Direction::NorthWest:
            return (U64) NotEdge::North & (U64) NotEdge::West;
        case Direction::SouthEast:
            return (U64) NotEdge::South & (U64) NotEdge::East;
        case Direction::SouthWest:
            return (U64) NotEdge::South & (U64) NotEdge::West;
        }
        return 0;
    }

    static inline Column GetColumn(int columnIndex) {
        switch (columnIndex)
        {
        case 0:
            return Column::A;
        case 1:
            return Column::B;
        case 2:
            return Column::C;
        case 3:
            return Column::D;
        case 4:
            return Column::E;
        case 5:
            return Column::F;
        case 6:
            return Column::G;
        case 7:
            return Column::H;
        }
        return Column::None;
    }

    static inline int GetColumnIndex(Square square) {
        return (int) square % 8;
    }

    static inline Column GetColumn(Square square) {
        return GetColumn(GetColumnIndex(square));
    }

    static inline Row GetRow(int rowIndex) {
        switch (rowIndex)
        {
        case 0:
            return Row::Row1;
        case 1:
            return Row::Row2;
        case 2:
            return Row::Row3;
        case 3:
            return Row::Row4;
        case 4:
            return Row::Row5;
        case 5:
            return Row::Row6;
        case 6:
            return Row::Row7;
        case 7:
            return Row::Row8;
        }
        return Row::None;
    }

    static inline int GetRowIndex(Square square) {
        return std::floor((int) square / 8);
    }

    static inline Row GetRow(Square square) {
        return GetRow(GetRowIndex(square));
    }

    // Converts 1-8 to bitshift equivilant
    static inline Direction GetDirection(int i) {
        switch (i)
        {
        case 1:
            return Direction::SouthWest;
        case 2:
            return Direction::South;
        case 3:
            return Direction::SouthEast;
        case 4:
            return Direction::West;
        case 6:
            return Direction::East;
        case 7:
            return Direction::NorthWest;
        case 8:
            return Direction::North;
        case 9:
            return Direction::NorthEast;
        default:
            return Direction::None;
        }
    }

    static inline void AddSquares(U64 (*attackSquares)[2], U64 addedSquares) {
        while (addedSquares) {
            int lsb = LSB_Pop(&addedSquares);
            if (lsb & (*attackSquares)[0])
                (*attackSquares)[1] |= lsb;
            else
                (*attackSquares)[0] |= lsb;
        }
    }
    static inline void RemoveSquares(U64 (*attackSquares)[2], U64 removedSquares) {
        while (removedSquares) {
            int lsb = LSB_Pop(&removedSquares);
            if (lsb & (*attackSquares)[1])
                (*attackSquares)[1] ^= lsb;
            else
                (*attackSquares)[0] ^= lsb;
        }
    }

    static inline std::string GetSquareString(Square sq) {
        std::string square = "";
        int col = GetColumnIndex(sq);
        int row = GetRowIndex(sq);
        square += 'a' + col;
        square += '1' + row;
        return square;
    }
}

#endif
