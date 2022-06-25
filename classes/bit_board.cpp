#include "headers/bit_board.hh"

BitBoard::BitBoard() {
    ClearBoard();
}

void BitBoard::ClearBoard() {
    for (int i = 0; i < PIECECOUNT; i++)
        pieceBB[i] = {};
    for (int i = 0; i < COLORCOUNT; i++)
        colorBB[i] = 0;
    for (int i = 0; i < COLORCOUNT; i++)
        for (int i2 = 0; i2 < PIECECOUNT; i2++)
            popCount[i][i2] = 0;
    enPassant = 0;
    occupiedBB = 0;
}

void BitBoard::Initialize() {
    ClearBoard();
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

    color = Color::White;
}

PieceType BitBoard::GetPiece(Square square) {
    for (int i = 0; i < PIECECOUNT; i++)
        if (pieceBB[i] & C64(square))
            return (PieceType) i;
    return PieceType::None;	
}

Color BitBoard::GetSquareColor(Square square) {
    if (colorBB[(int) Color::White] & C64(square))
        return Color::White;
    if (colorBB[(int) Color::Black] & C64(square))
        return Color::Black;
    else
        return Color::None;
}

void BitBoard::DoMove(Move move) {
    totalMoves++;
    PlacePiece(move.toSquare, move.fromType, move.fromColor);
    RemovePiece(move.fromSquare, move.fromType, move.fromColor);
    
    if ((short) move.type & CaptureBit) {
        if (move.type == MoveType::EPCapture) {
            Square captureSquare = (Square) ((move.fromColor == Color::White) ? (int) move.toSquare - 8 : (int) move.toSquare + 8);
            RemovePiece(captureSquare, move.toType, move.toColor);
        }
        else 
            RemovePiece(move.toSquare, move.toType, move.toColor);   
    }
        

    if (move.type == MoveType::DoublePawnPush)
        enPassant |= (U64) Utilities::GetColumn(move.fromSquare);
    else
        enPassant ^= (U64) Utilities::GetColumn(move.fromSquare);

    color = Utilities::GetOppositeColor(color);
}

void BitBoard::UndoMove(Move move) {
    RemovePiece(move.toSquare, move.fromType, move.fromColor);
    PlacePiece(move.fromSquare, move.fromType, move.fromColor);

    if ((short) move.type & CaptureBit) {
        if (move.type == MoveType::EPCapture) {
            Square captureSquare = (Square) ((move.fromColor == Color::White) ? (int) move.toSquare - 8 : (int) move.toSquare + 8);
            PlacePiece(captureSquare, move.toType, move.toColor);
        }
        else 
            PlacePiece(move.toSquare, move.toType, move.toColor);
    }

    color = Utilities::GetOppositeColor(color);
}

void BitBoard::PlacePiece(Square square, PieceChar pieceChar) {
    PlacePiece(square, Utilities::GetPieceType(pieceChar), Utilities::GetPieceColor(pieceChar));
}

void BitBoard::PlacePiece(Square square, PieceType type, Color color) {
    U64 bit = C64(square);
    pieceBB[(int) type] |= bit;
    colorBB[(int) color] |= bit;
    occupiedBB |= bit;
    popCount[(int) color][(int) type]++;
}

void BitBoard::RemovePiece(Square square, PieceType type, Color color) {
    U64 bit = C64(square);
    pieceBB[(int) type] ^= bit;
    colorBB[(int) color] ^= bit;
    occupiedBB ^= bit;
    popCount[(int) color][(int) type]--;
}

PieceType BitBoard::GetType(Square square, Color color) {
    U64 bit = C64(square);
    for (int i = 0; i < 6; i++) {
        if (pieceBB[i] & bit)
            return (PieceType) i;
    }
    return PieceType::None;
}
