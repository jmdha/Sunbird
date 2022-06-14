// Contains base stuff an ai needs e.g generating possible moves and such
#include "Headers/AI.h"
#include <iostream>
#include <algorithm>
#include <random>


Move GetValidMove(Board board, Point endPos, PieceIndex promotionType, PieceChar pieceChar) {
	std::vector<Move> moves = GetAllMovesOfPieceChar(board, pieceChar);
	for(int i = 0; i < (int) moves.size(); i++) {
		Piece* piece = board.GetPiece(moves[i].startX, moves[i].startY);
		if(piece->color == board.turn && moves[i].startX == endPos.x && moves[i].endX == endPos.x && moves[i].endY == endPos.y && moves[i].promotionType == promotionType) {
			bool valid = false;
			board.DoMove(&moves[i]);
			if(!board.IsKingVulnerable(piece->color))
				valid = true;
			board.UndoMove(&moves[i]);
			if(valid)
				return moves[i];
		}
	}
	throw std::invalid_argument("Found no move");
}

Move GetValidMove(Board board, Point endPos, PieceChar pieceChar) {
	Move move;
	int amountOfMatchingPieces = 0;
	Piece* matchingPieces[8] = {nullptr};
	for(int y = 0; y < HEIGHT; y++) {
		for(int x = 0; x < WIDTH; x++) {
			if(!board.IsSquareEmpty(x, y) && board.GetPiece(x, y)->GetPieceChar() == pieceChar) {
				matchingPieces[amountOfMatchingPieces] = board.GetPiece(x, y);
				amountOfMatchingPieces++;
			}
		}
	}

	// if multiple pieces can move to end square, take the one that hasn't moved
	for(int i = 0; i < amountOfMatchingPieces; i++) {
		if(!matchingPieces[i]->hasMoved) {
			move = matchingPieces[i]->GetMoveIfPossible(board, endPos);
			if(move.endX != -1 && move.endY != -1) {
				bool valid = false;
				board.DoMove(&move);
				if(!board.IsKingVulnerable(matchingPieces[i]->color))
					valid = true;
				board.UndoMove(&move);
				if(valid)
					return move;
			}
		}
	}
	for(int i = 0; i < amountOfMatchingPieces; i++) {
		if(matchingPieces[i]->hasMoved) {
			move = matchingPieces[i]->GetMoveIfPossible(board, endPos);
			if(move.endX != -1 && move.endY != -1) {
				bool valid = false;
				board.DoMove(&move);
				if(!board.IsKingVulnerable(matchingPieces[i]->color))
					valid = true;
				board.UndoMove(&move);
				if(valid)
					return move;
			}
		}
	}


	throw std::invalid_argument("Found no move");
}

Move GetValidMove(Board board, Point endPos, PieceChar pieceChar, int column) {
	Move move;

	for(int y = 0; y < HEIGHT; y++) {
		for(int x = 0; x < WIDTH; x++) {
			if(!board.IsSquareEmpty(x, y)) {
				Piece* piece = board.GetPiece(x, y);
				if(piece->GetPieceChar() == pieceChar && x == column) {
					move = piece->GetMoveIfPossible(board, endPos);
					if(move.endX != -1 && move.endY != -1) {
						bool valid = false;
						board.DoMove(&move);
						if(!board.IsKingVulnerable(piece->color))
							valid = true;
						board.UndoMove(&move);
						if(valid)
							return move;
					}
				}
			}
		}
	}

	throw std::invalid_argument("Found no move");
}

Move GetValidMove(Board board, Point endPos, PieceChar pieceChar, int column, PieceIndex promotionType) {
	std::vector<Move> moves;

	for(int y = 0; y < HEIGHT; y++) {
		for(int x = 0; x < WIDTH; x++) {
			if(!board.IsSquareEmpty(x, y)) {
				Piece* piece = board.GetPiece(x, y);
				if(piece->GetPieceChar() == pieceChar && x == column) {
					moves = piece->GetPossibleMoves(board);
					for(int i = 0; i < (int) moves.size(); i++) {
						if(moves[i].endX == endPos.x && moves[i].endY == endPos.y && moves[i].promotionType == promotionType) {
							bool valid = false;
							board.DoMove(&moves[i]);
							if(!board.IsKingVulnerable(piece->color))
								valid = true;
							board.UndoMove(&moves[i]);
							if(valid)
								return moves[i];
						}
					}
				}
			}
		}
	}

	throw std::invalid_argument("Found no move");
}

