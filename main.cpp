#include <iostream>

#include "classes/headers/bit_board.hh"
#include "classes/headers/board_importer.hh"
#include "classes/headers/minimax.hh"
#include "classes/headers/move_gen.hh"

int main(int, char**) {
    BitBoard* board = new BitBoard();
    BoardImporter::ImportFEN(board, "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1");
    
    //MoveGen gen = MoveGen(Color::White);
    //std::vector<Move> moves = gen.GetAllMoves(board);

    MiniMax miniMax = MiniMax(board);
    Move move = miniMax.GetBestMove(6);

    printf("%d\n", board->totalMoves);
}
