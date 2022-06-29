#include "headers/move_gen.hh"

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
    GenerateKnightMoves();
    GenerateKingMoves();
    GenerateKingThreats();
}

int MoveGen::GetAllMoves(Move* moves, BitBoard board, U64* attackedSquares) {
    U64 priorSquares = *attackedSquares;
    *attackedSquares = 0;
    int moveCount = 0;
    moveCount += GetPawnMoves(moves, moveCount, board, attackedSquares);
    moveCount += GetRookMoves(moves, moveCount, board, attackedSquares);
    moveCount += GetBishopMoves(moves, moveCount, board, attackedSquares);
    moveCount += GetQueenMoves(moves, moveCount, board, attackedSquares);
    moveCount += GetKnightMoves(moves, moveCount, board, attackedSquares);
    moveCount += GetKingMoves(moves, moveCount, board, attackedSquares, priorSquares);
    return moveCount;
}

int MoveGen::GetPawnMoves(Move* moves, int startIndex, BitBoard board, U64* attackedSquares) {
    // Generate single and double step moves
    U64 pieces = board.pieceBB[(int) PieceType::Pawn] & board.colorBB[(int) color];
    if (pieces == 0)
        return 0;
        
    int moveCount = 0;

    while (pieces) {
        int lsb = Utilities::LSB_Pop(&pieces);

        // Quiet move
        //// Single push
        if (!(board.occupiedBB & pawnSingleMove[(int) lsb])) {
            moves[startIndex + moveCount] = 
            Move(MoveType::Quiet, (Square) lsb, (Square) (lsb + (int) up), color, Color::None, PieceType::Pawn, PieceType::None);
            moveCount++;
        }
        //// Double push
        if (C64(lsb) & (U64) doubleRank && !(board.occupiedBB & pawnDoubleMove[(int) lsb])) {
            moves[startIndex + moveCount] = 
            Move(MoveType::Quiet, (Square) lsb, (Square) (lsb + (int) up * 2), color, Color::None, PieceType::Pawn, PieceType::None);
            moveCount++;
        }
        
        // Attack moves
        //// Diagonal
        U64 captures = board.colorBB[(int) oppColor] & pawnCaptureMoves[(int) lsb];
        while (captures) {
            int capturePiece = Utilities::LSB_Pop(&captures);
            moves[startIndex + moveCount] = 
            Move(MoveType::Capture, (Square) lsb, (Square) capturePiece, color, oppColor, PieceType::Pawn, board.GetType((Square) capturePiece, oppColor));
            moveCount++;
        }
        //// En Passant
        captures = board.enPassant & pawnCaptureMoves[(int) lsb] & (U64) enPassantRank;
        while (captures) {
            int capturePiece = Utilities::LSB_Pop(&captures);
            moves[startIndex + moveCount] = 
            Move(MoveType::EPCapture, (Square) lsb, (Square) capturePiece, color, oppColor, PieceType::Pawn, board.GetType((Square) BitShifts::Shift(capturePiece, up, -1), oppColor));
            moveCount++;
        }
    }

    return moveCount;
}

int MoveGen::GetRookMoves(Move* moves, int startIndex, BitBoard board, U64* attackedSquares) {
    Direction directions[4] = { Direction::North, Direction::East, Direction::South, Direction::West };
    int moveCount = 0;

    U64 pieces = board.pieceBB[(int) PieceType::Rook] & board.colorBB[(int) color];
    if (pieces == 0)
        return 0;
    for (int i = 0; i < 4; i++)
        moveCount += GetMoves(moves, startIndex, board, pieces, directions[i], PieceType::Rook, attackedSquares);
    return moveCount;
}

