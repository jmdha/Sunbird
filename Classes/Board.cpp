#include "Headers/Board.h"
#include "Pieces/Headers/Pawn.h"
#include "Pieces/Headers/Rook.h"
#include "Pieces/Headers/Knight.h"
#include "Pieces/Headers/Bishop.h"
#include "Pieces/Headers/King.h"
#include "Pieces/Headers/Queen.h"
#include <iostream>
#include "Headers/AI.h"

// used for minimax
#include <cstdlib>
#include <time.h>

// used in importFEN
#include <ctype.h>

// used for random moves
#include <chrono>

// used for import of PGN
#include "Headers/Utilities.h"

Board::Board() {
	SetStartPos();
}

// assumes correct FEN string
void Board::ImportFEN(std::string FEN) {
	ClearBoard();
	this->turnNumber = 1;

	// import position

	for(int y = HEIGHT - 1; y >= 0; y--) {
		int remainingSquares = WIDTH;
		while(remainingSquares > 0) {
			char currentChar = FEN[0];
			if(isdigit(currentChar)) {
				remainingSquares -= (int) currentChar - 48;
			} else {
				PlacePiece(GetPieceCharFromChar(currentChar), WIDTH - remainingSquares, y);
				remainingSquares--;
			}
			FEN.erase(0, 1);
		}
		FEN.erase(0, 1);
	}

	this->zobrist->IncrementCurrentHash();

	if(FEN.length() == 0)
		return;
	// import turn
	if(FEN[0] == 'w')
		SetTurn(Color::White);
	else
		SetTurn(Color::Black);
	FEN.erase(0, 2);

	// import king availibity
	// Disable castling
	for (int i = 0; i < 2; i++) {
		castlingValid[i][0] = false;
		castlingValid[i][1] = false;
	}
	// Enable the ones with flags
	int i = 0;
	while (i != FEN.length() && FEN[i] != ' ') {
		switch (FEN[i])
		{
		case 'K':
			castlingValid[(int) Color::White][(int) CastlingDirection::Right] = true;
			break;
		case 'Q':
			castlingValid[(int) Color::White][(int) CastlingDirection::Left] = true;
			break;
		case 'k':
			castlingValid[(int) Color::Black][(int) CastlingDirection::Right] = true;
			break;
		case 'q':
			castlingValid[(int) Color::Black][(int) CastlingDirection::Left] = true;
			break;
		}
	}
	FEN.erase(0, i);


	// import en-passant

	// import half move

	// import full move
}

std::string Board::GetFEN() {
	std::string FEN = "";

	for(int y = HEIGHT - 1; y >= 0; y--) {
		int blankCounter = 0;
		for(int x = 0; x < WIDTH; x++) {
			Piece* piece = GetPiece(x, y);
			if(piece == NULL)
				blankCounter++;
			else {
				if(blankCounter != 0)
					FEN.append(1, blankCounter + '0');
				FEN.append(1, (int) piece->GetPieceChar());
				blankCounter = 0;
			}
			if(x == 7 && piece == NULL && blankCounter != 0) {
				FEN.append(1, blankCounter + '0');
			}
		}
		if(y != 0)
			FEN.append(1, '/');
	}

	FEN.append(1, ' ');
	if(turn == Color::White)
		FEN.append(1, 'w');
	else
		FEN.append(1, 'b');

	return FEN;
}

