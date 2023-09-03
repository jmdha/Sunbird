#include <chess/internal/position.hpp>

using namespace Chess;

bool Position::IsKingSafe(BB tempOccuracyBoard, BB tempEnemyBoard, BB tempKingBoard) const {
    if (tempKingBoard == 0)
        return true;
    const Square kingPosIndex = First(tempKingBoard);

    BB enemyRooks = (GetPieces(PieceType::Rook) | GetPieces(PieceType::Queen)) & tempEnemyBoard;
    BB enemyBishops = (GetPieces(PieceType::Bishop) | GetPieces(PieceType::Queen)) & tempEnemyBoard;
    BB enemyKnights = GetPieces(PieceType::Knight) & tempEnemyBoard;
    BB enemyPawns = GetPieces(PieceType::Pawn) & tempEnemyBoard;

    // clang-format off
    if (Ray(kingPosIndex, Direction::North) & enemyRooks) [[unlikely]]
        if (First(Ray(kingPosIndex, Direction::North) & enemyRooks) == First(Ray(kingPosIndex, Direction::North) & tempOccuracyBoard))
            return false;
    if (Ray(kingPosIndex, Direction::East) & enemyRooks) [[unlikely]]
        if (First(Ray(kingPosIndex, Direction::East) & enemyRooks) == First(Ray(kingPosIndex, Direction::East) & tempOccuracyBoard))
            return false;
    if (Ray(kingPosIndex, Direction::South) & enemyRooks) [[unlikely]]
        if (Last(Ray(kingPosIndex, Direction::South) & enemyRooks) == Last(Ray(kingPosIndex, Direction::South) & tempOccuracyBoard))
            return false;
    if (Ray(kingPosIndex, Direction::West) & enemyRooks) [[unlikely]]
        if (Last(Ray(kingPosIndex, Direction::West) & enemyRooks) == Last(Ray(kingPosIndex, Direction::West) & tempOccuracyBoard))
            return false;

    if (Ray(kingPosIndex, Direction::NorthEast) & enemyBishops) [[unlikely]]
        if (First(Ray(kingPosIndex, Direction::NorthEast) & enemyBishops) == First(Ray(kingPosIndex, Direction::NorthEast) & tempOccuracyBoard))
            return false;
    if (Ray(kingPosIndex, Direction::NorthWest) & enemyBishops) [[unlikely]]
        if (First(Ray(kingPosIndex, Direction::NorthWest) & enemyBishops) == First(Ray(kingPosIndex, Direction::NorthWest) & tempOccuracyBoard))
            return false;
    if (Ray(kingPosIndex, Direction::SouthEast) & enemyBishops) [[unlikely]]
        if (Last(Ray(kingPosIndex, Direction::SouthEast) & enemyBishops) == Last(Ray(kingPosIndex, Direction::SouthEast) & tempOccuracyBoard))
            return false;
    if (Ray(kingPosIndex, Direction::SouthWest) & enemyBishops) [[unlikely]]
        if (Last(Ray(kingPosIndex, Direction::SouthWest) & enemyBishops) == Last(Ray(kingPosIndex, Direction::SouthWest) & tempOccuracyBoard))
            return false;
    // clang-format on

    if (Attacks(kingPosIndex, PieceType::Knight) & enemyKnights) [[unlikely]]
        return false;

    if (PawnAttacks(kingPosIndex, GetTurn()) & enemyPawns) [[unlikely]]
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