int MoveGen::GetBishopMoves(Move* moves, int startIndex, BitBoard board, U64* attackedSquares) {
    Direction directions[4] = { Direction::NorthEast, Direction::NorthWest, Direction::SouthEast, Direction::SouthWest };
    int moveCount = 0;

    U64 pieces = board.pieceBB[(int) PieceType::Bishop] & board.colorBB[(int) color];
    if (pieces == 0)
        return 0;
    for (int i = 0; i < 4; i++)
        moveCount += GetMoves(moves, startIndex, board, pieces, directions[i], PieceType::Bishop, attackedSquares);
    return moveCount;
}

int MoveGen::GetQueenMoves(Move* moves, int startIndex, BitBoard board, U64* attackedSquares) {
    Direction directions[8] = { Direction::North, Direction::East, Direction::South, Direction::West, 
                                Direction::NorthEast, Direction::NorthWest, Direction::SouthEast, Direction::SouthWest };
    int moveCount = 0;

    U64 pieces = board.pieceBB[(int) PieceType::Queen] & board.colorBB[(int) color];
    if (pieces == 0)
        return 0;
    for (int i = 0; i < 8; i++)
        moveCount += GetMoves(moves, startIndex, board, pieces, directions[i], PieceType::Queen, attackedSquares);
    return moveCount;
}

int MoveGen::GetKnightMoves(Move* moves, int startIndex, BitBoard board, U64* attackedSquares) {
    U64 pieces = board.pieceBB[(int) PieceType::Knight] & board.colorBB[(int) color];
    if (pieces == 0)
        return 0;
    int moveCount = 0;

    while (pieces) {
            int lsbPiece = Utilities::LSB_Pop(&pieces);
            U64 to = knightMoves[lsbPiece];
            (*attackedSquares) |= to;
            // Attack moves
            U64 attackMoves = to & board.colorBB[(int) oppColor];
            to &= ~board.occupiedBB;

            while (attackMoves) {
                int lsb = Utilities::LSB_Pop(&attackMoves);
                moves[startIndex + moveCount] = Move(MoveType::Capture, (Square) lsbPiece, (Square) lsb, color, oppColor, PieceType::Knight, board.GetType((Square) lsb, oppColor));
                moveCount++;
            }

            // Quiet moves
            U64 quietMoves = to & (~board.occupiedBB);
            while (quietMoves) {
                int lsb = Utilities::LSB_Pop(&quietMoves);
                moves[startIndex + moveCount] = Move(MoveType::Quiet, (Square) lsbPiece, (Square) lsb, color, Color::None, PieceType::Knight, PieceType::None);
                moveCount++;
            }
    }

    return moveCount;
}

int MoveGen::GetKingMoves(Move* moves, int startIndex, BitBoard board, U64* attackedSquares, U64 priorAttacks) {
    U64 pieces = board.pieceBB[(int) PieceType::King] & board.colorBB[(int) color];
    if (pieces == 0)
        return 0;
    int moveCount = 0;

    int lsbPiece = Utilities::LSB_Pop(&pieces);
    U64 to = kingMoves[lsbPiece];
    (*attackedSquares) |= to;
    // Attack moves
    U64 attackMoves = to & board.colorBB[(int) oppColor];
    to &= ~board.occupiedBB;
    to &= ~priorAttacks;
    attackMoves &= ~priorAttacks;

    while (attackMoves) {
        int lsb = Utilities::LSB_Pop(&attackMoves);
        moves[startIndex + moveCount] = Move(MoveType::Capture, (Square) lsbPiece, (Square) lsb, color, oppColor, PieceType::King, board.GetType((Square) lsb, oppColor));
        moveCount++;
    }

    // Quiet moves
    U64 quietMoves = to & (~board.occupiedBB);
    while (quietMoves) {
        int lsb = Utilities::LSB_Pop(&quietMoves);
        moves[startIndex + moveCount] = Move(MoveType::Quiet, (Square) lsbPiece, (Square) lsb, color, Color::None, PieceType::King, PieceType::None);
        moveCount++;
    }

    // Castling
    if (board.castlingAllowed[(int)color][(int) Castling::King] && !(board.occupiedBB & (U64) castlingBlock[(int) Castling::King]) && !(priorAttacks & (U64) castlingAttack[(int) Castling::King])) {
        moves[startIndex + moveCount] = Move(MoveType::KingCastle, color);
        moveCount++;
    } 
    if (board.castlingAllowed[(int)color][(int) Castling::Queen] && !(board.occupiedBB & (U64) castlingBlock[(int) Castling::Queen]) && !(priorAttacks & (U64) castlingAttack[(int) Castling::Queen])) {
        moves[startIndex + moveCount] = Move(MoveType::QueenCastle, color);
        moveCount++;
    }
        
    return moveCount;
}

