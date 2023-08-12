#include <chess/internal/position.hpp>

using namespace Chess;

bool Position::IsKingSafe(BB tempOccuracyBoard, BB tempEnemyBoard,
                          BB tempKingBoard) const noexcept {
    if (tempKingBoard == 0)
        return true;
    BB kingPosIndex = jank::bit::lsb_pop(tempKingBoard);

    BB enemyRooks = (GetPieces(PieceType::Rook) | GetPieces(PieceType::Queen)) & tempEnemyBoard;
    BB enemyBishops = (GetPieces(PieceType::Bishop) | GetPieces(PieceType::Queen)) & tempEnemyBoard;
    BB enemyKnights = GetPieces(PieceType::Knight) & tempEnemyBoard;
    BB enemyPawns = GetPieces(PieceType::Pawn) & tempEnemyBoard;

    // clang-format off
    if (Ray((Square)kingPosIndex, Direction::North) & enemyRooks) [[unlikely]]
        if (jank::bit::lsb(Ray((Square)kingPosIndex, Direction::North) & enemyRooks) == jank::bit::lsb(Ray((Square)kingPosIndex, Direction::North) & tempOccuracyBoard))
            return false;
    if (Ray((Square)kingPosIndex, Direction::East) & enemyRooks) [[unlikely]]
        if (jank::bit::lsb(Ray((Square)kingPosIndex, Direction::East) & enemyRooks) == jank::bit::lsb(Ray((Square)kingPosIndex, Direction::East) & tempOccuracyBoard))
            return false;
    if (Ray((Square)kingPosIndex, Direction::South) & enemyRooks) [[unlikely]]
        if (jank::bit::msb(Ray((Square)kingPosIndex, Direction::South) & enemyRooks) == jank::bit::msb(Ray((Square)kingPosIndex, Direction::South) & tempOccuracyBoard))
            return false;
    if (Ray((Square)kingPosIndex, Direction::West) & enemyRooks) [[unlikely]]
        if (jank::bit::msb(Ray((Square)kingPosIndex, Direction::West) & enemyRooks) == jank::bit::msb(Ray((Square)kingPosIndex, Direction::West) & tempOccuracyBoard))
            return false;

    if (Ray((Square)kingPosIndex, Direction::NorthEast) & enemyBishops) [[unlikely]]
        if (jank::bit::lsb(Ray((Square)kingPosIndex, Direction::NorthEast) & enemyBishops) == jank::bit::lsb(Ray((Square)kingPosIndex, Direction::NorthEast) & tempOccuracyBoard))
            return false;
    if (Ray((Square)kingPosIndex, Direction::NorthWest) & enemyBishops) [[unlikely]]
        if (jank::bit::lsb(Ray((Square)kingPosIndex, Direction::NorthWest) & enemyBishops) == jank::bit::lsb(Ray((Square)kingPosIndex, Direction::NorthWest) & tempOccuracyBoard))
            return false;
    if (Ray((Square)kingPosIndex, Direction::SouthEast) & enemyBishops) [[unlikely]]
        if (jank::bit::msb(Ray((Square)kingPosIndex, Direction::SouthEast) & enemyBishops) == jank::bit::msb(Ray((Square)kingPosIndex, Direction::SouthEast) & tempOccuracyBoard))
            return false;
    if (Ray((Square)kingPosIndex, Direction::SouthWest) & enemyBishops) [[unlikely]]
        if (jank::bit::msb(Ray((Square)kingPosIndex, Direction::SouthWest) & enemyBishops) == jank::bit::msb(Ray((Square)kingPosIndex, Direction::SouthWest) & tempOccuracyBoard))
            return false;
    // clang-format on

    if (Attacks((Square)kingPosIndex, PieceType::Knight) & enemyKnights) [[unlikely]]
        return false;

    if (PawnAttacks((Square)kingPosIndex, GetTurn()) & enemyPawns) [[unlikely]]
        return false;

    return true;
}

BB Position::GenerateAttackSquares(Color color) const noexcept {
    BB attacks = 0;

    BB tempPieces[PIECECOUNT];
    for (int pIndex = 0; pIndex < PIECECOUNT; pIndex++)
        tempPieces[pIndex] = GetPieces(color, (PieceType)pIndex);

    while (tempPieces[(int)PieceType::Pawn])
        attacks |= PawnAttacks((Square)jank::bit::lsb_pop(tempPieces[(int)PieceType::Pawn]), color);

    for (int pIndex = 1; pIndex < PIECECOUNT; ++pIndex)
        while (tempPieces[pIndex]) {
            unsigned short pos = jank::bit::lsb_pop(tempPieces[pIndex]);
            BB attacks1 = Attacks((Square)pos, (PieceType)pIndex);

            for (BB b = GetPieces() & BAndB((Square)pos, (PieceType)pIndex); b != 0; b &= (b - 1))
                attacks1 &= ~XRay((Square)pos, (Square)jank::bit::lsb(b));

            attacks |= attacks1;
        }

    return attacks;
}
