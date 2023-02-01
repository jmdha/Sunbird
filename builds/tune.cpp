#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-slicing"
#include <iostream>
#include <cassert>

#include "bit_shifts.hh"
#include "board.hh"
#include "constants.hh"

class Parameter {
public:
    Parameter() = default;
};

class PieceValParam : public Parameter {
public:
    const PieceType type;
    const int defaultVal;
    explicit PieceValParam(PieceType type) : type(type), defaultVal(pieceValues[(int) type]) {
        assert(type != PieceType::None && type != PieceType::King);
    };
};

std::vector<Parameter> parameters {
    PieceValParam(PieceType::Pawn),
    PieceValParam(PieceType::Bishop),
    PieceValParam(PieceType::Knight),
    PieceValParam(PieceType::Rook),
    PieceValParam(PieceType::Queen),
    PieceValParam(PieceType::King)
};

int main(int argc, char* argv[]) {
    BitShifts::Init();
    Board board = Board();

}

#pragma clang diagnostic pop