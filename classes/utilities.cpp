#include "headers/utilities.h"

PieceChar Utilities::GetPieceChar(char c) {
    return (PieceChar) c;
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
