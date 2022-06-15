#include "headers/board_importer.h"

void BoardImporter::ImportFEN(Board* board, std::string FEN) {
    board->ClearBoard();

	// import position
	for(int y = HEIGHT - 1; y >= 0; y--) {
		int remainingSquares = WIDTH;
		while(remainingSquares > 0) {
			char currentChar = FEN[0];
			if(isdigit(currentChar)) {
				remainingSquares -= (int) currentChar - 48;
			} else {
				board->PlacePiece(Utilities::GetSquare(WIDTH - remainingSquares, y), (PieceChar) currentChar);
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