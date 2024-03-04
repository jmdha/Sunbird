#pragma once

#include "types.hpp"
#include <stdexcept>
#include <strings.h>

namespace Utilities {
constexpr Square GetSquare(int x, int y) { return static_cast<Square>(x + 8 * y); }

constexpr Square GetSquare(char row, char column) { return GetSquare(row - 97, column - 49); }

constexpr Column GetColumn(int columnIndex) {
    switch (columnIndex) {
    case 0: return Column::A;
    case 1: return Column::B;
    case 2: return Column::C;
    case 3: return Column::D;
    case 4: return Column::E;
    case 5: return Column::F;
    case 6: return Column::G;
    case 7: return Column::H;
    default: return Column::None;
    }
}

constexpr Column GetColumnByChar(char column) { return GetColumn(column - 97); }

constexpr size_t GetColumnIndex(Square square) {
    return COLUMN_INDEX.at(static_cast<size_t>(square));
}

constexpr size_t GetColumnIndex(Column col) {
    switch (col) {
    case Column::A: return 0;
    case Column::B: return 1;
    case Column::C: return 2;
    case Column::D: return 3;
    case Column::E: return 4;
    case Column::F: return 5;
    case Column::G: return 6;
    case Column::H: return 7;
    case Column::None: return 8;
    }
    throw std::logic_error("Invalid flow");
}

constexpr Column GetColumn(Square square) {
    return COLUMN_BY_SQUARE.at(static_cast<size_t>(square));
}

constexpr Row GetRow(size_t rowIndex) {
    switch (rowIndex) {
    case 0: return Row::Row1;
    case 1: return Row::Row2;
    case 2: return Row::Row3;
    case 3: return Row::Row4;
    case 4: return Row::Row5;
    case 5: return Row::Row6;
    case 6: return Row::Row7;
    case 7: return Row::Row8;
    default: return Row::None;
    }
}

constexpr size_t GetRowIndex(Square square) {
    return (static_cast<size_t>(square) - (static_cast<size_t>(square) % 8)) / 8;
}

constexpr Row GetRow(Square square) { return GetRow(GetRowIndex(square)); }

constexpr Row GetRowByChar(char row) { return GetRow(static_cast<size_t>(row - '1')); }

inline std::string GetSquareString(Square sq) {
    std::string square;
    const size_t col = GetColumnIndex(sq);
    const size_t row = GetRowIndex(sq);
    square += 'a' + static_cast<char>(col);
    square += '1' + static_cast<char>(row);
    return square;
}

constexpr Direction GetDirection(Square from, Square to) {
    if (GetRowIndex(from) < GetRowIndex(to)) {
        if (GetColumnIndex(from) < GetColumnIndex(to))
            return NORTH_EAST;
        else if (GetColumnIndex(from) > GetColumnIndex(to))
            return NORTH_WEST;
        else
            return NORTH;
    } else if (GetRowIndex(from) > GetRowIndex(to)) {
        if (GetColumnIndex(from) < GetColumnIndex(to))
            return SOUTH_EAST;
        else if (GetColumnIndex(from) > GetColumnIndex(to))
            return SOUTH_WEST;
        else
            return SOUTH;
    } else {
        if (GetColumnIndex(from) < GetColumnIndex(to))
            return EAST;
        else if (GetColumnIndex(from) > GetColumnIndex(to))
            return WEST;
    }
    throw std::invalid_argument("GetDirection called with same from and to");
}

constexpr Color GetTurn(char turn) {
    if (turn == 'w' || turn == 'W')
        return WHITE;
    else
        return BLACK;
}
} // namespace Utilities
