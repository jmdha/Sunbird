#include "headers/pawn_gen.hh"
#include "../headers/bit_shifts.hh"

U8 PawnGen::GetQuietMoves(std::array<Move, 128> *moves, Board *board, unsigned long long int attackedSquares,
                          bool isKingSafe, unsigned short startIndex) {
    U64 pieces = board->GetPiecePos(color, PieceType::Pawn);
    U8 moveCount = 0;

    while (pieces) {
        U64 lsb = Utilities::LSB_Pop(&pieces);

        // Single push
        if (!(board->GetOccupiedBB() & singleMove[(int) lsb]))
            if (isKingSafe || board->IsKingSafe((board->GetOccupiedBB() ^ C64(lsb)) | BitShifts::Shift(C64(lsb), up, 1))) {
                // If Promotion
                if (C64(lsb) & (U64) promotionRank) {
                    AppendMove(moves, startIndex + moveCount, &moveCount, Move(MoveType::BPromotion, (Square) lsb, (Square) (lsb + (int) up)));
                    AppendMove(moves, startIndex + moveCount, &moveCount, Move(MoveType::NPromotion, (Square) lsb, (Square) (lsb + (int) up)));
                    AppendMove(moves, startIndex + moveCount, &moveCount, Move(MoveType::QPromotion, (Square) lsb, (Square) (lsb + (int) up)));
                    AppendMove(moves, startIndex + moveCount, &moveCount, Move(MoveType::RPromotion, (Square) lsb, (Square) (lsb + (int) up)));
                } else
                    AppendMove(moves, startIndex + moveCount, &moveCount, Move(MoveType::Quiet, (Square) lsb, (Square) (lsb + (int) up)));
            }

        // Double push
        if (C64(lsb) & (U64) doubleRank && !(board->GetOccupiedBB() & doubleMove[(int) lsb]))
            if (isKingSafe || board->IsKingSafe((board->GetOccupiedBB() ^ C64(lsb)) | BitShifts::Shift(C64(lsb), up, 2)))
                AppendMove(moves, startIndex + moveCount, &moveCount, Move(MoveType::DoublePawnPush, (Square) lsb, (Square) (lsb + (int) up * 2)));
    }

    return moveCount;
}

U8 PawnGen::GetAttackMoves(std::array<Move, 128> *moves, Board *board, unsigned long long int attackedSquares,
                           bool isKingSafe, unsigned short startIndex) {
    U64 pieces = board->GetPiecePos(color, PieceType::Pawn);
    U8 moveCount = 0;

    while (pieces) {
        U64 lsb = Utilities::LSB_Pop(&pieces);

        // Diagonal
        U64 captures = board->GetColorBB(oppColor) & PawnAttacks[(int) color][(int) lsb];
        while (captures) {
            U64 capturePiece = Utilities::LSB_Pop(&captures);
            if (isKingSafe || board->IsKingSafe((board->GetOccupiedBB() ^ C64(lsb)) | C64(capturePiece), board->GetColorBB(oppColor) ^ C64(capturePiece))) {
                // If Promotion
                if (C64(lsb) & (U64) promotionRank) {
                    AppendMove(moves, startIndex + moveCount, &moveCount,
                               Move(MoveType::QPromotionCapture, (Square) lsb, (Square) capturePiece,
                                    board->GetType((Square) capturePiece)));
                    AppendMove(moves, startIndex + moveCount, &moveCount,
                               Move(MoveType::RPromotionCapture, (Square) lsb, (Square) capturePiece,
                                    board->GetType((Square) capturePiece)));
                    AppendMove(moves, startIndex + moveCount, &moveCount,
                               Move(MoveType::BPromotionCapture, (Square) lsb, (Square) capturePiece,
                                    board->GetType((Square) capturePiece)));
                    AppendMove(moves, startIndex + moveCount, &moveCount,
                               Move(MoveType::NPromotionCapture, (Square) lsb, (Square) capturePiece,
                                    board->GetType((Square) capturePiece)));
                } else {
                    AppendMove(moves, startIndex + moveCount, &moveCount,
                               Move(MoveType::Capture, (Square) lsb, (Square) capturePiece,
                                    board->GetType((Square) capturePiece)));
                }
            }
        }

        // En Passant
        captures = board->GetEnPassant() & PawnAttacks[(int) color][(int) lsb] & (U64) enPassantRank;
        while (captures) {
            U64 capturePiece = Utilities::LSB_Pop(&captures);
            if (isKingSafe || board->IsKingSafe((board->GetOccupiedBB() ^ C64(lsb)) | C64(capturePiece),  board->GetColorBB(oppColor) ^ C64(capturePiece)))
                AppendMove(moves, startIndex + moveCount, &moveCount, Move(MoveType::EPCapture, (Square) lsb, (Square) capturePiece, PieceType::Pawn));
        }
    }

    return moveCount;
}

void PawnGen::GeneratePawnMoves() {
    for (int i = 0; i < 64; ++i) {
        U64 bit = C64(i);
        if (bit & (U64) notPromotionRank) {
            U64 upOne = BitShifts::Shift(bit, up, 1);
            singleMove[i] |= BitShifts::Shift(bit, up, 1);
            if (bit & (U64) doubleRank) {
                int pawnDoubleMove = 0;
                doubleMove[i] |= upOne;
                doubleMove[i] |= BitShifts::Shift(bit, up, 2);
            }
        }
    }
}
