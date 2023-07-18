#ifndef UTILITIES
#define UTILITIES

#include <strings.h>
#include <stdexcept>
#include <cmath>
#include <bitset>

#include "constants.hpp"
namespace Utilities {
    static PieceChar GetPieceChar(char c) {
        return (PieceChar) c;
    }

    static PieceChar GetPieceChar(PieceType type, Color color) {
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

    constexpr PieceType GetPieceType(PieceChar pieceChar) {
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

    constexpr Color GetPieceColor(PieceChar pieceChar) {
        if (std::isupper((char) pieceChar))
            return Color::White;
        else
            return Color::Black;
    }

    constexpr Square GetSquare(int x, int y) {
        return (Square) (x + 8 * y);
    }

    constexpr Square GetSquare(char row, char column) {
        return GetSquare((int) row - 97, (int) column - 49);
    }

    constexpr Square GetSquare(Row row, Column col) {
        return GetSquare((int) row, (int) col);
    }

    constexpr Color GetOppositeColor(Color color) {
        if (color == Color::White)
            return Color::Black;
        else
            return Color::White;
    }

    // Returns the amount of bits set to 1
    // e.g. 0110 => 2 and 1000 => 1
    // This implementation is from https://www.chessprogramming.org/Population_Count
    // Specifically the one titled "Brian Kerninghan"
    constexpr U8 PopCount(U64 x) {
        U8 count = 0;
        while (x) {
            ++count;
            x &= x - 1;
        }
        return count;
    }

    constexpr U64 LSB(U64 x) {
        return ffsll(x) - 1;
    }

    // Returns the least significant set bit, and pops it
    // e.g. 0110 => 1 and 1000 => 3 and 1001 => 0
    constexpr U64 LSB_Pop(U64* x) {
        U64 lsb = LSB(*x);
        *x ^= (C64(0) << lsb);
        return lsb;
    }

    constexpr U64 MSB(U64 x) {
        U64 index = 0;
        while (1 < x) {
            x = x >> 1;
            ++index;
        }
        return index;
    }

    constexpr U64 NotEdge(Direction dir) {
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
        throw std::logic_error("Invalid flow");
    }

    constexpr Column GetColumn(int columnIndex) {
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

    constexpr U8 GetColumnIndex(Square square) {
        return COLUMN_INDEX.at((U8)square);
    }

    constexpr U8 GetColumnIndex(Column col) {
        switch (col) {

            case Column::A:
                return 0;
            case Column::B:
                return 1;
            case Column::C:
                return 2;
            case Column::D:
                return 3;
            case Column::E:
                return 4;
            case Column::F:
                return 5;
            case Column::G:
                return 6;
            case Column::H:
                return 7;
            case Column::None:
                throw std::logic_error("Invalid argument");
        }
        throw std::logic_error("Invalid flow");
    }

    constexpr Column GetColumn(Square square) {
        return COLUMN_BY_SQUARE.at((U8)square);
    }

    constexpr Row GetRow(U8 rowIndex) {
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

    constexpr U8 GetRowIndex(Square square) {
        return ((U8)square - ((U8)square % 8)) / 8;
    }

    constexpr Row GetRow(Square square) {
        return GetRow(GetRowIndex(square));
    }

    static std::string GetSquareString(Square sq) {
        std::string square;
        U8 col = GetColumnIndex(sq);
        U8 row = GetRowIndex(sq);
        square += 'a' + col;
        square += '1' + row;
        return square;
    }

    constexpr DirectionIndex GetDirectionIndex(Direction direction) {
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
            default:
                throw std::invalid_argument("Unexpected direction in GetDirectionIndex: " + std::to_string((int)direction));
        }
        throw std::logic_error("Invalid flow");
    }

    constexpr DirectionIndex GetDirectionIndex(Square from, Square to) {
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

    constexpr Color GetTurn(char turn) {
        if (turn == 'w' || turn == 'W')
            return Color::White;
        else
            return Color::Black;
    }
}

#endif // UTILITIES
