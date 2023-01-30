#include "headers/piece_gen.hh"
#include "../headers/bit_shifts.hh"

U8 PieceGen::GetALlMoves(std::array<Move, 128> *moves, Board *board, unsigned long long int attackedSquares,
                         bool isKingSafe, unsigned short startIndex) {
    int moveCount = 0;
    moveCount += GetAttackMoves(moves, board, attackedSquares, isKingSafe, moveCount + startIndex);
    moveCount += GetQuietMoves(moves, board, attackedSquares, isKingSafe, moveCount + startIndex);
    return moveCount;
}

U8 PieceGen::GetSlidingMoves(std::array<Move, MAXMOVECOUNT> *moves, Board *board, U64 pieces, Direction direction,
                             bool isKingSafe, U8 startIndex, U64 attackedSquares) {
    U8 moveCount = 0;
    int counter = 1;

    while (pieces) {
        pieces = BitShifts::Shift(pieces & Utilities::NotEdge(direction), direction, 1) & ~board->GetColorBB(color);
        pieces &= ~board->GetOccupiedBB();

        U64 quietMoves = pieces;
        while (quietMoves) {
            U8 lsb = Utilities::LSB_Pop(&quietMoves);
            U64 oriPiece = BitShifts::Shift(C64(lsb), Utilities::GetOppositeDirection(direction), counter);
            if ((isKingSafe && ((oriPiece & attackedSquares) == 0)) || board->IsKingSafe((board->GetOccupiedBB() ^ oriPiece | C64(lsb))))
                AppendMove(moves, startIndex + moveCount, &moveCount, Move(MoveType::Quiet, (Square) (lsb - (int) direction * counter), (Square) lsb));
        }

        counter++;
    }

    return moveCount;
}

U8 PieceGen::GetSlidingAttacks(std::array<Move, MAXMOVECOUNT> *moves, Board *board, PieceType type,
                               bool isKingSafe, U8 startIndex, U64 attackedSquares) {
    U8 moveCount = 0;
    U64 pieces = board->GetPiecePos(color, type);

    while (pieces) {
        U8 piece = Utilities::LSB_Pop(&pieces);
        U64 unblocked = (0xffffffffffffffff ^ C64(piece)) & BitShifts::GetAttacks(piece, type);
        for (U8 i = 1; i < 8 && unblocked & BitShifts::GetAttacks(piece, type); i++) {
            U64 blockers = BitShifts::GetRing(piece, i) & unblocked & board->GetOccupiedBB();

            while (blockers) {
                U8 blocker = Utilities::LSB_Pop(&blockers);
                unblocked ^= BitShifts::GetSqRay(piece, blocker);
                if (C64(blocker) & board->GetColorBB(oppColor))
                    if ((isKingSafe && ((C64(piece) & attackedSquares) == 0)) ||
                    board->IsKingSafe((board->GetOccupiedBB() ^ C64(piece)) | C64(blocker), board->GetColorBB(oppColor) ^ C64(blocker)))
                        AppendMove(moves, startIndex + moveCount, &moveCount, Move(MoveType::Capture, (Square) piece, (Square) blocker, board->GetType((Square) blocker)));
            }
        }
    }

    return moveCount;
}

