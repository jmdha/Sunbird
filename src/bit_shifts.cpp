#include "bit_shifts.hh"

void BitShifts::Init () {
    for (U64 square = 0; square < SQUARECOUNT; square++)
        for (int dir = 0; dir < DIRECTIONCOUNT; dir++)
            rays[square][dir] = GenerateRay(square, (DirectionIndex) dir);
    for (U8 from = 0; from < SQUARECOUNT; from++)
        for (U8 to = 0; to < SQUARECOUNT; to++)
            if (from != to)
                behind[from][to] = GenerateBehind(from, to);
    for (U8 square = 0; square < SQUARECOUNT; square++) {
        pieceAttacks[(int) PieceType::Knight][square] = KnightMoves[square];
        pieceAttacks[(int) PieceType::King][square] = KingMoves[square];
        pieceAttacks[(int) PieceType::Rook][square] = GetRay(square, DirectionIndex::North) |
                                                      GetRay(square, DirectionIndex::East) |
                                                      GetRay(square, DirectionIndex::South) |
                                                      GetRay(square, DirectionIndex::West);
        pieceAttacks[(int) PieceType::Bishop][square] = GetRay(square, DirectionIndex::NorthWest) |
                                                        GetRay(square, DirectionIndex::NorthEast) |
                                                        GetRay(square, DirectionIndex::SouthEast) |
                                                        GetRay(square, DirectionIndex::SouthWest);
        pieceAttacks[(int) PieceType::Queen][square] = pieceAttacks[(int) PieceType::Rook][square] | pieceAttacks[(int) PieceType::Bishop][square];
        blockersAndBeyond[(int) PieceType::Rook][square] = GenerateBB(square, PieceType::Rook);
        blockersAndBeyond[(int) PieceType::Bishop][square] = GenerateBB(square, PieceType::Bishop);
        blockersAndBeyond[(int) PieceType::Queen][square] = GenerateBB(square, PieceType::Queen);
    }
    for (U8 square = 0; square < SQUARECOUNT; square++)
        for (U8 offset = 1; offset < 7; offset++)
            rings[square][offset] = GenerateRing(square, offset);
    for (U8 from = 0; from < SQUARECOUNT; from++)
        for (U8 to = 0; to < SQUARECOUNT; to++)
            if (from != to)
                sqRays[from][to] = GenerateSqRay(from, to);
}
