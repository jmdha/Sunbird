#include "king_gen.hh"

U8 KingGen::GetQuietMoves(std::array<Move, MAXMOVECOUNT> &moves, const std::shared_ptr<Board> &board, unsigned long long int attackedSquares,
                            bool isKingSafe, unsigned short startIndex) {
    U8 moveCount = 0;
    U64 pieces = board->GetPiecePos(color, PieceType::King);

    U64 lsbPiece = Utilities::LSB_Pop(&pieces);

    // Quiet moves
    U64 quietMoves = KingMoves[lsbPiece] & ~board->GetOccupiedBB() & ~attackedSquares;
    while (quietMoves) {
        U64 lsb = Utilities::LSB_Pop(&quietMoves);
        if (isKingSafe || board->IsKingSafe((board->GetOccupiedBB() ^ C64(lsbPiece) | C64(lsb)), board->GetColorBB(oppColor), C64(lsb)))
            AppendMove(moves, startIndex + moveCount, moveCount, Move(MoveType::Quiet, (Square) lsbPiece, (Square) lsb));
    }

    // Castling
    if (board->IsCastlingAllowed(color, Castling::King) && !(board->GetOccupiedBB() & (U64) castlingBlock[(int) Castling::King]) && !(attackedSquares & (U64) castlingAttack[(int) Castling::King]))
        if (isKingSafe)
            AppendMove(moves, startIndex + moveCount, moveCount, Move(MoveType::KingCastle, kingPos, castleKSide));
    if (board->IsCastlingAllowed(color, Castling::Queen) && !(board->GetOccupiedBB() & (U64) castlingBlock[(int) Castling::Queen]) && !(attackedSquares & (U64) castlingAttack[(int) Castling::Queen]))
        if (isKingSafe)
            AppendMove(moves, startIndex + moveCount, moveCount, Move(MoveType::QueenCastle, kingPos, castleQSide));

    return moveCount;
}

U8 KingGen::GetAttackMoves(std::array<Move, MAXMOVECOUNT> &moves, const std::shared_ptr<Board> &board, unsigned long long int attackedSquares,
                             bool isKingSafe, unsigned short startIndex) {
    U8 moveCount = 0;
    U64 pieces = board->GetPiecePos(color, PieceType::King);

    U64 lsbPiece = Utilities::LSB_Pop(&pieces);

    // Attack moves
    U64 attackMoves = KingMoves[lsbPiece] & board->GetColorBB(oppColor) & ~attackedSquares;
    while (attackMoves) {
        U64 lsb = Utilities::LSB_Pop(&attackMoves);
        if (isKingSafe || board->IsKingSafe(board->GetOccupiedBB() ^ C64(lsbPiece), board->GetColorBB(oppColor) ^ C64(lsb), C64(lsb)))
            AppendMove(moves, startIndex + moveCount, moveCount, Move(MoveType::Capture, (Square) lsbPiece, (Square) lsb, board->GetType((Square) lsb)));
    }

    return moveCount;
}
