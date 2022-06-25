#include "headers/board_importer.hh"

void BoardImporter::ImportFEN(BitBoard* board, std::string FEN) {
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
	if(FEN[0] == 'w')
		board->SetColor(Color::White);
	else
		board->SetColor(Color::Black);
		
	FEN.erase(0, 2);

	while(FEN[0] != ' ') {
		switch (FEN[0])
		{
		case 'K':
			board->EnableCastling(Color::White, Castling::King);
			break;
		case 'k':
			board->EnableCastling(Color::Black, Castling::King);
			break;
		case 'Q':
			board->EnableCastling(Color::White, Castling::Queen);
			break;
		case 'q':
			board->EnableCastling(Color::Black, Castling::Queen);
			break;
		}
		FEN.erase(0, 1);
	}

	// import king availibity

	// import en-passant

	// import half move

	// import full move
}