void Board::ImportPGN(std::string moves, bool exportMovePerHash) {
	this->turnNumber = 1;
	this->zobrist->Clear();
	this->zobrist->IncrementCurrentHash();
	Move move;
	std::string moveString;
	enPassant = -1;
	bool inComment = false;
	bool nestedComment = false;
	for(int i = 0; i < (int) moves.length(); i++) {
		if(moves[i] == ' ') {
			if(moves[i + 1] == '{' || moves[i + 1] == '(') {
				if(inComment)
					nestedComment = true;
				else
					inComment = true;
			}

			if(moves[i - 1] == '}' || moves[i - 1] == ')') {
				if(nestedComment)
					nestedComment = false;
				else
					inComment = false;
			}

			// moves[i + 1] == '*' checks for non terminated games
			if(inComment || moves[i + 1] == '*')
				continue;

			if(moves[i + 1] != '.' && !IsNumber(moves[i + 1]) && (moves[i - 1] == '.' || ((IsNumber(moves[i - 1]) || moves[i - 1] == 'O' || moves[i - 1] == '+' || moves[i - 1] == '#' || moves[i - 1] == '}' || moves[i - 1] == 'R' || moves[i - 1] == 'N' || moves[i - 1] == 'B' || moves[i - 1] == 'Q')))) {

				// get move
				if(!IsNumber(moves[i + 2])


				   || (!IsLowercase(moves[i + 1]) && IsNumber(moves[i + 2]))) {
					if(moves[i + 1] != 'O') {
						// Normal move
						char tempPieceChar = moves[i + 1];
						if(IsLowercase(tempPieceChar))
							tempPieceChar = ((turn == Color::White) ? (int) PieceChar::PawnWhite : (int) PieceChar::PawnBlack);
						else if(turn == Color::Black)
							tempPieceChar += 32;
						PieceChar pieceChar = (PieceChar) tempPieceChar;
						if(moves[i + 4] != ' ' && moves[i + 4] != '+' && moves[i + 4] != '#' && moves[i + 4] != '?' && moves[i + 4] != '!' && moves.length() != i + 4) {
							if(moves[i + 5] == ' ' || moves[i + 5] == '+' || moves[i + 5] == '#' || moves[i + 5] == '?' || moves[i + 5] == '!') {
								if(moves[i + 2] == 'x') {
									// if pawn take
									if(IsLowercase(moves[i + 1]))
										move = GetValidMove(*this, Point(GetColumnAsNumber(moves[i + 3]), moves[i + 4] - 49), pieceChar, GetColumnAsNumber(moves[i + 1]));
									else
										move = GetValidMove(*this, Point(GetColumnAsNumber(moves[i + 3]), moves[i + 4] - 49), pieceChar);
								} else if(IsNumber(moves[i + 2])) {
									move = GetValidMove(*this, Point(GetColumnAsNumber(moves[i + 3]), moves[i + 4] - 49), GetRowAsNumber(moves[i + 2]), pieceChar);
								} else
									move = GetValidMove(*this, Point(GetColumnAsNumber(moves[i + 3]), moves[i + 4] - 49), pieceChar, GetColumnAsNumber(moves[i + 2]));
							} else if(IsNumber(moves[i + 2])) {
								move = GetValidMove(*this, Point(GetColumnAsNumber(moves[i + 4]), moves[i + 5] - 49), GetRowAsNumber(moves[i + 2]), pieceChar);
							} else {
								// if pawn-promotion-capture(... a little unwieldly)
								if(moves[i + 5] == '=')
									move = GetValidMove(*this, Point(GetColumnAsNumber(moves[i + 3]), moves[i + 4] - 49), pieceChar, GetColumnAsNumber(moves[i + 1]), GetPieceIndexFromChar(moves[i + 6]));
								else {
									// this case happens when two pieces of the same type, can capture the same piece
									if(moves[i + 3] == 'x')
										move = GetValidMove(*this, Point(GetColumnAsNumber(moves[i + 4]), moves[i + 5] - 49), pieceChar, GetColumnAsNumber(moves[i + 2]));
									else {
										if(moves[i + 4] == 'x')
											move = GetValidMove(*this, Point(GetColumnAsNumber(moves[i + 5]), moves[i + 6] - 49), GetRowAsNumber(moves[i + 3]), pieceChar, GetColumnAsNumber(moves[i + 2]));
										else
											move = GetValidMove(*this, Point(GetColumnAsNumber(moves[i + 4]), moves[i + 5] - 49), GetRowAsNumber(moves[i + 3]), pieceChar, GetColumnAsNumber(moves[i + 2]));
									}

								}

							}
						} else {
							if(moves[i + 5] == '=')
								move = GetValidMove(*this, Point(GetColumnAsNumber(moves[i + 3]), moves[i + 4] - 49), GetPieceIndexFromChar(moves[i + 6]), pieceChar);
							else
								move = GetValidMove(*this, Point(GetColumnAsNumber(moves[i + 2]), moves[i + 3] - 49), pieceChar);
						}

					}

					else {
						//castle
						int y = ((turn == Color::White) ? (int) BackRow::White : (int) BackRow::Black);
						int startX = 4;
						int endX;
						if(moves[i + 4] != '-')
							endX = 6;
						else
							endX = 2;
						move = Move(startX, y, endX, y);
						move.castling = true;
					}
				} else {
					PieceChar pieceChar = ((turn == Color::White) ? PieceChar::PawnWhite : PieceChar::PawnBlack);
					// pawn move (not take)
					if(moves[i + 3] == ' ' || moves[i + 3] == '+' || moves[i + 3] == '#' || moves[i + 3] == '?' || moves[i + 3] == '!' || moves.length() <= i + 3) {
						int col = GetColumnAsNumber(moves[i + 1]);
						move = GetValidMove(*this, Point(col, moves[i + 2] - 49), pieceChar, col);
					}

					// same but with promotion
					else
						move = GetValidMove(*this, Point(GetColumnAsNumber(moves[i + 1]), moves[i + 2] - 49), GetPieceIndexFromChar(moves[i + 4]), pieceChar);
				}

				if(exportMovePerHash) {
					// get move as string
					moveString = "";
					int i2 = 1;
					while(moves[i + i2] != ' ' && moves[i + i2] != '+' && moves[i + i2] != '?' && moves[i + i2] != '!') {
						moveString += moves[i + i2];
						i2++;
					};
					std::cout << this->zobrist->getHash() << ' ' << moveString.c_str() << std::endl;
				}

				// commit move
				CommitMove(&move);
				if(move.pawnDoubleMove)
					enPassant = move.startX;
				else
					enPassant = -1;
				//printBoard();
				if(IsNumber(moves[i + 1]) && (moves[i + 2] == '/' || moves[i + 2] == '-'))
					break;
			} else if(IsNumber(moves[i + 1])) {
				// if the following is a turn number
				int t = i;
				std::string turnString = "";
				while(IsNumber(moves[t += 1])) {
					turnString += moves[t];
				}
				int turnStringAsNumber = atoi(turnString.c_str());
				if(turnStringAsNumber != turnNumber && turnStringAsNumber != turnNumber + 1)
					break;
			}
		}

	}
}

