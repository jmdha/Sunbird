#include "headers/knight_gen.hh"

U8 KnightGen::GetQuietMoves(std::array<Move, 128> *moves, Board *board, unsigned long long int attackedSquares,
                          bool isKingSafe, unsigned short startIndex) {
    U64 pieces = board->GetPiecePos(color, PieceType::Knight);
    if (pieces == 0)
        return 0;
    U8 moveCount = 0;

    while (pieces) {
        U64 lsbPiece = Utilities::LSB_Pop(&pieces);

        U64 quietMoves = KnightMoves[lsbPiece] & (~board->GetOccupiedBB());
        while (quietMoves) {
            U64 lsb = Utilities::LSB_Pop(&quietMoves);
            if (isKingSafe || board->IsKingSafe((board->GetOccupiedBB() ^ C64(lsbPiece)) | C64(lsb), board->GetColorBB(oppColor) ^ C64(lsb)))
                AppendMove(moves, startIndex + moveCount, &moveCount, Move(MoveType::Quiet, (Square) lsbPiece, (Square) lsb));
        }
    }

    return moveCount;
}

U8 KnightGen::GetAttackMoves(std::array<Move, 128> *moves, Board *board, unsigned long long int attackedSquares,
                           bool isKingSafe, unsigned short startIndex) {
    U64 pieces = board->GetPiecePos(color, PieceType::Knight);
    if (pieces == 0)
        return 0;
    U8 moveCount = 0;

    while (pieces) {
        U64 lsbPiece = Utilities::LSB_Pop(&pieces);

        U64 attackMoves = KnightMoves[lsbPiece] & board->GetColorBB(oppColor);
        while (attackMoves) {
            U64 lsb = Utilities::LSB_Pop(&attackMoves);
            if (isKingSafe || board->IsKingSafe((board->GetOccupiedBB() ^ C64(lsbPiece)) | C64(lsb), board->GetColorBB(oppColor) ^ C64(lsb)))
                AppendMove(moves, startIndex + moveCount, &moveCount, Move(MoveType::Capture, (Square) lsbPiece, (Square) lsb, board->GetType((Square) lsb)));
        }
    }

    return moveCount;
}