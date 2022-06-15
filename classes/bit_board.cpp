#include "headers/bit_board.h"

BitBoard::BitBoard() : Board() {
    ClearBoard();
}

void BitBoard::ClearBoard() {
    for (int i = 0; i < PIECECOUNT; i++)
        pieceBB[i] = 0;
    for (int i = 0; i < COLORCOUNT; i++)
        colorBB[i] = 0;
    occupiedBB = 0;
    emptyBB = 0;
}

void BitBoard::Initialize() {
    ClearBoard();
    PlacePiece(Square::A1, PieceType::Rook, Color::White);
    PlacePiece(Square::A1, PieceType::Rook, Color::White);
    PlacePiece(Square::A1, PieceType::Rook, Color::White);
    PlacePiece(Square::A1, PieceType::Rook, Color::White);
}

void BitBoard::ImportFEN(std::string FEN) {
    ClearBoard();

	// import position
	for(int y = HEIGHT - 1; y >= 0; y--) {
		int remainingSquares = WIDTH;
		while(remainingSquares > 0) {
			char currentChar = FEN[0];
			if(isdigit(currentChar)) {
				remainingSquares -= (int) currentChar - 48;
			} else {
				PlacePiece(Utilities::GetSquare(WIDTH - remainingSquares, y), (PieceChar) currentChar);
				remainingSquares--;
			}
			FEN.erase(0, 1);
		}
		FEN.erase(0, 1);
	}

	if(FEN.length() == 0)
		return;
	// import turn
	//if(FEN[0] == 'w')
	//	setTurn(Color::White);
	//else
	//	setTurn(Color::Black);
	//FEN.erase(0, 2);

	// import king availibity

	// import en-passant

	// import half move

	// import full move
}

void BitBoard::PlacePiece(Square square, PieceType type, Color color) {
    U64 bit = (U64)1 << (U64) square;
    pieceBB[(int) type] |= bit;
    colorBB[(int) color] |= bit;
    occupiedBB |= bit;
}

void BitBoard::PlacePiece(Square square, PieceChar pieceChar) {
    PlacePiece(square, Utilities::GetPieceType(pieceChar), Utilities::GetPieceColor(pieceChar));
}

void BitBoard::MovePiece(const int startX, const int startY, const int endX, const int endY) {

}