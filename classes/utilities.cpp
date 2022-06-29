#include "headers/utilities.hh"

PieceChar Utilities::GetPieceChar(char c) {
    return (PieceChar) c;
}

PieceChar Utilities::GetPieceChar(PieceType type, Color color) {
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

PieceType Utilities::GetPieceType(PieceChar pieceChar) {
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

Color Utilities::GetPieceColor(PieceChar pieceChar) {
    if (std::isupper((char) pieceChar))
        return Color::White;
    else
        return Color::Black;
}

Square Utilities::GetSquare(int x, int y) {
    return (Square) (x + 8 * y);
}

Square Utilities::GetSquare(char row, char column) {
    return GetSquare((int) row - 97, (int) column - 49);
}

int Utilities::GetPieceValue(PieceType type) {
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

Color Utilities::GetOppositeColor(Color color) {
    if (color == Color::White)
        return Color::Black;
    else
        return Color::White;
}

// This implementation is from https://www.chessprogramming.org/Population_Count
// Specifically the one titled "Brian Kerninghan"
int Utilities::PopCount(U64 x) {
    int count = 0;
    while (x) {
        count++;
        x &= x - 1;
    }
    return count;
}
int Utilities::LSB(U64 x) {
    U64 lsb = ffsll(x) - 1;
    return lsb;
}

int Utilities::LSB_Pop(U64* x) {
    U64 lsb = LSB(*x);
    *x ^= (C64(0) << lsb);
    return lsb;
}

U64 Utilities::NotEdge(Direction dir) {
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

Column Utilities::GetColumn(int columnIndex) {
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

Column Utilities::GetColumn(Square square) {
    return GetColumn(GetColumnIndex(square));
}

int Utilities::GetColumnIndex(Square square) {
    return (int) square % 8;
}

Row Utilities::GetRow(int columnIndex) {
    switch (columnIndex)
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

Row Utilities::GetRow(Square square) {
    return GetRow(GetRowIndex(square));
}

int Utilities::GetRowIndex(Square square) {
    return std::floor((int) square / 8);
}
