#include "board.hh"
#include "bit_shifts.hh"

void Board::Initialize() {
    for (U8 x = 0; x < WIDTH; ++x) {
        PlacePiece(Utilities::GetSquare(x, PAWNROWWHITE), PieceType::Pawn, Color::White);
        PlacePiece(Utilities::GetSquare(x, PAWNROWBLACK), PieceType::Pawn, Color::Black);
    }

    PlacePiece(Square::A1, PieceType::Rook, Color::White);
    PlacePiece(Square::H1, PieceType::Rook, Color::White);
    PlacePiece(Square::A8, PieceType::Rook, Color::Black);
    PlacePiece(Square::H8, PieceType::Rook, Color::Black);

    PlacePiece(Square::B1, PieceType::Knight, Color::White);
    PlacePiece(Square::G1, PieceType::Knight, Color::White);
    PlacePiece(Square::B8, PieceType::Knight, Color::Black);
    PlacePiece(Square::G8, PieceType::Knight, Color::Black);

    PlacePiece(Square::C1, PieceType::Bishop, Color::White);
    PlacePiece(Square::F1, PieceType::Bishop, Color::White);
    PlacePiece(Square::C8, PieceType::Bishop, Color::Black);
    PlacePiece(Square::F8, PieceType::Bishop, Color::Black);

    PlacePiece(Square::D1, PieceType::Queen, Color::White);
    PlacePiece(Square::D8, PieceType::Queen, Color::Black);

    PlacePiece(Square::E1, PieceType::King, Color::White);
    PlacePiece(Square::E8, PieceType::King, Color::Black);
    
    for (U8 i = 0; i < COLORCOUNT; ++i)
        for (U8 i2 = 0; i2 < 2; ++i2)
            castlingAllowed[i][i2] = true;

    turn = Color::White;
    oppColor = Color::Black;
}

