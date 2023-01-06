#include "headers/move_gen.hh"

#define DEBUGGING_COUNT_MOVE_TYPE true

MoveGen::MoveGen(Color color):
color(color),
oppColor(Utilities::GetOppositeColor(color)),
up((color == Color::White) ? Direction::North : Direction::South),
doubleRank((color == Color::White) ? Row::Row2 : Row::Row7),
enPassantRank((color == Color::White) ? Row::Row6 : Row::Row3),
promotionRank((color == Color::White) ? Row::Row7 : Row::Row2),
notPromotionRank((color == Color::White) ? NotEdge::North : NotEdge::South),
kingPos((color == Color::White) ? Square::E1 : Square::E8),
castleKSide((color == Color::White) ? Square::G1 : Square::G8),
castleQSide((color == Color::White) ? Square::C1 : Square::C8) {
    if (color == Color::White) {
        castlingBlock[(int) Castling::King] = CastlingBlockSquares::KSideWhite;
        castlingBlock[(int) Castling::Queen] = CastlingBlockSquares::QSideWhite;
        castlingAttack[(int) Castling::King] = CastlingAttackSquares::KSideWhite;
        castlingAttack[(int) Castling::Queen] = CastlingAttackSquares::QSideWhite;
    } else {
        castlingBlock[(int) Castling::King] = CastlingBlockSquares::KSideBlack;
        castlingBlock[(int) Castling::Queen] = CastlingBlockSquares::QSideBlack;
        castlingAttack[(int) Castling::King] = CastlingAttackSquares::KSideBlack;
        castlingAttack[(int) Castling::Queen] = CastlingAttackSquares::QSideBlack;
    }
    GeneratePawnMoves();
}

U8 MoveGen::GetAllMoves(std::array<Move, MAXMOVECOUNT> *moves, Board *board, U64 attackedSquares) {
    int moveCount = 0;
    bool isKingSafe = IsKingSafe(board);
    moveCount += GetPawnMoves(moves, moveCount, board, isKingSafe);
    moveCount += GetRookMoves(moves, moveCount, board, isKingSafe);
    moveCount += GetBishopMoves(moves, moveCount, board, isKingSafe);
    moveCount += GetQueenMoves(moves, moveCount, board, isKingSafe);
    moveCount += GetKnightMoves(moves, moveCount, board, isKingSafe);
    moveCount += GetKingMoves(moves, moveCount, board, isKingSafe, attackedSquares);

    return moveCount;
}

