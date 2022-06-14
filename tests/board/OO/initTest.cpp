/*#include <stdexcept>
#include <string>
#include <stdlib.h>

#include "../../../classes/headers/board_OO.h"
#include "../../../classes/headers/constants.h"

int main(int argc, char* argv[]) {
    Board_OO board = Board_OO();
    board.Initialize();

    for (int x = 0; x < WIDTH; x++) {
        if (board.GetPiece(x, PAWNROWWHITE)->color !=  Color::White || board.GetPiece(x, PAWNROWWHITE)->GetType() != PieceType::Pawn)
            throw std::logic_error(std::to_string(x) + ", " + std::to_string(PAWNROWWHITE) + " not white white");
        if (board.GetPiece(x, PAWNROWBLACK)->color !=  Color::Black || board.GetPiece(x, PAWNROWBLACK)->GetType() != PieceType::Pawn)
            throw std::logic_error(std::to_string(x) + ", " + std::to_string(PAWNROWBLACK) + " not black pawn");
    }

    if (board.GetPiece(0, 0)->color !=  Color::White || board.GetPiece(0, 0)->GetType() != PieceType::Rook)
        throw std::logic_error("0,0 not white rook");
    if (board.GetPiece(7, 0)->color !=  Color::White || board.GetPiece(7, 0)->GetType() != PieceType::Rook)
        throw std::logic_error("7,0 not white rook");
    if (board.GetPiece(0, 7)->color !=  Color::Black || board.GetPiece(0, 7)->GetType() != PieceType::Rook)
        throw std::logic_error("0,7 not black rook");
    if (board.GetPiece(7, 7)->color !=  Color::Black || board.GetPiece(7, 7)->GetType() != PieceType::Rook)
        throw std::logic_error("7,7 not black rook");

    if (board.GetPiece(1, 0)->color !=  Color::White || board.GetPiece(1, 0)->GetType() != PieceType::Knight)
        throw std::logic_error("1,0 not white knight");
    if (board.GetPiece(6, 0)->color !=  Color::White || board.GetPiece(6, 0)->GetType() != PieceType::Knight)
        throw std::logic_error("6,0 not white knight");
    if (board.GetPiece(1, 7)->color !=  Color::Black || board.GetPiece(1, 7)->GetType() != PieceType::Knight)
        throw std::logic_error("1,7 not black knight");
    if (board.GetPiece(6, 7)->color !=  Color::Black || board.GetPiece(6, 7)->GetType() != PieceType::Knight)
        throw std::logic_error("6,7 not black knight");

    if (board.GetPiece(2, 0)->color !=  Color::White || board.GetPiece(2, 0)->GetType() != PieceType::Bishop)
        throw std::logic_error("2,0 not white bishop");
    if (board.GetPiece(5, 0)->color !=  Color::White || board.GetPiece(5, 0)->GetType() != PieceType::Bishop)
        throw std::logic_error("5,0 not white bishop");
    if (board.GetPiece(2, 7)->color !=  Color::Black || board.GetPiece(2, 7)->GetType() != PieceType::Bishop)
        throw std::logic_error("2,7 not black bishop");
    if (board.GetPiece(5, 7)->color !=  Color::Black || board.GetPiece(5, 7)->GetType() != PieceType::Bishop)
        throw std::logic_error("5,7 not black bishop");

    if (board.GetPiece(3, 0)->color !=  Color::White || board.GetPiece(3, 0)->GetType() != PieceType::Queen)
        throw std::logic_error("3,0 not white queen");
    if (board.GetPiece(3, 7)->color !=  Color::Black || board.GetPiece(3, 7)->GetType() != PieceType::Queen)
        throw std::logic_error("3,7 not black queen");

    if (board.GetPiece(4, 0)->color !=  Color::White || board.GetPiece(4, 0)->GetType() != PieceType::King)
        throw std::logic_error("4,0 not white king");
    if (board.GetPiece(4, 7)->color !=  Color::Black || board.GetPiece(4, 7)->GetType() != PieceType::King)
        throw std::logic_error("4,7 not black king");

    for (int x = 0; x < WIDTH; x++) {
        for (int y = PAWNROWWHITE + 1; y < PAWNROWBLACK; y++) {
            if (board.GetPiece(x, y) != nullptr)
                throw std::logic_error("Unintended piece at " + std::to_string(x) + ", " + std::to_string(y));

        }
    }

    exit(EXIT_SUCCESS);
}*/