void Board::DoMove(Move &move) {
    PieceType fromType;
#ifdef STATS
    ++stats.totalMoves;
#endif
    if (move.GetType() == MoveType::KingCastle) {
        if (turn == Color::White) {
            PlacePiece(Square::G1, PieceType::King, Color::White);
            PlacePiece(Square::F1, PieceType::Rook, Color::White);
            RemovePiece(Square::E1, PieceType::King, Color::White);
            RemovePiece(Square::H1, PieceType::Rook, Color::White);
        } else {
            PlacePiece(Square::G8, PieceType::King, Color::Black);
            PlacePiece(Square::F8, PieceType::Rook, Color::Black);
            RemovePiece(Square::E8, PieceType::King, Color::Black);
            RemovePiece(Square::H8, PieceType::Rook, Color::Black);
        }
        fromType = PieceType::King;
#ifdef STATS
        ++stats.castlingMoves;
#endif
    } else if (move.GetType() == MoveType::QueenCastle) {
        if (turn == Color::White) {
            PlacePiece(Square::C1, PieceType::King, Color::White);
            PlacePiece(Square::D1, PieceType::Rook, Color::White);
            RemovePiece(Square::E1, PieceType::King, Color::White);
            RemovePiece(Square::A1, PieceType::Rook, Color::White);
        } else {
            PlacePiece(Square::C8, PieceType::King, Color::Black);
            PlacePiece(Square::D8, PieceType::Rook, Color::Black);
            RemovePiece(Square::E8, PieceType::King, Color::Black);
            RemovePiece(Square::A8, PieceType::Rook, Color::Black);
        }
        fromType = PieceType::King;
#ifdef STATS
        ++stats.castlingMoves;
#endif
    } else {
        fromType = GetType(move.GetFrom());
        RemovePiece(move.GetFrom(), fromType, turn);
        
        if (move.IsCapture()) {
            if (move.GetType() == MoveType::EPCapture) {
                auto captureSquare = (Square) ((turn == Color::White) ? (int) move.GetTo() - 8 : (int) move.GetTo() + 8);
                RemovePiece(captureSquare, PieceType::Pawn, oppColor);
#ifdef STATS
                ++stats.epMoves;
#endif
            } else
                RemovePiece(move.GetTo(), move.GetCapturedPiece(), oppColor);

            if (move.GetCapturedPiece() == PieceType::Rook && move.GetTo() == initRookPos[(U8) oppColor][(U8) Castling::King])
                if (castlingAllowed[(U8) oppColor][(U8) Castling::King])
                    DisableCastling(move, oppColor, Castling::King);
            if (move.GetCapturedPiece() == PieceType::Rook && move.GetTo() == initRookPos[(U8) oppColor][(U8) Castling::Queen])
                if (castlingAllowed[(U8) oppColor][(U8) Castling::Queen])
                    DisableCastling(move, oppColor, Castling::Queen);
        }
        if (move.IsPromotion()){
            if (move.GetType() == MoveType::RPromotion || move.GetType() == MoveType::RPromotionCapture)
                PlacePiece(move.GetTo(), PieceType::Rook, turn);
            else if (move.GetType() == MoveType::BPromotion || move.GetType() == MoveType::BPromotionCapture)
                PlacePiece(move.GetTo(), PieceType::Bishop, turn);
            else if (move.GetType() == MoveType::NPromotion || move.GetType() == MoveType::NPromotionCapture)
                PlacePiece(move.GetTo(), PieceType::Knight, turn);
            else if (move.GetType() == MoveType::QPromotion || move.GetType() == MoveType::QPromotionCapture)
                PlacePiece(move.GetTo(), PieceType::Queen, turn);
        } else
            PlacePiece(move.GetTo(), fromType, turn);
    }

    // En passant
    //// Check if there is old
    if (enPassant != Column::None)
        move.SetDisableEnPassant(enPassant);
    //// Set new
    if (move.GetType() == MoveType::DoublePawnPush) {
        SetEnPassant(Utilities::GetColumn(move.GetFrom()));
    } else
        SetEnPassant(Column::None);

    
    // Castling rights
    if (fromType == PieceType::King) {
        if (castlingAllowed[(U8) turn][(U8) Castling::King])
            DisableCastling(move, turn, Castling::King);
        if (castlingAllowed[(U8) turn][(U8) Castling::Queen])
            DisableCastling(move, turn, Castling::Queen);
    } else if (fromType == PieceType::Rook) {
        if (castlingAllowed[(U8) turn][(U8) Castling::King] && Utilities::GetColumn(move.GetFrom()) == Column::H)
            DisableCastling(move, turn, Castling::King);
        else if (castlingAllowed[(U8) turn][(U8) Castling::Queen] && Utilities::GetColumn(move.GetFrom()) == Column::A)
            DisableCastling(move, turn, Castling::Queen);
    }

    SwitchTurn();
    zobrist.IncrementHash();
    ++ply;
}

void Board::UndoMove(Move move) {
    zobrist.DecrementHash();
    PieceType toType;

    // Check if old en passant
    bool priorEP = false;
    if (move.IsDEP()) {
        SetEnPassant(move.GetDEP());
        priorEP = true;
    }

    if (move.GetType() == MoveType::KingCastle) {
        if (turn == Color::Black) {
            RemovePiece(Square::G1, PieceType::King, Color::White);
            RemovePiece(Square::F1, PieceType::Rook, Color::White);
            PlacePiece(Square::E1, PieceType::King, Color::White);
            PlacePiece(Square::H1, PieceType::Rook, Color::White);
        } else {
            RemovePiece(Square::G8, PieceType::King, Color::Black);
            RemovePiece(Square::F8, PieceType::Rook, Color::Black);
            PlacePiece(Square::E8, PieceType::King, Color::Black);
            PlacePiece(Square::H8, PieceType::Rook, Color::Black);
        }
    } else if (move.GetType() == MoveType::QueenCastle) {
        if (turn == Color::Black) {
            RemovePiece(Square::C1, PieceType::King, Color::White);
            RemovePiece(Square::D1, PieceType::Rook, Color::White);
            PlacePiece(Square::E1, PieceType::King, Color::White);
            PlacePiece(Square::A1, PieceType::Rook, Color::White);
        } else {
            RemovePiece(Square::C8, PieceType::King, Color::Black);
            RemovePiece(Square::D8, PieceType::Rook, Color::Black);
            PlacePiece(Square::E8, PieceType::King, Color::Black);
            PlacePiece(Square::A8, PieceType::Rook, Color::Black);
        }
    } else {
        toType = GetType(move.GetTo());
        if (move.IsPromotion()) {
            RemovePiece(move.GetTo(), toType, oppColor);
            PlacePiece(move.GetFrom(), PieceType::Pawn, oppColor);
        } else {
            RemovePiece(move.GetTo(), toType, oppColor);
            PlacePiece(move.GetFrom(), toType, oppColor);
            if (move.GetType() == MoveType::DoublePawnPush)
                if (!priorEP)
                    SetEnPassant(Column::None);
        }
        

        if (move.IsCapture()) {
            if (move.GetType() == MoveType::EPCapture) {
                auto captureSquare = (Square) ((turn == Color::Black) ? (int) move.GetTo() - 8 : (int) move.GetTo() + 8);
                PlacePiece(captureSquare, PieceType::Pawn, turn);
            }
            else 
                PlacePiece(move.GetTo(), move.GetCapturedPiece(), turn);
        }
    }
    EnableCastling(move);
    SwitchTurn();
    --ply;
}

