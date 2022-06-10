#include "headers/board_OO.h"

Board_OO::Board_OO() {
    for (int x = 0; x < WIDTH; x++)
        for (int y = 0; y < HEIGHT; y++)
            pieces[x][y] = nullptr;
}

void Board_OO::Initialize() {
    for (int x = 0; x < WIDTH; x++) {
        pieces[x][PAWNROWWHITE] = new Pawn(Color::White, x, PAWNROWWHITE);
        pieces[x][PAWNROWBLACK] = new Pawn(Color::Black, x, PAWNROWBLACK);
    }
    for (int x = 0; x < WIDTH; x += 7) {
        pieces[x][BACKROWWHITE] = new Rook(Color::White, x, PAWNROWWHITE);
        pieces[x][BACKROWBLACK] = new Rook(Color::Black, x, PAWNROWBLACK);
    }
    for (int x = 1; x < WIDTH; x += 5) {
        pieces[x][BACKROWWHITE] = new Knight(Color::White, x, PAWNROWWHITE);
        pieces[x][BACKROWBLACK] = new Knight(Color::Black, x, PAWNROWBLACK);
    }
    for (int x = 2; x < WIDTH; x += 3) {
        pieces[x][BACKROWWHITE] = new Bishop(Color::White, x, PAWNROWWHITE);
        pieces[x][BACKROWBLACK] = new Bishop(Color::Black, x, PAWNROWBLACK);
    }
    pieces[3][BACKROWWHITE] = new Queen(Color::White, 3, PAWNROWWHITE);
    pieces[3][BACKROWBLACK] = new Queen(Color::Black, 3, PAWNROWBLACK);

    pieces[4][BACKROWWHITE] = new King(Color::White, 4, PAWNROWWHITE);
    pieces[4][BACKROWBLACK] = new King(Color::Black, 4, PAWNROWBLACK);
}

void Board_OO::ImportFEN(std::string FEN) {
    // Import Position
    for(int y = HEIGHT - 1; y >= 0; y--) {
		int remainingSquares = WIDTH;
		while(remainingSquares > 0) {
			char currentChar = FEN[0];
			if(isdigit(currentChar)) {
				remainingSquares -= (int) currentChar - 48;
			} else {
				PlacePiece((PieceChar) currentChar, WIDTH - remainingSquares, y);
				remainingSquares--;
			}
			FEN.erase(0, 1);
		}
		FEN.erase(0, 1);
	}
}

Piece* Board_OO::GetPiece(const int x, const int y) {
    return pieces[x][y];
}

void Board_OO::MovePiece(const int startX, const int startY, const int endX, const int endY) {
    Piece* piece = RemovePiece(startX, startY);
    PlacePiece(piece, endX, endY);
}

void Board_OO::PlacePiece(Piece* piece, const int x, const int y) {
    pieces[x][y] = piece;
}

void Board_OO::PlacePiece(const PieceChar piece, const int x, const int y) {
    switch (piece)
    {
    case PieceChar::PawnWhite:
        PlacePiece(new Pawn(Color::White, x, y), x, y);
        break;
    case PieceChar::PawnBlack:
        PlacePiece(new Pawn(Color::Black, x, y), x, y);
        break;
    case PieceChar::KnightWhite:
        PlacePiece(new Knight(Color::White, x, y), x, y);
        break;
    case PieceChar::KnightBlack:
        PlacePiece(new Knight(Color::Black, x, y), x, y);
        break;
    case PieceChar::BishopWhite:
        PlacePiece(new Bishop(Color::White, x, y), x, y);
        break;
    case PieceChar::BishopBlack:
        PlacePiece(new Bishop(Color::Black, x, y), x, y);
        break;
    case PieceChar::RookWhite:
        PlacePiece(new Rook(Color::White, x, y), x, y);
        break;
    case PieceChar::RookBlack:
        PlacePiece(new Rook(Color::Black, x, y), x, y);
        break;
    case PieceChar::QueenWhite:
        PlacePiece(new Queen(Color::White, x, y), x, y);
        break;
    case PieceChar::QueenBlack:
        PlacePiece(new Queen(Color::Black, x, y), x, y);
        break;
    case PieceChar::KingWhite:
        PlacePiece(new King(Color::White, x, y), x, y);
        break;
    case PieceChar::KingBlack:
        PlacePiece(new King(Color::Black, x, y), x, y);
        break;
    case PieceChar::None:
    default:
       throw std::invalid_argument("Piecechar invalid " + (char) piece);
    }
}

Piece* Board_OO::RemovePiece(const int x, const int y) {
    Piece* piece = pieces[x][y];
    pieces[x][y] = nullptr;
    return piece;
}