#include <iostream>

#include "../classes/headers/board.hh"
#include "../classes/headers/board_importer.hh"
#include "../classes/headers/minimax.hh"
#include "../classes/headers/move_gen.hh"
#include "../classes/headers/perft.hh"

int main(int argc, char* argv[]) {
    std::string FEN = "r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1";
    Board board = Board();
    BoardImporter::ImportFEN(&board, FEN);    

    Board priorBoard = board;

    char* sMove = "O-O";

    Move move = Move(MoveType::Quiet);
    if (sMove == "O-O-O" || sMove == "o-o-o") 
        move = Move(MoveType::QueenCastle);
    else if (sMove == "O-O" || sMove == "o-o") 
        move = Move(MoveType::KingCastle);
    else {
        Square fromSquare = Utilities::GetSquare(sMove[0], sMove[1]);
        Square toSquare = Utilities::GetSquare(sMove[2], sMove[3]);

        PieceType toPiece = board.GetType(toSquare);
        MoveType moveType = (toPiece == PieceType::None) ? MoveType::Quiet : MoveType::Capture;

        move = Move(moveType, fromSquare, toSquare);
    }

    PieceType capturedPiece = board.DoMove(move);
    board.UndoMove(move, capturedPiece);

    if (priorBoard == board)
        exit(EXIT_SUCCESS);
    else
        throw std::logic_error("Unexpected changes from do-undo move");
}
