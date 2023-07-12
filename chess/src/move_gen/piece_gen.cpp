#include "chess/internal/piece_gen.hpp"
#include "chess/internal/bit_shifts.hpp"

U8 PieceGen::GetALlMoves(std::array<Move, MAXMOVECOUNT> &moves, const std::shared_ptr<Board> &board, unsigned short startIndex) {
    int moveCount = 0;
    moveCount += GetAttackMoves(moves, board, moveCount + startIndex);
    moveCount += GetQuietMoves(moves, board, moveCount + startIndex);
    return moveCount;
}

U8 PieceGen::GetQuietMovesGeneric(std::array<Move, MAXMOVECOUNT> &moves, const std::shared_ptr<Board> &board, PieceType type, U8 startIndex) {
    U8 moveCount = 0;
    U64 pieces = board->GetPiecePos(color, type);

    while (pieces) {
        const U8 piece = Utilities::LSB_Pop(&pieces);
        U64 unblocked = (0xffffffffffffffff) & BitShifts::GetAttacks(piece, type);
        for (U8 i = 1; i < 8; ++i) {
            const U64 ring = BitShifts::GetRing(piece, i);
            U64 potMoves = ring & unblocked;
            U64 blockers = potMoves & board->GetOccupiedBB();
            potMoves ^= blockers;

            while (blockers)
                unblocked = unblocked & ~BitShifts::GetSqRay(piece, Utilities::LSB_Pop(&blockers));

            while (potMoves) {
                const U8 sq = Utilities::LSB_Pop(&potMoves);
                if (board->IsKingSafe((board->GetOccupiedBB() ^ C64(piece) | C64(sq))))
                    AppendMove(moves, startIndex + moveCount, moveCount, Move(MoveType::Quiet, (Square) piece, (Square) sq));
            }
        }
    }

    return moveCount;
}

U8 PieceGen::GetAttackMovesGeneric(std::array<Move, MAXMOVECOUNT> &moves, const std::shared_ptr<Board> &board, PieceType type, U8 startIndex) {
    U8 moveCount = 0;
    U64 pieces = board->GetPiecePos(color, type);
    while (pieces) {
        const U8 piece = Utilities::LSB_Pop(&pieces);
        U64 unblocked = (0xffffffffffffffff) & BitShifts::GetAttacks(piece, type);
        for (U8 i = 1; i < 8; ++i) {
            const U64 ring = BitShifts::GetRing(piece, i);
            U64 blockers = ring & unblocked & board->GetOccupiedBB();
            U64 tempBlockers = blockers;
            while (tempBlockers)
                unblocked = unblocked & ~BitShifts::GetSqRay(piece, Utilities::LSB_Pop(&tempBlockers));
            blockers &= board->GetColorBB(oppColor);
            while (blockers) {
                const U64 blocker = Utilities::LSB_Pop(&blockers);
                if (board->IsKingSafe((board->GetOccupiedBB() ^ C64(piece)) | C64(blocker), board->GetColorBB(oppColor) ^ C64(blocker)))
                    AppendMove(moves, startIndex + moveCount, moveCount, Move(MoveType::Capture, (Square) piece, (Square) blocker, board->GetType((Square) blocker)));
            }
        }
    }

    return moveCount;
}