Move GetValidMove(Board board, Point endPos, int row, PieceChar pieceChar) {
	Move move;

	for(int y = 0; y < HEIGHT; y++) {
		for(int x = 0; x < WIDTH; x++) {
			if(!board.IsSquareEmpty(x, y)) {
				Piece* piece = board.GetPiece(x, y);
				if(piece->GetPieceChar() == pieceChar && y == row) {
					move = piece->GetMoveIfPossible(board, endPos);
					if(move.endX != -1 && move.endY != -1) {
						bool valid = false;
						board.DoMove(&move);
						if(!board.IsKingVulnerable(piece->color))
							valid = true;
						board.UndoMove(&move);
						if(valid)
							return move;
					}
				}
			}
		}
	}

	throw std::invalid_argument("Found no move");
}

Move GetValidMove(Board board, Point endPos, int row, PieceChar pieceChar, int column) {
	Move move;

	for(int y = 0; y < HEIGHT; y++) {
		for(int x = 0; x < WIDTH; x++) {
			if(!board.IsSquareEmpty(x, y)) {
				Piece* piece = board.GetPiece(x, y);
				if(piece->GetPieceChar() == pieceChar && y == row && x == column) {
					move = piece->GetMoveIfPossible(board, endPos);
					if(move.endX != -1 && move.endY != -1) {
						bool valid = false;
						board.DoMove(&move);
						if(!board.IsKingVulnerable(piece->color))
							valid = true;
						board.UndoMove(&move);
						if(valid)
							return move;
					}
				}
			}
		}
	}
	throw std::invalid_argument("Found no move");
}

std::vector<Move> GetAllMoves(Board board, Color side) {

	std::vector<Move> moves;

	std::vector<Move>::iterator it;
	it = moves.begin();
	std::vector<Move> tempMoveList;

	for(int y = 0; y < HEIGHT; y++) {
		for(int x = 0; x < WIDTH; x++) {
			if(!board.IsSquareEmpty(x, y)) {
				Piece* piece = board.GetPiece(x, y);
				if(piece->color == side) {
					tempMoveList = piece->GetPossibleMoves(board);
					// Go through moves
					for(int i = 0; i < tempMoveList.size(); i++) {
						board.DoMove(&tempMoveList[i]);
						if(!board.IsKingVulnerable(side))
							moves.push_back(tempMoveList[i]);
						board.UndoMove(&tempMoveList[i]);
					}
				}
			}
		}
	}

	return moves;
}

std::vector<Move> GetAllMovesOfPieceChar(Board board, PieceChar pieceChar) {

	std::vector<Move> moves;

	std::vector<Move>::iterator it;
	it = moves.begin();
	std::vector<Move> tempMoveList;

	for(int y = 0; y < HEIGHT; y++) {
		for(int x = 0; x < WIDTH; x++) {
			if(!board.IsSquareEmpty(x, y) && board.GetPiece(x, y)->GetPieceChar() == pieceChar) {
				tempMoveList = board.GetPiece(x, y)->GetPossibleMoves(board);
				moves.insert(it, tempMoveList.begin(), tempMoveList.end());
				it = moves.begin();
			}
		}
	}

	return moves;
}

std::vector<Move> GetAllMovesOfPieceChar(Board board, PieceChar pieceChar, int startCol) {

	std::vector<Move> moves;

	std::vector<Move>::iterator it;
	it = moves.begin();
	std::vector<Move> tempMoveList;

	for(int y = 0; y < HEIGHT; y++) {
		for(int x = 0; x < WIDTH; x++) {
			if(!board.IsSquareEmpty(x, y) && board.GetPiece(x, y)->GetPieceChar() == pieceChar && x == startCol) {
				tempMoveList = board.GetPiece(x, y)->GetPossibleMoves(board);
				moves.insert(it, tempMoveList.begin(), tempMoveList.end());
				it = moves.begin();
			}
		}
	}

	return moves;
}

std::vector<Move> GetAllMovesOfPieceChar(Board board, int startRow, PieceChar pieceChar) {

	std::vector<Move> moves;

	std::vector<Move>::iterator it;
	it = moves.begin();
	std::vector<Move> tempMoveList;

	for(int y = 0; y < HEIGHT; y++) {
		for(int x = 0; x < WIDTH; x++) {
			if(!board.IsSquareEmpty(x, y) && board.GetPiece(x, y)->GetPieceChar() == pieceChar && y == startRow) {
				tempMoveList = board.GetPiece(x, y)->GetPossibleMoves(board);
				moves.insert(it, tempMoveList.begin(), tempMoveList.end());
				it = moves.begin();
			}
		}
	}

	return moves;
}

std::vector<Move> GetAllMovesOfPieceChar(Board board, PieceChar pieceChar, int startCol, int startRow) {

	std::vector<Move> moves;

	std::vector<Move>::iterator it;
	it = moves.begin();
	std::vector<Move> tempMoveList;

	for(int y = 0; y < HEIGHT; y++) {
		for(int x = 0; x < WIDTH; x++) {
			if(!board.IsSquareEmpty(x, y) && board.GetPiece(x, y)->GetPieceChar() == pieceChar && x == startCol && y == startRow) {
				tempMoveList = board.GetPiece(x, y)->GetPossibleMoves(board);
				moves.insert(it, tempMoveList.begin(), tempMoveList.end());
				it = moves.begin();
			}
		}
	}

	return moves;
}

