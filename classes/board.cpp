#include "headers/board.hh"

void Board::Initialize() {
    for (U8 x = 0; x < WIDTH; x++) {
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
    
    for (U8 i = 0; i < COLORCOUNT; i++)
        for (U8 i2 = 0; i2 < 2; i2++)
            castlingAllowed[i][i2] = true;

    turn = Color::White;
    originalColor = Color::White;
}

void Board::DoMove(Move &move) {
    PieceType fromType;
#ifdef STATS
    stats.totalMoves++;
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
        stats.castlingMoves++;
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
        stats.castlingMoves++;
#endif
    } else {
        fromType = GetType(move.GetFrom());
        RemovePiece(move.GetFrom(), fromType, turn);
        
        if (move.IsCapture()) {
            if (move.GetType() == MoveType::EPCapture) {
                auto captureSquare = (Square) ((turn == Color::White) ? (int) move.GetTo() - 8 : (int) move.GetTo() + 8);
                RemovePiece(captureSquare, PieceType::Pawn, Utilities::GetOppositeColor(turn));
#ifdef STATS
                stats.epMoves++;
#endif
            }
            else 
                RemovePiece(move.GetTo(), move.GetCapturedPiece(), Utilities::GetOppositeColor(turn));
        }
        PlacePiece(move.GetTo(), fromType, turn);
    }

    // En passant
    //// Check if there is old
    if (enPassant)
        move.SetDisableEnPassant((Column) enPassant);
    //// Set new
    if (move.GetType() == MoveType::DoublePawnPush)
        enPassant = (U64) Utilities::GetColumn(move.GetFrom());
    else
        enPassant = 0;
    
    // Castling rights
    if (fromType == PieceType::King) {
        if (castlingAllowed[(U8) turn][(U8) Castling::King])
            DisableCastling(move, turn, Castling::King);
        if (castlingAllowed[(U8) turn][(U8) Castling::Queen])
            DisableCastling(move, turn, Castling::Queen);
    } else if (fromType == PieceType::Rook) {
        if (turn == Color::White) {
            if (castlingAllowed[(U8) turn][(U8) Castling::King] && move.GetFrom() == Square::H1)
                DisableCastling(move, turn, Castling::King);
            else if (castlingAllowed[(U8) turn][(U8) Castling::Queen] && move.GetFrom() == Square::A1)
                DisableCastling(move, turn, Castling::Queen);
        } else if (turn == Color::Black) {
            if (castlingAllowed[(U8) turn][(U8) Castling::King] && move.GetFrom() == Square::H8)
                DisableCastling(move, turn, Castling::King);
            else if (castlingAllowed[(U8) turn][(U8) Castling::Queen] && move.GetFrom() == Square::A8)
                DisableCastling(move, turn, Castling::Queen);
        }
    }

    turn = Utilities::GetOppositeColor(turn);
    zobrist.IncrementHash();
    ply++;
}

void Board::UndoMove(Move move) {
    zobrist.DecrementHash();
    PieceType toType;

    // Check if old en passant
    if (move.IsDEP())
        enPassant = (U64) move.GetDEP();

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
            RemovePiece(move.GetTo(), toType, Utilities::GetOppositeColor(turn));
            PlacePiece(move.GetTo(), PieceType::Pawn, Utilities::GetOppositeColor(turn));
        } else {
            RemovePiece(move.GetTo(), toType, Utilities::GetOppositeColor(turn));
            PlacePiece(move.GetFrom(), toType, Utilities::GetOppositeColor(turn));
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
    turn = Utilities::GetOppositeColor(turn);
    ply--;
}

void Board::EnableCastling(Move &move) {
    if (move.IsDC()) {
        if (move.IsDC(Color::White, Castling::King))
            castlingAllowed[(int) Color::White][(int) Castling::King] = true;
        if (move.IsDC(Color::White, Castling::Queen))
            castlingAllowed[(int) Color::White][(int) Castling::Queen] = true;
        if (move.IsDC(Color::Black, Castling::King))
            castlingAllowed[(int) Color::Black][(int) Castling::King] = true;
        if (move.IsDC(Color::Black, Castling::Queen))
            castlingAllowed[(int) Color::Black][(int) Castling::Queen] = true;
    }
}

void Board::DisableCastling(Move &move, Color color, Castling side) {
    castlingAllowed[(U8) color][(U8) side] = false;
    move.SetDisableCastle(color, side);
}