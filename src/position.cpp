#include "position.hpp"
#include "types.hpp"

bool Position::IsKingSafe(Color turn) const {
    Square king = GetKing(turn);

    BB occ     = GetPieces();
    BB pawns   = GetPieces(~turn, PAWN);
    BB knights = GetPieces(~turn, KNIGHT);
    BB bishops = (GetPieces(~turn, BISHOP) | GetPieces(~turn, QUEEN));
    BB rooks   = (GetPieces(~turn, ROOK) | GetPieces(~turn, QUEEN));

    // Checking jumpers
    if (PawnAttacks(king, turn) & pawns) return false;
    if (Attacks(king, KNIGHT) & knights) return false;

    if (Ray(king, NORTH) & rooks) [[unlikely]]
        if (lsb(Ray(king, NORTH) & rooks) == lsb(Ray(king, NORTH) & occ)) [[unlikely]]
            return false;
    if (Ray(king, EAST) & rooks) [[unlikely]]
        if (lsb(Ray(king, EAST) & rooks) == lsb(Ray(king, EAST) & occ)) [[unlikely]]
            return false;
    if (Ray(king, SOUTH) & rooks) [[unlikely]]
        if (msb(Ray(king, SOUTH) & rooks) == msb(Ray(king, SOUTH) & occ)) [[unlikely]]
            return false;
    if (Ray(king, WEST) & rooks) [[unlikely]]
        if (msb(Ray(king, WEST) & rooks) == msb(Ray(king, WEST) & occ)) [[unlikely]]
            return false;

    if (Ray(king, NORTH_EAST) & bishops) [[unlikely]]
        if (lsb(Ray(king, NORTH_EAST) & bishops) == lsb(Ray(king, NORTH_EAST) & occ)) [[unlikely]]
            return false;
    if (Ray(king, NORTH_WEST) & bishops) [[unlikely]]
        if (lsb(Ray(king, NORTH_WEST) & bishops) == lsb(Ray(king, NORTH_WEST) & occ)) [[unlikely]]
            return false;
    if (Ray(king, SOUTH_EAST) & bishops) [[unlikely]]
        if (msb(Ray(king, SOUTH_EAST) & bishops) == msb(Ray(king, SOUTH_EAST) & occ)) [[unlikely]]
            return false;
    if (Ray(king, SOUTH_WEST) & bishops) [[unlikely]]
        if (msb(Ray(king, SOUTH_WEST) & bishops) == msb(Ray(king, SOUTH_WEST) & occ)) [[unlikely]]
            return false;

    return true;
}

BB Position::GenerateAttackSquares(Color color) const {
    const BB allPieces = GetPieces();
    BB attacks         = 0;

    for (BB pieces = GetPieces(color, PAWN); pieces;)
        attacks |= PawnAttacks(lsb_pop(pieces), color);

    for (const Piece p : NON_PAWNS) {
        for (BB pieces = GetPieces(color, p); pieces;) {
            const Square sq = lsb_pop(pieces);
            BB attacks1     = Attacks(sq, p);

            for (BB b = allPieces & BAndB(sq, p); b != 0; b &= (b - 1))
                attacks1 &= ~XRay(sq, lsb(b));

            attacks |= attacks1;
        }
    }

    return attacks;
}
