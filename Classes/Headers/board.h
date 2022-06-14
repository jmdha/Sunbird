#ifndef Board_H
#define Board_H

class Zobrist;
class Piece;
class Bishop;
class King;
class Knight;
class Pawn;
class Queen;
class Rook;
class Move;

#include <string>
#include <vector>
#include <list>
#include "piece.h"
#include "constants.h"
#include "move.h"
#include "zobrist.h"
#include "point.h"

// used for random taking move
#include <algorithm>
#include <random>



class Board {
public:
	int turnNumber = 0;
	bool kingAlive[2] = {true, true};
	Point kingPos[2] = {Point(0, 0), Point(0,0)};
	bool castlingValid[2][2] = {{true, true}, {true, true}};
	// First number is total, second is white, then black
	int pieceCount[3] = {32, 16, 16};
	int pawnsOnFile[8] = {2};
	Zobrist* zobrist;
	int enPassant;
	Color turn = Color::White;
	Color startTurn;
	int turnCount = 0;

	Board();

	void SetStartPos();
	void ClearBoard();
	void ImportFEN(std::string FEN);
	void ImportPGN(std::string moves, bool exportMovePerHash);
	void ImportMoveSequence(std::string moves);
	void PrintBoard();
	bool IsSquareEmpty(int x, int y);
	bool IsSquareEnemy(Color color, int x, int y);
	Piece* GetPiece(int x, int y);
	int EvaluateBoard();
	int EvaluateBoard(Color side);
	int GetPieceCount();
	void CommitMove(Move* move);
	void DoMove(Move* move);
	void SwitchTurn();
	std::string GetFEN();
	void UndoMove(Move* move);
	bool IsKingVulnerable(Color side);

private:
	Piece* board[WIDTH][HEIGHT];
	



	void SetTurn(Color turn);

	void PlacePiece(Piece* piece, int x, int y);
	void PlacePiece(PieceChar piece, int x, int y);
	void RemovePiece(int x, int y);



	Piece* GetPieceFromChar(PieceChar piece);
	PieceChar GetPieceCharFromChar(char piece);
	PieceIndex GetPieceIndexFromChar(char piece);

};

#endif