int MoveGen::GetMoves(Move* moves, int startIndex, BitBoard board, U64 pieces, Direction direction, PieceType type, U64* attackedSquares) {
    U64 to = pieces;
    int moveCount = 0;
    int counter = 1;
    while (to) {
        to = BitShifts::Shift(to & Utilities::NotEdge(direction), direction, 1) & ~board.colorBB[(int) color];
        (*attackedSquares) |= to;
        U64 attackMoves = to & board.occupiedBB;
        to &= ~board.occupiedBB;

        while (attackMoves) {
            int lsb = Utilities::LSB_Pop(&attackMoves);
            moves[startIndex + moveCount] = Move(MoveType::Capture, (Square) (lsb - (int) direction * counter), (Square) lsb, color, oppColor, type, board.GetType((Square) lsb, oppColor));
            moveCount++;
        }

        U64 quietMoves = to;

        while (quietMoves) {
            int lsb = Utilities::LSB_Pop(&quietMoves);
            moves[startIndex + moveCount] = Move(MoveType::Quiet, (Square) (lsb - (int) direction * counter), (Square) lsb, color, Color::None, type, PieceType::None);
            moveCount++;
        }

        counter++;
    }

    return moveCount;
}

void MoveGen::GeneratePawnMoves() {
    for (int i = 0; i < 64; i++) {
        U64 bit = C64(i);
        pawnCaptureMoves[i] = 0;
        if (bit & (U64) notPromotionRank) {
            if (bit & (U64) NotEdge::West)
                pawnCaptureMoves[i] |= BitShifts::Shift(bit, upLeft, 1);
            if (bit & (U64) NotEdge::East)    
                pawnCaptureMoves[i] |= BitShifts::Shift(bit, upRight, 1);
            U64 upOne = BitShifts::Shift(bit, up, 1);
            pawnSingleMove[i] |= BitShifts::Shift(bit, up, 1);
            if (bit & (U64) doubleRank) {
                pawnDoubleMove[i] |= upOne;
                pawnDoubleMove[i] |= BitShifts::Shift(bit, up, 2);
            }
        }   
    }
}

void MoveGen::GenerateKnightMoves() {
    for (int i = 0; i < 64; i++) {
        U64 bit = C64(i);
        if (bit & (U64) NotEdgeKnight::North) {
            if (bit & (U64) NotEdge::East) {
                knightMoves[i] |= BitShifts::Shift(BitShifts::Shift(bit, Direction::North, 1), Direction::NorthEast, 1);
            }
            if (bit & (U64) NotEdge::West) {
                knightMoves[i] |= BitShifts::Shift(BitShifts::Shift(bit, Direction::North, 1), Direction::NorthWest, 1);
            }
        }
        if (bit & (U64) NotEdgeKnight::South) {
            if (bit & (U64) NotEdge::East) {
                knightMoves[i] |= BitShifts::Shift(BitShifts::Shift(bit, Direction::South, 1), Direction::SouthEast, 1);
            }
            if (bit & (U64) NotEdge::West) {
                knightMoves[i] |= BitShifts::Shift(BitShifts::Shift(bit, Direction::South, 1), Direction::SouthWest, 1);
            }
        }
        if (bit & (U64) NotEdgeKnight::East) {
            if (bit & (U64) NotEdge::North) {
                knightMoves[i] |= BitShifts::Shift(BitShifts::Shift(bit, Direction::North, 1), Direction::East, 2);
            }
            if (bit & (U64) NotEdge::South) {
                knightMoves[i] |= BitShifts::Shift(BitShifts::Shift(bit, Direction::South, 1), Direction::East, 2);
            }
        }
        if (bit & (U64) NotEdgeKnight::West) {
            if (bit & (U64) NotEdge::North) {
                knightMoves[i] |= BitShifts::Shift(BitShifts::Shift(bit, Direction::North, 1), Direction::West, 2);
            }
            if (bit & (U64) NotEdge::South) {
                knightMoves[i] |= BitShifts::Shift(BitShifts::Shift(bit, Direction::South, 1), Direction::West, 2);
            }
        }
    }
}

