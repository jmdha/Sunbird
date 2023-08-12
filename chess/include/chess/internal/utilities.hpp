#ifndef CHESS_UTILITIES
#define CHESS_UTILITIES

#include <strings.h>
#include <stdexcept>
#include <cmath>
#include <bitset>

#include "constants.hpp"
#include "jank/bit/bit.hpp"
namespace Chess::Utilities {
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

    constexpr BB NotEdge(Direction dir) {
        switch (dir)
        {
        case Direction::North:
            return (BB) NotEdge::North;
        case Direction::East:
            return (BB) NotEdge::East;
        case Direction::South:
            return (BB) NotEdge::South;
        case Direction::West:
            return (BB) NotEdge::West;
        case Direction::NorthEast:
            return (BB) NotEdge::North & (BB) NotEdge::East;
        case Direction::NorthWest:
            return (BB) NotEdge::North & (BB) NotEdge::West;
        case Direction::SouthEast:
            return (BB) NotEdge::South & (BB) NotEdge::East;
        case Direction::SouthWest:
            return (BB) NotEdge::South & (BB) NotEdge::West;
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

    constexpr Column GetColumnByChar(char column) {
        return GetColumn(column - 97);
    }

    constexpr int GetColumnIndex(Square square) {
        return COLUMN_INDEX.at((int)square);
    }

    constexpr int GetColumnIndex(Column col) {
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
                return 8;
        }
        throw std::logic_error("Invalid flow");
    }

    constexpr Column GetColumn(Square square) {
        return COLUMN_BY_SQUARE.at((int)square);
    }

    constexpr Row GetRow(int rowIndex) {
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

    constexpr int GetRowIndex(Square square) {
        return ((int)square - ((int)square % 8)) / 8;
    }

    constexpr Row GetRow(Square square) {
        return GetRow(GetRowIndex(square));
    }

    constexpr Row GetRowByChar(char row) {
        return GetRow(row - '1');
    }

    static std::string GetSquareString(Square sq) {
        std::string square;
        int col = GetColumnIndex(sq);
        int row = GetRowIndex(sq);
        square += 'a' + col;
        square += '1' + row;
        return square;
    }

    constexpr Direction GetDirection(Square from, Square to) {
        if (GetRowIndex(from) < GetRowIndex(to)) {
            if (GetColumnIndex(from) < GetColumnIndex(to))
                return Direction::NorthEast;
            else if (GetColumnIndex(from) > GetColumnIndex(to))
                return Direction::NorthWest;
            else
                return Direction::North;
        } else if (GetRowIndex(from) > GetRowIndex(to)) {
            if (GetColumnIndex(from) < GetColumnIndex(to))
                return Direction::SouthEast;
            else if (GetColumnIndex(from) > GetColumnIndex(to))
                return Direction::SouthWest;
            else
                return Direction::South;
        } else {
            if (GetColumnIndex(from) < GetColumnIndex(to))
                return Direction::East;
            else if (GetColumnIndex(from) > GetColumnIndex(to))
                return Direction::West;
        }
        throw std::invalid_argument("GetDirection called with same from and to");
    }

    constexpr Color GetTurn(char turn) {
        if (turn == 'w' || turn == 'W')
            return Color::White;
        else
            return Color::Black;
    }
}

#endif // UTILITIES