U8 MoveGen::GetPawnMoves(std::array<Move, MAXMOVECOUNT> *moves, int startIndex, Board *board, bool isKingSafe) {
    U64 pieces = board->GetPiecePos(color, PieceType::Pawn);
    U8 moveCount = 0;

    while (pieces) {
        U64 lsb = Utilities::LSB_Pop(&pieces);

        // Attack moves
        //// Diagonal
        U64 captures = board->GetColorBB(oppColor) & PawnAttacks[(int) color][(int) lsb];
        while (captures) {
            U64 capturePiece = Utilities::LSB_Pop(&captures);
            if (isKingSafe || IsKingSafe(board, (board->GetOccupiedBB() ^ C64(lsb)) | C64(capturePiece), board->GetColorBB(oppColor) ^ C64(capturePiece))) {
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

        //// En Passant
        captures = board->GetEnPassant() & PawnAttacks[(int) color][(int) lsb] & (U64) enPassantRank;
        while (captures) {
            U64 capturePiece = Utilities::LSB_Pop(&captures);
            if (isKingSafe || IsKingSafe(board, (board->GetOccupiedBB() ^ C64(lsb)) | C64(capturePiece),  board->GetColorBB(oppColor) ^ C64(capturePiece)))
                AppendMove(moves, startIndex + moveCount, &moveCount, Move(MoveType::EPCapture, (Square) lsb, (Square) capturePiece, PieceType::Pawn));
        }

        // Quiet move
        //// Single push
        if (!(board->GetOccupiedBB() & pawnSingleMove[(int) lsb]))
            if (isKingSafe || IsKingSafe(board, (board->GetOccupiedBB() ^ C64(lsb)) | BitShifts::Shift(C64(lsb), up, 1))) {
                // If Promotion
                if (C64(lsb) & (U64) promotionRank) {
                    AppendMove(moves, startIndex + moveCount, &moveCount, Move(MoveType::BPromotion, (Square) lsb, (Square) (lsb + (int) up)));
                    AppendMove(moves, startIndex + moveCount, &moveCount, Move(MoveType::NPromotion, (Square) lsb, (Square) (lsb + (int) up)));
                    AppendMove(moves, startIndex + moveCount, &moveCount, Move(MoveType::QPromotion, (Square) lsb, (Square) (lsb + (int) up)));
                    AppendMove(moves, startIndex + moveCount, &moveCount, Move(MoveType::RPromotion, (Square) lsb, (Square) (lsb + (int) up)));
                } else
                    AppendMove(moves, startIndex + moveCount, &moveCount, Move(MoveType::Quiet, (Square) lsb, (Square) (lsb + (int) up)));
            }

        //// Double push
        if (C64(lsb) & (U64) doubleRank && !(board->GetOccupiedBB() & pawnDoubleMove[(int) lsb]))
            if (isKingSafe || IsKingSafe(board, (board->GetOccupiedBB() ^ C64(lsb)) | BitShifts::Shift(C64(lsb), up, 2)))
                AppendMove(moves, startIndex + moveCount, &moveCount, Move(MoveType::DoublePawnPush, (Square) lsb, (Square) (lsb + (int) up * 2)));
    }

    return moveCount;
}

U8 MoveGen::GetRookMoves(std::array<Move, MAXMOVECOUNT> *moves, int startIndex, Board *board, bool isKingSafe) {
    int moveCount = 0;

    U64 pieces = board->GetPiecePos(color, PieceType::Rook);
    for (const auto & rookDirection : rookDirections)
        moveCount += GetMoves(moves, startIndex + moveCount, board, pieces, rookDirection, isKingSafe);
    return moveCount;
}

U8 MoveGen::GetBishopMoves(std::array<Move, MAXMOVECOUNT> *moves, int startIndex, Board *board, bool isKingSafe) {
    int moveCount = 0;

    U64 pieces = board->GetPiecePos(color, PieceType::Bishop);
    for (const auto & bishopDirection : bishopDirections)
        moveCount += GetMoves(moves, startIndex + moveCount, board, pieces, bishopDirection, isKingSafe);
    return moveCount;
}

U8 MoveGen::GetQueenMoves(std::array<Move, MAXMOVECOUNT> *moves, int startIndex, Board *board, bool isKingSafe) {
    int moveCount = 0;

    U64 pieces = board->GetPiecePos(color, PieceType::Queen);
    for (const auto & queenDirection : queenDirections)
        moveCount += GetMoves(moves, startIndex + moveCount, board, pieces, queenDirection, isKingSafe);
    return moveCount;
}

U8 MoveGen::GetKnightMoves(std::array<Move, MAXMOVECOUNT> *moves, int startIndex, Board *board, bool isKingSafe) {
    U64 pieces = board->GetPiecePos(color, PieceType::Knight);
    if (pieces == 0)
        return 0;
    U8 moveCount = 0;

    while (pieces) {
            U64 lsbPiece = Utilities::LSB_Pop(&pieces);
            U64 to = KnightMoves[lsbPiece];

            // Attack moves
            U64 attackMoves = to & board->GetColorBB(oppColor);
            to &= ~board->GetOccupiedBB();

            while (attackMoves) {
                U64 lsb = Utilities::LSB_Pop(&attackMoves);
                if (isKingSafe || IsKingSafe(board, (board->GetOccupiedBB() ^ C64(lsbPiece)) | C64(lsb), board->GetColorBB(oppColor) ^ C64(lsb)))
                    AppendMove(moves, startIndex + moveCount, &moveCount, Move(MoveType::Capture, (Square) lsbPiece, (Square) lsb, board->GetType((Square) lsb)));
            }

            // Quiet moves
            U64 quietMoves = to & (~board->GetOccupiedBB());
            while (quietMoves) {
                U64 lsb = Utilities::LSB_Pop(&quietMoves);
                if (isKingSafe || IsKingSafe(board, (board->GetOccupiedBB() ^ C64(lsbPiece) | C64(lsb))))
                    AppendMove(moves, startIndex + moveCount, &moveCount, Move(MoveType::Quiet, (Square) lsbPiece, (Square) lsb));
            }
    }

    return moveCount;
}

U8 MoveGen::GetKingMoves(std::array<Move, MAXMOVECOUNT> *moves, int startIndex, Board *board, bool isKingSafe, U64 attackedSquares) {
    U64 pieces = board->GetPiecePos(color, PieceType::King);
    if (pieces == 0)
        return 0;
    U8 moveCount = 0;

    U64 lsbPiece = Utilities::LSB_Pop(&pieces);
    U64 to = KingMoves[lsbPiece];
    // Attack moves
    U64 attackMoves = to & board->GetColorBB(oppColor);
    // King cannot move onto a piece, without it being an attack
    to &= ~board->GetOccupiedBB();
    // King cannot move to an attacked square
    to &= ~attackedSquares;
    // King cannot attack an attacked square
    attackMoves &= ~attackedSquares;

    while (attackMoves) {
        U64 lsb = Utilities::LSB_Pop(&attackMoves);
        if (isKingSafe || IsKingSafe(board, board->GetOccupiedBB() ^ C64(lsbPiece), board->GetColorBB(oppColor) ^ C64(lsb), C64(lsb)))
            AppendMove(moves, startIndex + moveCount, &moveCount, Move(MoveType::Capture, (Square) lsbPiece, (Square) lsb, board->GetType((Square) lsb)));
    }

    // Quiet moves
    U64 quietMoves = to & (~board->GetOccupiedBB());
    while (quietMoves) {
        U64 lsb = Utilities::LSB_Pop(&quietMoves);
        if (isKingSafe || IsKingSafe(board, (board->GetOccupiedBB() ^ C64(lsbPiece) | C64(lsb)), board->GetColorBB(oppColor), C64(lsb)))
            AppendMove(moves, startIndex + moveCount, &moveCount, Move(MoveType::Quiet, (Square) lsbPiece, (Square) lsb));
    }

    // Castling
    if (board->IsCastlingAllowed(color, Castling::King) && !(board->GetOccupiedBB() & (U64) castlingBlock[(int) Castling::King]) && !(attackedSquares & (U64) castlingAttack[(int) Castling::King]))
        if (isKingSafe)
            AppendMove(moves, startIndex + moveCount, &moveCount, Move(MoveType::KingCastle, kingPos, castleKSide));
    if (board->IsCastlingAllowed(color, Castling::Queen) && !(board->GetOccupiedBB() & (U64) castlingBlock[(int) Castling::Queen]) && !(attackedSquares & (U64) castlingAttack[(int) Castling::Queen]))
        if (isKingSafe)
            AppendMove(moves, startIndex + moveCount, &moveCount, Move(MoveType::QueenCastle, kingPos, castleQSide));
        
    return moveCount;
}

U8 MoveGen::GetMoves(std::array<Move, MAXMOVECOUNT> *moves, int startIndex, Board *board, U64 pieces, Direction direction, bool isKingSafe) {
    U8 moveCount = 0;
    int counter = 1;

    while (pieces) {
        pieces = BitShifts::Shift(pieces & Utilities::NotEdge(direction), direction, 1) & ~board->GetColorBB(color);
        U64 attackMoves = pieces & board->GetOccupiedBB();
        pieces &= ~board->GetOccupiedBB();

        while (attackMoves) {
            U64 lsb = Utilities::LSB_Pop(&attackMoves);
            if (isKingSafe || IsKingSafe(board, (board->GetOccupiedBB() ^ BitShifts::Shift(C64(lsb), Utilities::GetOppositeDirection(direction), counter)) | C64(lsb), board->GetColorBB(oppColor) ^ C64(lsb)))
                AppendMove(moves, startIndex + moveCount, &moveCount, Move(MoveType::Capture, (Square) (lsb - (int) direction * counter), (Square) lsb, board->GetType((Square) lsb)));
        }

        U64 quietMoves = pieces;

        while (quietMoves) {
            U8 lsb = Utilities::LSB_Pop(&quietMoves);
            if (isKingSafe || IsKingSafe(board, (board->GetOccupiedBB() ^ BitShifts::Shift(C64(lsb), Utilities::GetOppositeDirection(direction), counter) | C64(lsb))))
                AppendMove(moves, startIndex + moveCount, &moveCount, Move(MoveType::Quiet, (Square) (lsb - (int) direction * counter), (Square) lsb));
        }

        counter++;
    }

    return moveCount;
}

void MoveGen::GeneratePawnMoves() {
    for (int i = 0; i < 64; i++) {
        U64 bit = C64(i);
        if (bit & (U64) notPromotionRank) {
            U64 upOne = BitShifts::Shift(bit, up, 1);
            pawnSingleMove[i] |= BitShifts::Shift(bit, up, 1);
            if (bit & (U64) doubleRank) {
                pawnDoubleMove[i] |= upOne;
                pawnDoubleMove[i] |= BitShifts::Shift(bit, up, 2);
            }
        }   
    }
}

bool MoveGen::IsKingSafe(Board *board, U64 tempOccuracyBoard, U64 tempEnemyBoard, U64 tempKingBoard) {
    U64 kingPos = tempKingBoard;
    U64 kingPosIndex = Utilities::LSB_Pop(&kingPos);

    U64 enemyRooks = (board->GetPiecePos(PieceType::Rook) | board->GetPiecePos(PieceType::Queen)) & tempEnemyBoard;
    U64 enemyBishops = (board->GetPiecePos(PieceType::Bishop) | board->GetPiecePos(PieceType::Queen)) & tempEnemyBoard;
    U64 enemyKnights = board->GetPiecePos(PieceType::Knight) & tempEnemyBoard;
    U64 enemyPawns = board->GetPiecePos(PieceType::Pawn) & tempEnemyBoard;

    if (BitShifts::GetRay(kingPosIndex, DirectionIndex::North) & enemyRooks)
        if (Utilities::LSB(BitShifts::GetRay(kingPosIndex, DirectionIndex::North) & enemyRooks) == Utilities::LSB(BitShifts::GetRay(kingPosIndex, DirectionIndex::North) & tempOccuracyBoard))
            return false;
    if (BitShifts::GetRay(kingPosIndex, DirectionIndex::East) & enemyRooks)
        if (Utilities::LSB(BitShifts::GetRay(kingPosIndex, DirectionIndex::East) & enemyRooks) == Utilities::LSB(BitShifts::GetRay(kingPosIndex, DirectionIndex::East) & tempOccuracyBoard))
            return false;
    if (BitShifts::GetRay(kingPosIndex, DirectionIndex::South) & enemyRooks)
        if (Utilities::MSB(BitShifts::GetRay(kingPosIndex, DirectionIndex::South) & enemyRooks) == Utilities::MSB(BitShifts::GetRay(kingPosIndex, DirectionIndex::South) & tempOccuracyBoard))
            return false;
    if (BitShifts::GetRay(kingPosIndex, DirectionIndex::West) & enemyRooks)
        if (Utilities::MSB(BitShifts::GetRay(kingPosIndex, DirectionIndex::West) & enemyRooks) == Utilities::MSB(BitShifts::GetRay(kingPosIndex, DirectionIndex::West) & tempOccuracyBoard))
            return false;

    if (BitShifts::GetRay(kingPosIndex, DirectionIndex::NorthEast) & enemyBishops)
        if (Utilities::LSB(BitShifts::GetRay(kingPosIndex, DirectionIndex::NorthEast) & enemyBishops) == Utilities::LSB(BitShifts::GetRay(kingPosIndex, DirectionIndex::NorthEast) & tempOccuracyBoard))
            return false;
    if (BitShifts::GetRay(kingPosIndex, DirectionIndex::NorthWest) & enemyBishops)
        if (Utilities::LSB(BitShifts::GetRay(kingPosIndex, DirectionIndex::NorthWest) & enemyBishops) == Utilities::LSB(BitShifts::GetRay(kingPosIndex, DirectionIndex::NorthWest) & tempOccuracyBoard))
            return false;
    if (BitShifts::GetRay(kingPosIndex, DirectionIndex::SouthEast) & enemyBishops)
        if (Utilities::MSB(BitShifts::GetRay(kingPosIndex, DirectionIndex::SouthEast) & enemyBishops) == Utilities::MSB(BitShifts::GetRay(kingPosIndex, DirectionIndex::SouthEast) & tempOccuracyBoard))
            return false;
    if (BitShifts::GetRay(kingPosIndex, DirectionIndex::SouthWest) & enemyBishops)
        if (Utilities::MSB(BitShifts::GetRay(kingPosIndex, DirectionIndex::SouthWest) & enemyBishops) == Utilities::MSB(BitShifts::GetRay(kingPosIndex, DirectionIndex::SouthWest) & tempOccuracyBoard))
            return false;

    if (KnightMoves[kingPosIndex] & enemyKnights)
        return false;

    if (PawnAttacks[(int) color][kingPosIndex] & enemyPawns)
        return false;

    return true;
}
