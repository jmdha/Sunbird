#include "knight_gen.hh"

U8 KnightGen::GetQuietMoves(std::array<Move, MAXMOVECOUNT> &moves, const std::shared_ptr<Board> &board, unsigned long long int attackedSquares,
                          bool isKingSafe, unsigned short startIndex) {
    U8 moveCount = 0;
    U64 pieces = board->GetPiecePos(color, PieceType::Knight);

    while (pieces) {
        U64 lsbPiece = Utilities::LSB_Pop(&pieces);

        U64 quietMoves = KnightMoves[lsbPiece] & (~board->GetOccupiedBB());
        while (quietMoves) {
            U64 lsb = Utilities::LSB_Pop(&quietMoves);
            if ((isKingSafe && ((C64(lsbPiece) & attackedSquares) == 0)) || board->IsKingSafe((board->GetOccupiedBB() ^ C64(lsbPiece)) | C64(lsb), board->GetColorBB(oppColor) ^ C64(lsb)))
                AppendMove(moves, startIndex + moveCount, moveCount, Move(MoveType::Quiet, (Square) lsbPiece, (Square) lsb));
        }
    }

    return moveCount;
}

U8 KnightGen::GetAttackMoves(std::array<Move, MAXMOVECOUNT> &moves, const std::shared_ptr<Board> &board, unsigned long long int attackedSquares,
                           bool isKingSafe, unsigned short startIndex) {
    U8 moveCount = 0;
    U64 pieces = board->GetPiecePos(color, PieceType::Knight);

    while (pieces) {
        U64 lsbPiece = Utilities::LSB_Pop(&pieces);

        U64 attackMoves = KnightMoves[lsbPiece] & board->GetColorBB(oppColor);
        while (attackMoves) {
            U64 lsb = Utilities::LSB_Pop(&attackMoves);
            if ((isKingSafe && ((C64(lsbPiece) & attackedSquares) == 0)) || board->IsKingSafe((board->GetOccupiedBB() ^ C64(lsbPiece)) | C64(lsb), board->GetColorBB(oppColor) ^ C64(lsb)))
                AppendMove(moves, startIndex + moveCount, moveCount, Move(MoveType::Capture, (Square) lsbPiece, (Square) lsb, board->GetType((Square) lsb)));
        }
    }

    return moveCount;
}
