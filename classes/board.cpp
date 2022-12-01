#include "headers/board.hh"

void Board::Initialize() {
    for (int x = 0; x < WIDTH; x++) {
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
    
    for (int i = 0; i < COLORCOUNT; i++)
        for (int i2 = 0; i2 < 2; i2++) {
            castlingAllowed[i][i2] = true;
            movesSinceCastlingDisallowed[i][i2] = -1;
        }
            

    color = Color::White;
    originalColor = Color::White;
}

void Board::DoMove(Move move) {
#ifdef STATS
    stats.totalMoves++;
#endif
    if (move.type == MoveType::KingCastle) {
        if (move.fromColor == Color::White) {
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
    } else if (move.type == MoveType::QueenCastle) {
        if (move.fromColor == Color::White) {
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
    } else {
        RemovePiece(move.fromSquare, move.fromType, move.fromColor);
        
        if ((short) move.type & CaptureBit) {
            if (move.type == MoveType::EPCapture) {
                Square captureSquare = (Square) ((move.fromColor == Color::White) ? (int) move.toSquare - 8 : (int) move.toSquare + 8);
                RemovePiece(captureSquare, move.toType, move.toColor);
            }
            else 
                RemovePiece(move.toSquare, move.toType, move.toColor);   
        }
        PlacePiece(move.toSquare, move.fromType, move.fromColor);
    }

    // En passant    
    if (move.type == MoveType::DoublePawnPush)
        enPassant |= (U64) Utilities::GetColumn(move.fromSquare);
    else
        enPassant = 0;
    
    // Castling rights
    IncrementCastling();
    if (move.fromType == PieceType::King) {
        if (castlingAllowed[(int) move.fromColor][(int) Castling::King])
            DisableCastling(move.fromColor, Castling::King);
        if (castlingAllowed[(int) move.fromColor][(int) Castling::Queen])
            DisableCastling(move.fromColor, Castling::Queen);
    } else if (move.fromType == PieceType::Rook) {
        if (move.fromColor == Color::White) {
            if (castlingAllowed[(int) move.fromColor][(int) Castling::King] && move.fromSquare == Square::H1)
                DisableCastling(move.fromColor, Castling::King);
            else if (castlingAllowed[(int) move.fromColor][(int) Castling::Queen] && move.fromSquare == Square::A1)
                DisableCastling(move.fromColor, Castling::Queen);
        } else if (move.fromColor == Color::Black) {
            if (castlingAllowed[(int) move.fromColor][(int) Castling::King] && move.fromSquare == Square::H8)
                DisableCastling(move.fromColor, Castling::King);
            else if (castlingAllowed[(int) move.fromColor][(int) Castling::Queen] && move.fromSquare == Square::A8)
                DisableCastling(move.fromColor, Castling::Queen);
        }
    }

    color = Utilities::GetOppositeColor(color);
}

void Board::UndoMove(Move move) {
    if (move.type == MoveType::KingCastle) {
        if (move.fromColor == Color::White) {
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
    } if (move.type == MoveType::QueenCastle) {
        if (move.fromColor == Color::White) {
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
        RemovePiece(move.toSquare, move.fromType, move.fromColor);
        PlacePiece(move.fromSquare, move.fromType, move.fromColor);

        // En passant
        if ((short) move.type & CaptureBit) {
            if (move.type == MoveType::EPCapture) {
                Square captureSquare = (Square) ((move.fromColor == Color::White) ? (int) move.toSquare - 8 : (int) move.toSquare + 8);
                PlacePiece(captureSquare, move.toType, move.toColor);
            }
            else 
                PlacePiece(move.toSquare, move.toType, move.toColor);
        }
    }
    

    // Castling rights
    DecrementCastling();

    color = Utilities::GetOppositeColor(color);
}

void Board::EnableCastling(Color color, Castling side) {
    castlingAllowed[(int) color][(int) side] = true;
    movesSinceCastlingDisallowed[(int) color][(int) side] = -1;
}

void Board::DisableCastling(Color color, Castling side) {
    castlingAllowed[(int) color][(int) side] = false;
    movesSinceCastlingDisallowed[(int) color][(int) side] = 0;
}

void Board::IncrementCastling() {
    for (int i = 0; i < 2; i++) {
        for (int i2 = 0; i2 < 2; i2++) {
            if (movesSinceCastlingDisallowed[i][i2] != -1)
                movesSinceCastlingDisallowed[i][i2]++;
        }
    }
}

void Board::DecrementCastling() {
    for (int i = 0; i < 2; i++) {
        for (int i2 = 0; i2 < 2; i2++) {
            if (movesSinceCastlingDisallowed[i][i2] == 0) {
                castlingAllowed[i][i2] = true;
                movesSinceCastlingDisallowed[i][i2] = -1;
            } else if (movesSinceCastlingDisallowed[i][i2] != -1)
                movesSinceCastlingDisallowed[i][i2]--;
        }
    }
}