Move GetBestMove(Board* board, int maxTime) {
	int totalMovesChecked = 0;
	board->startTurn = board->turn;

	auto startTime = std::chrono::high_resolution_clock::now();
	long long accTime = 0;

	Move move;

	for (int i = 1; i <= MAXDEPTH; i++) {
		startTime = std::chrono::high_resolution_clock::now();
		move = Minimax(board, i, true, board->turn, -((int)Value::Infinite), (int)Value::Infinite, false, &totalMovesChecked, 0);

		auto currentTime = std::chrono::high_resolution_clock::now();
		auto timeSpent = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime).count();
		accTime += timeSpent;
		auto estimatedTimeForNextMove = timeSpent * 40;
		auto threshold = maxTime / 20;
		if (maxTime >= 0 && accTime + estimatedTimeForNextMove > threshold)
			break;
	}

	return move;

}

Move GenerateMove(Board* board, int depth, int& totalMoves)
{
	return Minimax(board, depth, true, board->turn, -((int)Value::Infinite), (int)Value::Infinite, false, &totalMoves, 0);
}

std::random_device rd;
std::mt19937 g(rd());

Move Minimax(Board* board, int depth, bool isMax, Color currentTurn, int a, int b, bool doingHE, int* totalMoves, int accDepth) {
	(*totalMoves) += 1;
	Move bestMove;
	Color oppositeColor = ((currentTurn == Color::White) ? Color::Black : Color::White);

	// check if the position has occured more than 2 times
	// this is to check for draw
	if(board->zobrist->GetCurrentInstanceCount() > 2) {
		int multiplier = ((currentTurn == board->turn) ? 1 : -1);
		bestMove.value = (int) Value::Draw * multiplier;
		bestMove.moveDepth = accDepth;
		return bestMove;
	}

	if(isMax)
		bestMove.value = -((int) Value::Infinite);
	else
		bestMove.value = (int) Value::Infinite;
	if(depth == 0 || !board->kingAlive[(int) currentTurn] || !board->kingAlive[(int) oppositeColor]) {
		bestMove.value = board->EvaluateBoard(board->turn);
		bestMove.moveDepth = accDepth;
		return bestMove;
	}

	std::vector<Move> moves = GetAllMoves(*board, currentTurn);

	for(int i = 0; i < (int) moves.size(); i++)
		moves[i].moveSizeBatch = (int) moves.size();
		
	//std::shuffle(moves.begin(), moves.end(), g);

	if((int) moves.size() == 0) {
		if(board->IsKingVulnerable(currentTurn))
			if(board->turn != currentTurn)
				bestMove.value = (int) Value::Mate;
			else
				bestMove.value = -((int) Value::Mate);
		else
			bestMove.value = (int) Value::Draw;
		bestMove.moveDepth = accDepth;
		return bestMove;
	}
	for(int i = 0; i < (int) moves.size(); i++) {
		Move move;

		board->DoMove(&(moves[i]));

		// If the position has not already been evaluated
		if (!board->zobrist->GetValue(depth, move.value)) {
			// Go deeper
			if (depth == 1 && !doingHE &&
				(moves[i].target != NULL ||
					board->IsKingVulnerable(currentTurn) ||
					board->IsKingVulnerable(oppositeColor) ||
					board->pieceCount[0] < 8))
				move = Minimax(board, HEDEPTH, !isMax, oppositeColor, a, b, true, totalMoves, accDepth + 1);
			else {
				if (doingHE && moves[i].target == NULL)
					move = Minimax(board, 0, !isMax, oppositeColor, a, b, doingHE, totalMoves, accDepth + 1);
				else
					move = Minimax(board, depth - 1, !isMax, oppositeColor, a, b, doingHE, totalMoves, accDepth + 1);
			}

			 board->zobrist->StoreEval(depth, move.value);
		}

		board->UndoMove(&(moves[i]));


		if(isMax && move.value > bestMove.value) {
			bestMove = moves[i];
			bestMove.value = move.value;
			bestMove.moveDepth = move.moveDepth;
		} else if(!isMax && move.value < bestMove.value) {
			bestMove = moves[i];
			bestMove.value = move.value;
			bestMove.moveDepth = move.moveDepth;
		} else if(move.value == bestMove.value && move.moveDepth < bestMove.moveDepth) {
			bestMove = moves[i];
			bestMove.value = move.value;
			bestMove.moveDepth = move.moveDepth;
		}

		
		if(isMax) {
			a = std::max(move.value, a);
			if(a >= b)
				break;
		} else {
			b = std::min(move.value, b);
			if(b <= a)
				break;
		}
		
		




	}
	bestMove.totalMoves = *totalMoves;
	return bestMove;
}
