#include "headers/piece_gen.hh"
#include "../headers/bit_shifts.hh"

U8 PieceGen::GetALlMoves(std::array<Move, 128> *moves, Board *board, unsigned long long int attackedSquares,
                         bool isKingSafe, unsigned short startIndex) {
    int moveCount = 0;
    moveCount += GetAttackMoves(moves, board, attackedSquares, isKingSafe, moveCount + startIndex);
    moveCount += GetQuietMoves(moves, board, attackedSquares, isKingSafe, moveCount + startIndex);
    return moveCount;
}

U8 PieceGen::GetSlidingMoves(std::array<Move, 128> *moves, Board *board, unsigned long long int pieces,
                             Direction direction, bool isKingSafe, unsigned short startIndex) {
    U8 moveCount = 0;
    int counter = 1;

    while (pieces) {
        pieces = BitShifts::Shift(pieces & Utilities::NotEdge(direction), direction, 1) & ~board->GetColorBB(color);
        pieces &= ~board->GetOccupiedBB();

        U64 quietMoves = pieces;
        while (quietMoves) {
            U8 lsb = Utilities::LSB_Pop(&quietMoves);
            if (isKingSafe || board->IsKingSafe((board->GetOccupiedBB() ^ BitShifts::Shift(C64(lsb), Utilities::GetOppositeDirection(direction), counter) | C64(lsb))))
                AppendMove(moves, startIndex + moveCount, &moveCount, Move(MoveType::Quiet, (Square) (lsb - (int) direction * counter), (Square) lsb));
        }

        counter++;
    }

    return moveCount;
}

U8 PieceGen::GetSlidingAttacks(std::array<Move, 128> *moves, Board *board, unsigned long long int pieces,
                               Direction direction, bool isKingSafe, unsigned short startIndex) {
    U8 moveCount = 0;
    int counter = 1;

    while (pieces) {
        pieces = BitShifts::Shift(pieces & Utilities::NotEdge(direction), direction, 1) & ~board->GetColorBB(color);
        U64 attackMoves = pieces & board->GetOccupiedBB();
        pieces &= ~board->GetOccupiedBB();

        while (attackMoves) {
            U64 lsb = Utilities::LSB_Pop(&attackMoves);
            if (isKingSafe || board->IsKingSafe((board->GetOccupiedBB() ^ BitShifts::Shift(C64(lsb), Utilities::GetOppositeDirection(direction), counter)) | C64(lsb), board->GetColorBB(oppColor) ^ C64(lsb)))
                AppendMove(moves, startIndex + moveCount, &moveCount, Move(MoveType::Capture, (Square) (lsb - (int) direction * counter), (Square) lsb, board->GetType((Square) lsb)));
        }

        counter++;
    }

    return moveCount;
}

