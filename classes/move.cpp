#include "headers/move.hh"

std::string Move::ToString() {
    std::string move = "";
    move += Utilities::GetSquareString(fromSquare);
    move += Utilities::GetSquareString(toSquare);
    return move;
}