void Board::EnableCastling(Move &move) {
    auto enableCastling = [&](Color color, Castling castling) {
        castlingAllowed[(int) color][(int) castling] = true;
        zobrist.FlipCastling(color, castling);
    };
    if (move.IsDC()) {
        if (move.IsDC(Color::White, Castling::King))
            enableCastling(Color::White, Castling::King);
        if (move.IsDC(Color::White, Castling::Queen))
            enableCastling(Color::White, Castling::Queen);
        if (move.IsDC(Color::Black, Castling::King))
            enableCastling(Color::Black, Castling::King);
        if (move.IsDC(Color::Black, Castling::Queen))
            enableCastling(Color::Black, Castling::Queen);
    }
}

void Board::DisableCastling(Move &move, Color color, Castling side) {
    zobrist.FlipCastling(color, side);
    castlingAllowed[(U8) color][(U8) side] = false;
    move.SetDisableCastle(color, side);
}

U64 Board::GenerateAttackSquares(Color color) const {
    U64 attacks = 0;

    U64 tempPieces[PIECECOUNT];
    for (U8 pIndex = 0; pIndex < PIECECOUNT; pIndex++)
        tempPieces[pIndex] = GetPiecePos(color, (PieceType) pIndex);

    while (tempPieces[(U8) PieceType::Pawn]) attacks |= PawnAttacks[(int) color][Utilities::LSB_Pop(&tempPieces[(U8) PieceType::Pawn])];

    for (U8 pIndex = 1; pIndex < PIECECOUNT; ++pIndex)
        while (tempPieces[pIndex]) {
            unsigned short pos = Utilities::LSB_Pop(&tempPieces[pIndex]);
            U64 attacks1 = BitShifts::GetAttacks(pos, (PieceType) pIndex);

            for (U64 b = occupiedBB & BitShifts::GetBB(pos, (PieceType) pIndex); b != 0; b &= (b - 1))
                attacks1 &= ~BitShifts::GetBehind(pos, Utilities::LSB(b));

            attacks |= attacks1;
        }

    return attacks;
}

bool Board::IsKingSafe(U64 tempOccuracyBoard, U64 tempEnemyBoard, U64 tempKingBoard) {
    if (tempKingBoard == 0)
        return true;
    U64 kingPosIndex = Utilities::LSB_Pop(&tempKingBoard);

    U64 enemyRooks = (GetPiecePos(PieceType::Rook) | GetPiecePos(PieceType::Queen)) & tempEnemyBoard;
    U64 enemyBishops = (GetPiecePos(PieceType::Bishop) | GetPiecePos(PieceType::Queen)) & tempEnemyBoard;
    U64 enemyKnights = GetPiecePos(PieceType::Knight) & tempEnemyBoard;
    U64 enemyPawns = GetPiecePos(PieceType::Pawn) & tempEnemyBoard;

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

    if (PawnAttacks[(int) turn][kingPosIndex] & enemyPawns)
        return false;

    return true;
}

void Board::SetEnPassant(Column col) {
    if (enPassant != Column::None)
        zobrist.FlipEnPassant(enPassant);
    if (col != Column::None)
        zobrist.FlipEnPassant(col);
    enPassant = col;

}
