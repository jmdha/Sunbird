#include "headers/board_importer.hh"

void BoardImporter::ImportFEN(Board* board, std::string FEN) {
    (*board) = Board();

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
	if(FEN[0] == 'w') {
		board->color = Color::White;
		board->originalColor = Color::White;
	} else {
		board->color = Color::Black;
		board->originalColor = Color::Black;
	}
		
	FEN.erase(0, 2);

	while(FEN[0] != ' ') {
		switch (FEN[0])
		{
		case 'K':
			board->castlingAllowed[(int) Color::White][(int) Castling::King] = true;
			break;
		case 'k':
			board->castlingAllowed[(int) Color::Black][(int) Castling::King] = true;
			break;
		case 'Q':
			board->castlingAllowed[(int) Color::White][(int) Castling::Queen] = true;
			break;
		case 'q':
			board->castlingAllowed[(int) Color::Black][(int) Castling::Queen] = true;
			break;
		}
		FEN.erase(0, 1);
	}

	// import king availibity

	// import en-passant

	// import half move

	// import full move
}

void BoardImporter::ImportMoveSequence(Board* board, std::string moves) {
	board->Initialize();
	std::string move = "";
	for (int i = 0; i < moves.length(); i++) {
		if (moves[i] != ' ')
			move.push_back(moves[i]);

		if (moves[i] == ' ' || i == (int) moves.length() - 1) {
			Square fromSquare = Utilities::GetSquare(move[0], move[1]);
			Square toSquare = Utilities::GetSquare(move[2], move[3]);
			PieceType fromType = board->GetType(fromSquare);
			PieceType toType = board->GetType(toSquare);
			Color fromColor = board->GetColor(fromSquare);
			Color toColor = board->GetColor(toSquare);
			MoveType type;
			// Is castling?
			//// Is kingside castle
			if (fromType == PieceType::King && (move == "e1g1" || move == "e8g8"))
				type = MoveType::KingCastle;
			//// Is queenside castle
			else if (fromType == PieceType::King && (move == "e1c1" || move == "e8c8"))
				type = MoveType::QueenCastle;
			
			else if (fromType == PieceType::Pawn) {
				// If double pawn push
				if (std::abs((int) Utilities::GetRowIndex(fromSquare) - (int) Utilities::GetRowIndex(toSquare)) == 2)
					type = MoveType::DoublePawnPush;
				// If promotion
				else if (Utilities::GetRow(toSquare) == ((fromColor == Color::White) ? Row::Row8 : Row::Row1)) {
					// If none capture promotion
					if (toType == PieceType::None) {
						if (moves.length() == 5) {
							if (move[4] == 'r')
								type = MoveType::RPromotion;
							else if (move[4] == 'n')
								type = MoveType::NPromotion;
							else if (move[4] == 'b')
								type = MoveType::BPromotion;
						} else
							type = MoveType::QPromotion;
					} else {
						// Capture promotion
						if (moves.length() == 5) {
							if (move[4] == 'r')
								type = MoveType::RPromotionCapture;
							else if (move[4] == 'n')
								type = MoveType::NPromotionCapture;
							else if (move[4] == 'b')
								type = MoveType::BPromotionCapture;
						} else
							type = MoveType::QPromotionCapture;
					}
				} else if (toType != PieceType::None)
					type = MoveType::Capture; 
				else
					type = MoveType::Quiet;
			} else if (toType != PieceType::None)
				type = MoveType::Capture; 
			else
				type = MoveType::Quiet;
			Move tempMove = Move(type, fromSquare, toSquare);
			board->DoMove(&tempMove);
			move = "";
		}
	}
}