void Board::ImportMoveSequence(std::string moves) {
	this->turnNumber = 1;
	this->zobrist->Clear();
	this->zobrist->IncrementCurrentHash();
	std::string move = "";
	for(int i = 0; i < (int) moves.length(); i++) {
		if(moves[i] != ' ') {
			move.push_back(moves[i]);
		}
		if(moves[i] == ' ' || i == (int) moves.length() - 1) {
			Move* newMove = new Move(move);
			if(GetPiece(newMove->startX, newMove->startY)->GetIndex() == (int) PieceIndex::King)
				if(move == "e1g1" || move == "e1c1" || move == "e8g8" || move == "e8c8")
					newMove->castling = true;

			// check pawn "stuff"
			if(GetPiece(newMove->startX, newMove->startY)->GetIndex() == (int) PieceIndex::Pawn) {
				// if promotion
				if(newMove->endY == (int) BackRow::White || newMove->endY == (int) BackRow::Black) {
					// if promotion to something else
					PieceIndex type = PieceIndex::Queen;
					if(move.length() == 5) {
						switch(move[4]) {
							case 'r':
							type = PieceIndex::Rook;
							break;
							case 'n':
							type = PieceIndex::Knight;
							break;
							case 'b':
							type = PieceIndex::Bishop;
							break;
						}
					}
					newMove->promotionType = type;
					newMove->promotion = true;
				}

				// if double move
				else if(abs(newMove->endY - newMove->startY) == 2)
					newMove->pawnDoubleMove = true;
				// en passant take
				else if(newMove->startX != newMove->endX && IsSquareEmpty(newMove->endX, newMove->endY))
					newMove->enPassantTake = true;
			}


			CommitMove(newMove);
			if(newMove->pawnDoubleMove)
				enPassant = newMove->startX;
			else
				enPassant = -1;
			move = "";
		}
	}
}