void MoveGen::GenerateKingMoves() {
    for (int i = 0; i < 64; i++) {
        U64 bit = C64(i);
        if (bit & (U64) NotEdge::North) {
            kingMoves[i] |= BitShifts::Shift(bit, Direction::North, 1);
            if (bit & (U64) NotEdge::East)
                kingMoves[i] |= BitShifts::Shift(bit, Direction::NorthEast, 1);
            if (bit & (U64) NotEdge::West)
                kingMoves[i] |= BitShifts::Shift(bit, Direction::NorthWest, 1);
        }
        if (bit & (U64) NotEdge::South) {
            kingMoves[i] |= BitShifts::Shift(bit, Direction::South, 1);
            if (bit & (U64) NotEdge::East)
                kingMoves[i] |= BitShifts::Shift(bit, Direction::SouthEast, 1);
            if (bit & (U64) NotEdge::West)
                kingMoves[i] |= BitShifts::Shift(bit, Direction::SouthWest, 1);
        }
        if (bit & (U64) NotEdge::East) {
            kingMoves[i] |= BitShifts::Shift(bit, Direction::East, 1);
        }
        if (bit & (U64) NotEdge::West) {
            kingMoves[i] |= BitShifts::Shift(bit, Direction::West, 1);
        }
    }
}

void MoveGen::GenerateKingThreats() {
    for (int i = 0; i < 64; i++) {
        kingThreatsRook[i] = 0;
        kingThreatsBishop[i] = 0;

        // Rook Threats
        kingThreatsRook[i] |= (U64) Utilities::GetRow((Square) i);
        kingThreatsRook[i] |= (U64) Utilities::GetColumn((Square) i);

        // Bishop Threats
        U64 tempBit = C64(i);
        while (tempBit & (U64) NotEdge::North && tempBit & (U64) NotEdge::West) {
            tempBit = BitShifts::Shift(tempBit, Direction::NorthWest, 1);
            kingThreatsBishop[i] |= tempBit;
        }
        tempBit = C64(i);
        while (tempBit & (U64) NotEdge::North && tempBit & (U64) NotEdge::East) {
            tempBit = BitShifts::Shift(tempBit, Direction::NorthEast, 1);
            kingThreatsBishop[i] |= tempBit;
        }
        tempBit = C64(i);
        while (tempBit & (U64) NotEdge::South && tempBit & (U64) NotEdge::West) {
            tempBit = BitShifts::Shift(tempBit, Direction::SouthWest, 1);
            kingThreatsBishop[i] |= tempBit;
        }
        tempBit = C64(i);
        while (tempBit & (U64) NotEdge::South && tempBit & (U64) NotEdge::East) {
            tempBit = BitShifts::Shift(tempBit, Direction::SouthEast, 1);
            kingThreatsBishop[i] |= tempBit;
        }
    }
}

bool MoveGen::IsKingSafe() {
    
    return true;
}

bool MoveGen::IsSafeMove(Square square) {

    return true;
}
