#include "headers/move.hh"

std::string Move::ToString() {    
    if (type == MoveType::KingCastle) {
        if (fromColor == Color::White)
            return "O-O";
        else
            return "o-o";
    } else if (type == MoveType::QueenCastle) {
        if (fromColor == Color::White)
            return "O-O-O";
        else
            return "o-o-o";
    }
    std::string move = "";        
    move += Utilities::GetSquareString(fromSquare);
    move += Utilities::GetSquareString(toSquare);
    return move;
}