void Board::PrintBoard() {
	for(int y = HEIGHT - 1; y >= 0; y--) {
		for(int x = 0; x < WIDTH; x++) {
			if(IsSquareEmpty(x, y))
				printf(".");
			else
				printf("%c", GetPiece(x, y)->GetPieceChar());
		}
		printf("\n");
	}
}

bool Board::IsSquareEmpty(int x, int y) {
	return GetPiece(x, y) == NULL;
}

bool Board::IsSquareEnemy(Color color, int x, int y) {
	if(IsSquareEmpty(x, y))
		return false;
	return GetPiece(x, y)->color == ((color == Color::White) ?Color::Black : Color::White);
}

void Board::SetStartPos() {
	ClearBoard();
	for(int x = 0; x < WIDTH; x++) {
		PlacePiece(PieceChar::PawnBlack, x, (int) PawnRow::Black);
		PlacePiece(PieceChar::PawnWhite, x, (int) PawnRow::White);
	}
	PlacePiece(PieceChar::RookWhite, 0, (int) BackRow::White);
	PlacePiece(PieceChar::RookWhite, 7, (int) BackRow::White);
	PlacePiece(PieceChar::RookBlack, 0, (int) BackRow::Black);
	PlacePiece(PieceChar::RookBlack, 7, (int) BackRow::Black);
	PlacePiece(PieceChar::KnightWhite, 1, (int) BackRow::White);
	PlacePiece(PieceChar::KnightWhite, 6, (int) BackRow::White);
	PlacePiece(PieceChar::KnightBlack, 1, (int) BackRow::Black);
	PlacePiece(PieceChar::KnightBlack, 6, (int) BackRow::Black);
	PlacePiece(PieceChar::BishopWhite, 2, (int) BackRow::White);
	PlacePiece(PieceChar::BishopWhite, 5, (int) BackRow::White);
	PlacePiece(PieceChar::BishopBlack, 2, (int) BackRow::Black);
	PlacePiece(PieceChar::BishopBlack, 5, (int) BackRow::Black);
	PlacePiece(PieceChar::KingWhite, 4, (int) BackRow::White);
	PlacePiece(PieceChar::KingBlack, 4, (int) BackRow::Black);
	PlacePiece(PieceChar::QueenWhite, 3, (int) BackRow::White);
	PlacePiece(PieceChar::QueenBlack, 3, (int) BackRow::Black);
	zobrist->IncrementCurrentHash();
}

void Board::ClearBoard() {
	for(int y = 0; y < HEIGHT; y++)
		for(int x = 0; x < WIDTH; x++)
			this->board[x][y] = NULL;
	this->zobrist = new Zobrist(this);
	pieceCount[0] = 0, pieceCount[1] = 0, pieceCount[2] = 0;
	for(int x = 0; x < WIDTH; x++)
		pawnsOnFile[x] = 0;
}

void Board::SetTurn(Color turn) {
	this->turn = turn;
}

void Board::SwitchTurn() {
	if(turn == Color::White)
		turn =Color::Black;
	else {
		turn = Color::White;
		this->turnNumber++;
	}
}

void Board::PlacePiece(Piece* piece, int x, int y) {
	board[x][y] = piece;
	piece->x = x;
	piece->y = y;
	this->zobrist->flipSquare(x, y, piece->GetIndex(), (int) piece->color);
	pieceCount[0]++;
	pieceCount[1 + (int) piece->color]++;
	if(piece->GetIndex() == (int) PieceIndex::Pawn)
		pawnsOnFile[x]++;
	else if(piece->GetIndex() == (int) PieceIndex::King)
		kingPos[(int) piece->color] = Point(piece->x, piece->y);
}

void Board::PlacePiece(PieceChar piece, int x, int y) {
	PlacePiece(GetPieceFromChar(piece), x, y);
}

void Board::RemovePiece(int x, int y) {
	Piece* piece = this->GetPiece(x, y);

	if(piece != NULL) {
		int index = piece->GetIndex();
		int color = (int) piece->color;
		this->zobrist->flipSquare(x, y, index, color);
		pieceCount[0]--;
		pieceCount[1 + color]--;
		if(piece->GetIndex() == (int) PieceIndex::Pawn)
			pawnsOnFile[x]--;
	}

	board[x][y] = NULL;
}

