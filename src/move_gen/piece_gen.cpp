#include "headers/piece_gen.hh"
#include "../headers/bit_shifts.hh"

U8 PieceGen::GetALlMoves(std::array<Move, MAXMOVECOUNT> &moves, const std::shared_ptr<Board> &board, unsigned long long int attackedSquares,
                         bool isKingSafe, unsigned short startIndex) {
    int moveCount = 0;
    moveCount += GetAttackMoves(moves, board, attackedSquares, isKingSafe, moveCount + startIndex);
    moveCount += GetQuietMoves(moves, board, attackedSquares, isKingSafe, moveCount + startIndex);
    return moveCount;
}

U8 PieceGen::GetSlidingMoves(std::array<Move, MAXMOVECOUNT> &moves, const std::shared_ptr<Board> &board, PieceType type,
                             bool isKingSafe, U8 startIndex, U64 attackedSquares) {
    U8 moveCount = 0;
    U64 pieces = board->GetPiecePos(color, type);

    while (pieces) {
        U8 piece = Utilities::LSB_Pop(&pieces);
        U64 unblocked = (0xffffffffffffffff ^ C64(piece)) & BitShifts::GetAttacks(piece, type);
        for (U8 i = 1; i < 8; i++) {
            const U64 ring = BitShifts::GetRing(piece, i);
            U64 potMoves = ring & unblocked;
            U64 blockers = potMoves & board->GetOccupiedBB();
            potMoves ^= blockers;

            while (blockers)
                unblocked ^= BitShifts::GetSqRay(piece, Utilities::LSB_Pop(&blockers));

            while (potMoves) {
                U8 sq = Utilities::LSB_Pop(&potMoves);
                if ((isKingSafe && ((C64(piece) & attackedSquares) == 0)) || board->IsKingSafe((board->GetOccupiedBB() ^ C64(piece) | C64(sq))))
                    AppendMove(moves, startIndex + moveCount, moveCount, Move(MoveType::Quiet, (Square) piece, (Square) sq));
            }
            if (!(ring & unblocked))
                break;
        }
    }

    return moveCount;
}

U8 PieceGen::GetSlidingAttacks(std::array<Move, MAXMOVECOUNT> &moves, const std::shared_ptr<Board> &board, PieceType type,
                               bool isKingSafe, U8 startIndex, U64 attackedSquares) {
    U8 moveCount = 0;
    U64 pieces = board->GetPiecePos(color, type);
    while (pieces) {
        U8 piece = Utilities::LSB_Pop(&pieces);
        U64 unblocked = (0xffffffffffffffff ^ C64(piece)) & BitShifts::GetAttacks(piece, type);
        for (U8 i = 1; i < 8; i++) {
            const U64 ring = BitShifts::GetRing(piece, i);
            U64 blockers = ring & unblocked & board->GetOccupiedBB();
            while (blockers) {
                U8 blocker = Utilities::LSB_Pop(&blockers);
                unblocked ^= BitShifts::GetSqRay(piece, blocker);
                if (C64(blocker) & board->GetColorBB(oppColor))
                    if ((isKingSafe && ((C64(piece) & attackedSquares) == 0)) ||
                    board->IsKingSafe((board->GetOccupiedBB() ^ C64(piece)) | C64(blocker), board->GetColorBB(oppColor) ^ C64(blocker)))
                    AppendMove(moves, startIndex + moveCount, moveCount, Move(MoveType::Capture, (Square) piece, (Square) blocker, board->GetType((Square) blocker)));
            }
            if (!(ring & unblocked))
                break;
        }
    }

    return moveCount;
}

