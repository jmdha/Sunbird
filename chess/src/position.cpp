#include <chess/internal/position.hpp>

using namespace Chess;

bool Position::IsKingSafe(Color turn) const {
    Square king = GetKing(turn);

    BB occ = GetPieces();
    BB pawns = GetPieces(~turn, PieceType::Pawn);
    BB knights = GetPieces(~turn, PieceType::Knight);
    BB bishops = (GetPieces(~turn, PieceType::Bishop) | GetPieces(~turn, PieceType::Queen));
    BB rooks = (GetPieces(~turn, PieceType::Rook) | GetPieces(~turn, PieceType::Queen));

    // Checking jumpers
    if (PawnAttacks(king, turn) & pawns)
        return false;
    if (Attacks(king, PieceType::Knight) & knights)
        return false;

    if (Ray(king, Direction::North) & rooks) [[unlikely]]
        if (First(Ray(king, Direction::North) & rooks) == First(Ray(king, Direction::North) & occ))
            [[unlikely]]
            return false;
    if (Ray(king, Direction::East) & rooks) [[unlikely]]
        if (First(Ray(king, Direction::East) & rooks) == First(Ray(king, Direction::East) & occ))
            [[unlikely]]
            return false;
    if (Ray(king, Direction::South) & rooks) [[unlikely]]
        if (Last(Ray(king, Direction::South) & rooks) == Last(Ray(king, Direction::South) & occ))
            [[unlikely]]
            return false;
    if (Ray(king, Direction::West) & rooks) [[unlikely]]
        if (Last(Ray(king, Direction::West) & rooks) == Last(Ray(king, Direction::West) & occ))
            [[unlikely]]
            return false;

    if (Ray(king, Direction::NorthEast) & bishops) [[unlikely]]
        if (First(Ray(king, Direction::NorthEast) & bishops) ==
            First(Ray(king, Direction::NorthEast) & occ)) [[unlikely]]
            return false;
    if (Ray(king, Direction::NorthWest) & bishops) [[unlikely]]
        if (First(Ray(king, Direction::NorthWest) & bishops) ==
            First(Ray(king, Direction::NorthWest) & occ)) [[unlikely]]
            return false;
    if (Ray(king, Direction::SouthEast) & bishops) [[unlikely]]
        if (Last(Ray(king, Direction::SouthEast) & bishops) ==
            Last(Ray(king, Direction::SouthEast) & occ)) [[unlikely]]
            return false;
    if (Ray(king, Direction::SouthWest) & bishops) [[unlikely]]
        if (Last(Ray(king, Direction::SouthWest) & bishops) ==
            Last(Ray(king, Direction::SouthWest) & occ)) [[unlikely]]
            return false;

    return true;
}

BB Position::GenerateAttackSquares(Color color) const {
    const BB allPieces = GetPieces();
    BB attacks = 0;

    for (BB pieces = GetPieces(color, PieceType::Pawn); pieces;)
        attacks |= PawnAttacks(Next(pieces), color);

    for (const PieceType p : NON_PAWNS) {
        for (BB pieces = GetPieces(color, p); pieces;) {
            const Square sq = Next(pieces);
            BB attacks1 = Attacks(sq, p);

            for (BB b = allPieces & BAndB(sq, p); b != 0; b &= (b - 1))
                attacks1 &= ~XRay(sq, First(b));

            attacks |= attacks1;
        }
    }

    return attacks;
}