Piece* Board::GetPiece(int x, int y) {
	if(x < 0 || x > 7 || y < 0 || y > 7)
		return nullptr;
	return board[x][y];
}

Piece* Board::GetPieceFromChar(PieceChar piece) {
	switch(piece) {
		case PieceChar::PawnWhite:
		return new Pawn(Color::White);

		case PieceChar::PawnBlack:
		return new Pawn(Color::Black);

		case PieceChar::RookWhite:
		return new Rook(Color::White);

		case PieceChar::RookBlack:
		return new Rook(Color::Black);

		case PieceChar::KnightWhite:
		return new Knight(Color::White);

		case PieceChar::KnightBlack:
		return new Knight(Color::Black);

		case PieceChar::BishopWhite:
		return new Bishop(Color::White);

		case PieceChar::BishopBlack:
		return new Bishop(Color::Black);

		case PieceChar::QueenWhite:
		return new Queen(Color::White);

		case PieceChar::QueenBlack:
		return new Queen(Color::Black);

		case PieceChar::KingWhite:
		return new King(Color::White);

		case PieceChar::KingBlack:
		return new King(Color::Black);

		default:
		throw std::invalid_argument("Invalid direction " + (int) piece);
	}
}

PieceChar Board::GetPieceCharFromChar(char piece) {
	switch(piece) {
		case 'P':
		return PieceChar::PawnWhite;

		case 'p':
		return PieceChar::PawnBlack;

		case 'R':
		return PieceChar::RookWhite;

		case 'r':
		return PieceChar::RookBlack;

		case 'N':
		return PieceChar::KnightWhite;

		case 'n':
		return PieceChar::KnightBlack;

		case 'B':
		return PieceChar::BishopWhite;

		case 'b':
		return PieceChar::BishopBlack;

		case 'Q':
		return PieceChar::QueenWhite;

		case 'q':
		return PieceChar::QueenBlack;

		case 'K':
		return PieceChar::KingWhite;

		case 'k':
		return PieceChar::KingBlack;

		default:
		return PieceChar::Blank;
	}
}

PieceIndex Board::GetPieceIndexFromChar(char piece) {
	switch(piece) {
		case 'P':
		case 'p':
		return PieceIndex::Pawn;

		case 'R':
		case 'r':
		return PieceIndex::Rook;

		case 'N':
		case 'n':
		return PieceIndex::Knight;

		case 'B':
		case 'b':
		return PieceIndex::Bishop;

		case 'Q':
		case 'q':
		return PieceIndex::Queen;

		case 'K':
		case 'k':
		return PieceIndex::King;

		default:
		return PieceIndex::Blank;
	}
}

int Board::GetPieceCount() {
	int count = 0;
	for(int x = 0; x < WIDTH; x++) {
		for(int y = 0; y < HEIGHT; y++) {
			if(!IsSquareEmpty(x, y))
				count++;
		}
	}
	return count;
}


void Board::CommitMove(Move* move) {
	DoMove(move);
	SwitchTurn();
}

