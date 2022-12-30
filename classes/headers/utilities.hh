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

    static inline U64 GetPieceValue(PieceType type) {
        switch(type) {
        case PieceType::Bishop:
            return (U64) PieceValue::Bishop;
        case PieceType::King:
            return (U64) PieceValue::King;
        case PieceType::Knight:
            return (U64) PieceValue::Knight;
        case PieceType::Pawn:
            return (U64) PieceValue::Pawn;
        case PieceType::Queen:
            return (U64) PieceValue::Queen;
        case PieceType::Rook:
            return (U64) PieceValue::Rook;
        default:
            return (U64) PieceValue::None;
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
    static inline U8 PopCount(U64 x) {
        U8 count = 0;
        while (x) {
            count++;
            x &= x - 1;
        }
        return count;
    }

    static inline U64 LSB(U64 x) {
        return ffsll(x) - 1;
    }

    // Returns the least significant set bit, and pops it
    // e.g. 0110 => 1 and 1000 => 3 and 1001 => 0
    static inline U64 LSB_Pop(U64* x) {
        U64 lsb = LSB(*x);
        *x ^= (C64(0) << lsb);
        return lsb;
    }

    static inline U64 MSB(U64 x) {
        U64 index = 0;
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
        case Direction::None:
            throw std::invalid_argument("No direction given");
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
        default:
            return Column::None;
        }
    }

    static inline U8 GetColumnIndex(Square square) {
        return (U8) square % 8;
    }

    static inline Column GetColumn(Square square) {
        return GetColumn(GetColumnIndex(square));
    }

    static inline Row GetRow(U8 rowIndex) {
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
        default:
            return Row::None;
        }
    }

    static inline U8 GetRowIndex(Square square) {
        return std::floor((U8) square / 8);
    }

    static inline Row GetRow(Square square) {
        return GetRow(GetRowIndex(square));
    }

    static inline std::string GetSquareString(Square sq) {
        std::string square;
        U8 col = GetColumnIndex(sq);
        U8 row = GetRowIndex(sq);
        square += 'a' + col;
        square += '1' + row;
        return square;
    }

    static inline DirectionIndex GetDirectionIndex(Direction direction) {
        switch (direction) {
            case Direction::North:
                return DirectionIndex::North;
            case Direction::East:
                return DirectionIndex::East;
            case Direction::South:
                return DirectionIndex::South;
            case Direction::West:
                return DirectionIndex::West;
            case Direction::NorthWest:
                return DirectionIndex::NorthWest;
            case Direction::NorthEast:
                return DirectionIndex::NorthEast;
            case Direction::SouthWest:
                return DirectionIndex::SouthWest;
            case Direction::SouthEast:
                return DirectionIndex::SouthEast;
            case Direction::None:
                return DirectionIndex::None;
        }
    }

    static inline DirectionIndex GetDirectionIndex(Square from, Square to) {
        if (GetRowIndex(from) < GetRowIndex(to)) {
            if (GetColumnIndex(from) < GetColumnIndex(to))
                return DirectionIndex::NorthEast;
            else if (GetColumnIndex(from) > GetColumnIndex(to))
                return DirectionIndex::NorthWest;
            else
                return DirectionIndex::North;
        } else if (GetRowIndex(from) > GetRowIndex(to)) {
            if (GetColumnIndex(from) < GetColumnIndex(to))
                return DirectionIndex::SouthEast;
            else if (GetColumnIndex(from) > GetColumnIndex(to))
                return DirectionIndex::SouthWest;
            else
                return DirectionIndex::South;
        } else {
            if (GetColumnIndex(from) < GetColumnIndex(to))
                return DirectionIndex::East;
            else if (GetColumnIndex(from) > GetColumnIndex(to))
                return DirectionIndex::West;
        }
        return DirectionIndex::None;
    }
}

#endif // UTILITIES
