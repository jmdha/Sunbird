#include "headers/move_gen.hh"

#define DEBUGGING_COUNT_MOVE_TYPE true

MoveGen::MoveGen(Color color):
color(color),
oppColor(Utilities::GetOppositeColor(color)),
up((color == Color::White) ? Direction::North : Direction::South),
upRight((up == Direction::North) ? Direction::NorthEast : Direction::SouthEast),
upLeft((up == Direction::North) ? Direction::NorthWest : Direction::SouthWest),
doubleRank((up == Direction::North) ? Row::Row2 : Row::Row7),
enPassantRank((up == Direction::North) ? Row::Row6 : Row::Row3),
notPromotionRank((up == Direction::North) ? NotEdge::North : NotEdge::South) {
    if (up == Direction::North) {
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
    moveCount += GetPawnMoves(moves, moveCount, board, isKingSafe, attackedSquares);
    moveCount += GetRookMoves(moves, moveCount, board, isKingSafe, attackedSquares);
    moveCount += GetBishopMoves(moves, moveCount, board, isKingSafe, attackedSquares);
    moveCount += GetQueenMoves(moves, moveCount, board, isKingSafe, attackedSquares);
    moveCount += GetKnightMoves(moves, moveCount, board, isKingSafe, attackedSquares);
    moveCount += GetKingMoves(moves, moveCount, board, isKingSafe, attackedSquares);

    return moveCount;
}

U8 MoveGen::GetPawnMoves(std::array<Move, MAXMOVECOUNT> *moves, int startIndex, Board *board, bool isKingSafe, U64 attackedSquares) {
    // Generate single and double step moves
    U64 pieces = board->pieceBB[(int) PieceType::Pawn] & board->colorBB[(int) color];
    if (pieces == 0)
        return 0;
        
    U8 moveCount = 0;

    // Generate attackedSquares
    U64 tempPieces = pieces;

    while (pieces) {
        int lsb = Utilities::LSB_Pop(&pieces);

        // Attack moves
        //// Diagonal
        U64 captures = board->colorBB[(int) oppColor] & PawnAttacks[(int) color][(int) lsb];
        while (captures) {
            int capturePiece = Utilities::LSB_Pop(&captures);
            if (isKingSafe || IsKingSafe(board, board->occupiedBB ^ C64(lsb), board->colorBB[(int) oppColor] ^ C64(capturePiece)))
                AppendMove(moves, startIndex + moveCount, &moveCount, Move(MoveType::Capture, (Square) lsb, (Square) capturePiece, board->GetType((Square) capturePiece)));
        }

        //// En Passant
        captures = board->enPassant & PawnAttacks[(int) color][(int) lsb] & (U64) enPassantRank;
        while (captures) {
            int capturePiece = Utilities::LSB_Pop(&captures);
            if (isKingSafe || IsKingSafe(board, board->occupiedBB ^ C64(lsb),  board->colorBB[(int) oppColor] ^ C64(capturePiece)))
                AppendMove(moves, startIndex + moveCount, &moveCount, Move(MoveType::EPCapture, (Square) lsb, (Square) capturePiece, PieceType::Pawn));
        }

        // Quiet move
        //// Single push
        if (!(board->occupiedBB & pawnSingleMove[(int) lsb]))
            if (isKingSafe || IsKingSafe(board, (board->occupiedBB ^ C64(lsb)) | C64(lsb + (int) up)))
                AppendMove(moves, startIndex + moveCount, &moveCount, Move(MoveType::Quiet, (Square) lsb, (Square) (lsb + (int) up)));
        //// Double push
        if (C64(lsb) & (U64) doubleRank && !(board->occupiedBB & pawnDoubleMove[(int) lsb]))
            if (isKingSafe || IsKingSafe(board, (board->occupiedBB ^ C64(lsb)) | C64(lsb + (int) up * 2)))
                AppendMove(moves, startIndex + moveCount, &moveCount, Move(MoveType::DoublePawnPush, (Square) lsb, (Square) (lsb + (int) up * 2)));
    }
    return moveCount;
}

U8 MoveGen::GetRookMoves(std::array<Move, MAXMOVECOUNT> *moves, int startIndex, Board *board, bool isKingSafe, U64 attackedSquares) {
    int moveCount = 0;

    U64 pieces = board->pieceBB[(int) PieceType::Rook] & board->colorBB[(int) color];
    for (int i = 0; i < 4; i++)
        moveCount += GetMoves(moves, startIndex + moveCount, board, pieces, rookDirections[i], PieceType::Rook, isKingSafe);
    return moveCount;
}

U8 MoveGen::GetBishopMoves(std::array<Move, MAXMOVECOUNT> *moves, int startIndex, Board *board, bool isKingSafe, U64 attackedSquares) {
    int moveCount = 0;

    U64 pieces = board->pieceBB[(int) PieceType::Bishop] & board->colorBB[(int) color];
    for (int i = 0; i < 4; i++)
        moveCount += GetMoves(moves, startIndex + moveCount, board, pieces, bishopDirections[i], PieceType::Bishop, isKingSafe);
    return moveCount;
}

U8 MoveGen::GetQueenMoves(std::array<Move, MAXMOVECOUNT> *moves, int startIndex, Board *board, bool isKingSafe, U64 attackedSquares) {
    int moveCount = 0;

    U64 pieces = board->pieceBB[(int) PieceType::Queen] & board->colorBB[(int) color];
    for (int i = 0; i < 8; i++)
        moveCount += GetMoves(moves, startIndex + moveCount, board, pieces, queenDirections[i], PieceType::Queen, isKingSafe);
    return moveCount;
}

U8 MoveGen::GetKnightMoves(std::array<Move, MAXMOVECOUNT> *moves, int startIndex, Board *board, bool isKingSafe, U64 attackedSquares) {
    U64 pieces = board->pieceBB[(int) PieceType::Knight] & board->colorBB[(int) color];
    if (pieces == 0)
        return 0;
    U8 moveCount = 0;

    while (pieces) {
            int lsbPiece = Utilities::LSB_Pop(&pieces);
            U64 to = KnightMoves[lsbPiece];

            // Attack moves
            U64 attackMoves = to & board->colorBB[(int) oppColor];
            to &= ~board->occupiedBB;

            while (attackMoves) {
                int lsb = Utilities::LSB_Pop(&attackMoves);
                if (isKingSafe || IsKingSafe(board, board->occupiedBB ^ C64(lsbPiece), board->colorBB[(int) oppColor] ^ C64(lsb)))
                    AppendMove(moves, startIndex + moveCount, &moveCount, Move(MoveType::Capture, (Square) lsbPiece, (Square) lsb, board->GetType((Square) lsb)));
            }

            // Quiet moves
            U64 quietMoves = to & (~board->occupiedBB);
            while (quietMoves) {
                int lsb = Utilities::LSB_Pop(&quietMoves);
                if (isKingSafe || IsKingSafe(board, (board->occupiedBB ^ C64(lsbPiece) | C64(lsb))))
                    AppendMove(moves, startIndex + moveCount, &moveCount, Move(MoveType::Quiet, (Square) lsbPiece, (Square) lsb));
            }
    }

    return moveCount;
}

U8 MoveGen::GetKingMoves(std::array<Move, MAXMOVECOUNT> *moves, int startIndex, Board *board, bool isKingSafe, U64 attackedSquares) {
    U64 pieces = board->pieceBB[(int) PieceType::King] & board->colorBB[(int) color];
    if (pieces == 0)
        return 0;
    U8 moveCount = 0;

    int lsbPiece = Utilities::LSB_Pop(&pieces);
    U64 to = KingMoves[lsbPiece];
    // Attack moves
    U64 attackMoves = to & board->colorBB[(int) oppColor];
    // King cannot move onto a piece, without it being an attack
    to &= ~board->occupiedBB;
    // King cannot move to an attacked square
    to &= ~attackedSquares;
    // King cannot attack an attacked square
    attackMoves &= ~attackedSquares;

    while (attackMoves) {
        int lsb = Utilities::LSB_Pop(&attackMoves);
        if (isKingSafe || IsKingSafe(board, board->occupiedBB ^ C64(lsbPiece), board->colorBB[(int) oppColor] ^ C64(lsb), C64(lsb)))
            AppendMove(moves, startIndex + moveCount, &moveCount, Move(MoveType::Capture, (Square) lsbPiece, (Square) lsb, board->GetType((Square) lsb)));
    }

    // Quiet moves
    U64 quietMoves = to & (~board->occupiedBB);
    while (quietMoves) {
        int lsb = Utilities::LSB_Pop(&quietMoves);
        if (isKingSafe || IsKingSafe(board, (board->occupiedBB ^ C64(lsbPiece) | C64(lsb)), board->colorBB[(int) oppColor], C64(lsb)))
            AppendMove(moves, startIndex + moveCount, &moveCount, Move(MoveType::Quiet, (Square) lsbPiece, (Square) lsb));
    }

    // Castling
    if (board->castlingAllowed[(int)color][(int) Castling::King] && !(board->occupiedBB & (U64) castlingBlock[(int) Castling::King]) && !(attackedSquares & (U64) castlingAttack[(int) Castling::King]))
        if (isKingSafe)
            AppendMove(moves, startIndex + moveCount, &moveCount, Move(MoveType::KingCastle, ((color == Color::White) ? Square::E1 : Square::E8), (color == Color::White) ? Square::G1 : Square::G8));
    if (board->castlingAllowed[(int)color][(int) Castling::Queen] && !(board->occupiedBB & (U64) castlingBlock[(int) Castling::Queen]) && !(attackedSquares & (U64) castlingAttack[(int) Castling::Queen]))
        if (isKingSafe)
            AppendMove(moves, startIndex + moveCount, &moveCount, Move(MoveType::QueenCastle, ((color == Color::White) ? Square::E1 : Square::E8), (color == Color::White) ? Square::C1 : Square::C8));
        
    return moveCount;
}

U8 MoveGen::GetMoves(std::array<Move, MAXMOVECOUNT> *moves, int startIndex, Board *board, U64 pieces, Direction direction, PieceType type, bool isKingSafe) {
    U8 moveCount = 0;
    int counter = 1;
    while (pieces) {
        pieces = BitShifts::Shift(pieces & Utilities::NotEdge(direction), direction, 1) & ~board->colorBB[(int) color];
        U64 attackMoves = pieces & board->occupiedBB;
        pieces &= ~board->occupiedBB;

        while (attackMoves) {
            int lsb = Utilities::LSB_Pop(&attackMoves);
            if (isKingSafe || IsKingSafe(board, board->occupiedBB ^ C64(lsb - (int) direction * counter), board->colorBB[(int) oppColor] ^ C64(lsb)))
                AppendMove(moves, startIndex + moveCount, &moveCount, Move(MoveType::Capture, (Square) (lsb - (int) direction * counter), (Square) lsb, board->GetType((Square) lsb)));
        }

        U64 quietMoves = pieces;

        while (quietMoves) {
            int lsb = Utilities::LSB_Pop(&quietMoves);
            if (isKingSafe || IsKingSafe(board, (board->occupiedBB ^ C64(lsb - (int) direction * counter) | C64(lsb))))
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
    int kingPosIndex = Utilities::LSB_Pop(&kingPos);

    U64 enemyRooks = (board->pieceBB[(int) PieceType::Rook] | board->pieceBB[(int) PieceType::Queen]) & tempEnemyBoard;
    U64 enemyBishops = (board->pieceBB[(int) PieceType::Bishop] | board->pieceBB[(int) PieceType::Queen]) & tempEnemyBoard;
    U64 enemyKnights = board->pieceBB[(int) PieceType::Knight] & tempEnemyBoard;
    U64 enemyPawns = board->pieceBB[(int) PieceType::Pawn] & tempEnemyBoard;

    if (BitShifts::rays[kingPosIndex][(int) DirectionIndex::North] & enemyRooks)
        if (Utilities::LSB(BitShifts::rays[kingPosIndex][(int) DirectionIndex::North] & enemyRooks) == Utilities::LSB(BitShifts::rays[kingPosIndex][(int) DirectionIndex::North] & tempOccuracyBoard))
            return false;
    if (BitShifts::rays[kingPosIndex][(int) DirectionIndex::East] & enemyRooks)
        if (Utilities::LSB(BitShifts::rays[kingPosIndex][(int) DirectionIndex::East] & enemyRooks) == Utilities::LSB(BitShifts::rays[kingPosIndex][(int) DirectionIndex::East] & tempOccuracyBoard))
            return false;
    if (BitShifts::rays[kingPosIndex][(int) DirectionIndex::South] & enemyRooks)
        if (Utilities::MSB(BitShifts::rays[kingPosIndex][(int) DirectionIndex::South] & enemyRooks) == Utilities::MSB(BitShifts::rays[kingPosIndex][(int) DirectionIndex::South] & tempOccuracyBoard))
            return false;
    if (BitShifts::rays[kingPosIndex][(int) DirectionIndex::West] & enemyRooks)
        if (Utilities::MSB(BitShifts::rays[kingPosIndex][(int) DirectionIndex::West] & enemyRooks) == Utilities::MSB(BitShifts::rays[kingPosIndex][(int) DirectionIndex::West] & tempOccuracyBoard))
            return false;

    if (BitShifts::rays[kingPosIndex][(int) DirectionIndex::NorthEast] & enemyBishops)
        if (Utilities::LSB(BitShifts::rays[kingPosIndex][(int) DirectionIndex::NorthEast] & enemyBishops) == Utilities::LSB(BitShifts::rays[kingPosIndex][(int) DirectionIndex::NorthEast] & tempOccuracyBoard))
            return false;
    if (BitShifts::rays[kingPosIndex][(int) DirectionIndex::NorthWest] & enemyBishops)
        if (Utilities::LSB(BitShifts::rays[kingPosIndex][(int) DirectionIndex::NorthWest] & enemyBishops) == Utilities::LSB(BitShifts::rays[kingPosIndex][(int) DirectionIndex::NorthWest] & tempOccuracyBoard))
            return false;
    if (BitShifts::rays[kingPosIndex][(int) DirectionIndex::SouthEast] & enemyBishops)
        if (Utilities::MSB(BitShifts::rays[kingPosIndex][(int) DirectionIndex::SouthEast] & enemyBishops) == Utilities::MSB(BitShifts::rays[kingPosIndex][(int) DirectionIndex::SouthEast] & tempOccuracyBoard))
            return false;
    if (BitShifts::rays[kingPosIndex][(int) DirectionIndex::SouthWest] & enemyBishops)
        if (Utilities::MSB(BitShifts::rays[kingPosIndex][(int) DirectionIndex::SouthWest] & enemyBishops) == Utilities::MSB(BitShifts::rays[kingPosIndex][(int) DirectionIndex::SouthWest] & tempOccuracyBoard))
            return false;

    if (KnightMoves[kingPosIndex] & enemyKnights)
        return false;

    if (PawnAttacks[(int) color][kingPosIndex] & enemyPawns)
        return false;

    return true;
}

bool MoveGen::IsKingSafe(Board *board, U64 tempOccuracyBoard, U64 tempEnemyBoard) {
    return IsKingSafe(board, tempOccuracyBoard, tempEnemyBoard, board->pieceBB[(int) PieceType::King] & board->colorBB[(int) color]);
}

bool MoveGen::IsKingSafe(Board *board, U64 tempOccuracyBoard) {
    return IsKingSafe(board, tempOccuracyBoard, board->colorBB[(int) oppColor]);
}

bool MoveGen::IsKingSafe(Board *board) {
    return IsKingSafe(board, board->occupiedBB);
}

U64 MoveGen::GetAttackSquares(Board *board) {
    U64 attacks = 0;

    U64 pawns   = board->GetPiecePos(color, PieceType::Pawn);
    U64 knights = board->GetPiecePos(color, PieceType::Knight);
    U64 bishops = board->GetPiecePos(color, PieceType::Bishop);
    U64 rooks   = board->GetPiecePos(color, PieceType::Rook);
    U64 queens  = board->GetPiecePos(color, PieceType::Queen);
    U64 kings   = board->GetPiecePos(color, PieceType::King);

    // Non-sliding Pieces
    while (pawns)   attacks |= PawnAttacks[(int) color][Utilities::LSB_Pop(&pawns)];
    while (knights) attacks |= KnightMoves             [Utilities::LSB_Pop(&knights)];
    while (kings)   attacks |= KingMoves               [Utilities::LSB_Pop(&kings)];
    // Sliding Pieces
    while (bishops) attacks |= GetSlidingAttacks(board, Utilities::LSB_Pop(&bishops), bishopDirections, 4);
    while (rooks)   attacks |= GetSlidingAttacks(board, Utilities::LSB_Pop(&rooks),   rookDirections,   4);
    while (queens)  attacks |= GetSlidingAttacks(board, Utilities::LSB_Pop(&queens),  queenDirections,  8);

    return attacks;
}

U64 MoveGen::GetSlidingAttacks(const Board *board, const U64 pieceIndex, const Direction directions[], const int directionCount) const {
    U64 attacks = 0;
    
    U64 piece = C64(pieceIndex);
    for (int dirIndex = 0; dirIndex < directionCount; dirIndex++) {
        U64 workingPiece = piece;
        for (int offSet = 1; offSet < 8; offSet++) {
            workingPiece = BitShifts::Shift(workingPiece & Utilities::NotEdge(directions[dirIndex]), directions[dirIndex], 1);
            
            // If hitting friendly
            if (workingPiece & (board->GetOccupiedBB() & board->colorBB[(int) Utilities::GetOppositeColor(board->GetColor())]))
                break;

            // If hitting friendly
            if (workingPiece & (board->GetOccupiedBB() & board->colorBB[(int) board->GetColor()])) {
                attacks |= workingPiece;
                break;
            }
            // If unoccupied
            attacks |= workingPiece;
        }
    }

    return attacks;
}