// assumed valid move
void Board::DoMove(Move* move) {
	move->SetTarget(GetPiece(move->endX, move->endY));
	// Used for hashing
	if(move->target != NULL) {
		RemovePiece(move->endX, move->endY);
		PieceChar capturedPiece = move->target->GetPieceChar();

		if(capturedPiece == PieceChar::KingWhite)
			kingAlive[(int) Color::White] = false;
		else if(capturedPiece == PieceChar::KingBlack)
			kingAlive[(int) Color::Black] = false;
	}
	Piece* piece = GetPiece(move->startX, move->startY);
	if(piece->GetIndex() == (int) PieceIndex::King) {
		int side = (int) piece->color;

		for(int i = 0; i < 2; i++) {
			if(castlingValid[side][i]) {
				move->disallowedCastling[i] = true;
				castlingValid[side][i] = false;
			}
		}

		if(move->castling) {
			int rookStartX;
			int rookEndX;
			if(move->endX == 2) {
				rookStartX = 0;
				rookEndX = 3;
			} else {
				rookStartX = 7;
				rookEndX = 5;
			}

			if(GetPiece(rookStartX, piece->y) == NULL)
				move->castling = false;
			else
				DoMove(new Move(rookStartX, piece->y, rookEndX, piece->y));


		}
	} else if(piece->GetIndex() == (int) PieceIndex::Rook) {
		int side = (int) piece->color;
		if(piece->x == 0 && castlingValid[side][0]) {
			move->disallowedCastling[0] = true;
			castlingValid[side][0] = false;
		}

		else if(piece->x == 7 && castlingValid[side][1]) {
			move->disallowedCastling[1] = true;
			castlingValid[side][1] = false;
		}
	} else if(piece->GetIndex() == (int) PieceIndex::Pawn) {
		if(move->promotion) {
			switch(move->promotionType) {
				case PieceIndex::Rook:
				piece = new Rook(piece->color);
				break;
				case PieceIndex::Knight:
				piece = new Knight(piece->color);
				break;
				case PieceIndex::Bishop:
				piece = new Bishop(piece->color);
				break;
				case PieceIndex::Queen:
				default:
				piece = new Queen(piece->color);
				break;
			}
			piece->x = move->endX;
			piece->y = move->endY;
		} else if(move->enPassantTake) {
			int passedPawnYPosition = ((piece->color == Color::White) ? 4 : 3);
			move->SetTarget(GetPiece(move->endX, passedPawnYPosition));
			RemovePiece(move->endX, passedPawnYPosition);
		}

		move->priorEnPassant = enPassant;

		if(move->pawnDoubleMove)
			enPassant = move->startX;
		else
			enPassant = -1;
	}

	if(!piece->hasMoved) {
		piece->hasMoved = true;
		move->firstMove = true;
	}

	RemovePiece(move->startX, move->startY);
	PlacePiece(piece, move->endX, move->endY);
	this->zobrist->IncrementCurrentHash();
	turnCount++;
}

// assumes valid move
void Board::UndoMove(Move* move) {
	this->zobrist->DecrementCurrentHash();
	turnCount--;
	Piece* piece = GetPiece(move->endX, move->endY);

	int side = (int) piece->color;
	for(int i = 0; i < 2; i++)
		if(move->disallowedCastling[i])
			castlingValid[side][i] = true;

	if(move->firstMove)
		piece->hasMoved = false;

	if(move->promotion) {
		piece = new Pawn(piece->color);
		piece->x = move->startX;
		piece->y = move->startX;
	}
	RemovePiece(move->endX, move->endY);
	PlacePiece(piece, move->startX, move->startY);
	if(move->target != NULL) {
		PlacePiece(move->target, move->target->x, move->target->y);
		move->SetTarget(NULL);
		for(int i = 0; i < 2; i++)
			kingAlive[i] = true;
	} else if(move->castling) {
		int rookStartX;
		int rookEndX;
		if(move->endX == 2) {
			rookStartX = 0;
			rookEndX = 3;
		} else {
			rookStartX = 7;
			rookEndX = 5;
		}

		UndoMove(new Move(rookStartX, piece->y, rookEndX, piece->y));
	}
	enPassant = move->priorEnPassant;
}

bool Board::IsKingVulnerable(Color side) {
	// See if enemy can attack
	for(int y = 0; y < HEIGHT; y++) {
		for(int x = 0; x < WIDTH; x++) {
			if(IsSquareEnemy(side, x, y)) {
				if(GetPiece(x, y)->CheckIfPosPossible(*(this), kingPos[(int) side]))
					return true;
			}
		}
	}

	return false;
}


int Board::EvaluateBoard() {
	return EvaluateBoard(this->turn);
}

int Board::EvaluateBoard(Color side) {
	int value = 0;

	for(int y = 0; y < HEIGHT; y++) {
		for(int x = 0; x < WIDTH; x++) {
			if(!IsSquareEmpty(x, y)) {
				Piece* piece = GetPiece(x, y);
				int multiplier = ((piece->color == side) ? 1 : -1);
				value += piece->GetValue() * multiplier;
			}
		}
	}

	return value;
}