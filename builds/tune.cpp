#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-slicing"
#include <iostream>
#include <cassert>

#include "bit_shifts.hh"
#include "board.hh"
#include "constants.hh"
#include "evaluator.hh"
#include "minimax.hh"
#include "board_importer.hh"
#include "positions.hh"

class Parameter {
public:
    enum class Type {
        None,
        PieceVal
    };
    const Type type;
    explicit Parameter(Type type) : type(type) {};
    virtual ~Parameter() = default;
};

class PieceValParam : public Parameter {
public:
    const PieceType type;
    const int defaultVal;
    explicit PieceValParam(PieceType type) : type(type), defaultVal(pieceValues[(int) type]), Parameter(Parameter::Type::PieceVal) {
        assert(type != PieceType::None && type != PieceType::King);
    };
};

std::vector<Parameter> parameters {
    PieceValParam(PieceType::Pawn),
    PieceValParam(PieceType::Bishop),
    PieceValParam(PieceType::Knight),
    PieceValParam(PieceType::Rook),
    PieceValParam(PieceType::Queen)
};

std::array<int, PIECECOUNT> GenPieceVals(const std::vector<Parameter>& params) {
    std::array<int, PIECECOUNT> pieceVals{pieceValues};
    for (auto param : params) {
        if (param.type == Parameter::Type::PieceVal) {
            auto p = dynamic_cast<PieceValParam*>(&param);
            pieceVals.at((int) p->type);
        }
    }
}

int main(int argc, char* argv[]) {
    BitShifts::Init();
    Board board = Board();
    const int paramCount = 1;
    std::vector<Parameter> activeParameters;
    std::vector<MiniMax*> engines { new MiniMax(&board), new MiniMax(&board) };
    auto positions = Positions::GetPositions(100);
    std::vector<U64> winCount { 0, 0 };
    for (const auto & position : positions) {
        BoardImporter::ImportFEN(&board, position);
        bool gameOver = false;
        bool draw = false;
        while (!gameOver) {
            Move move;
            if (board.GetColor() == Color::White)
                move = engines.at(0)->GetBestMove(3);
            else
                move = engines.at(1)->GetBestMove(3);
            if (move.GetType() == MoveType::SPECIAL_DRAW)
                draw = true;
            if (move.GetValue() != 0 && move.GetType() != MoveType::SPECIAL_DRAW) {
                board.DoMove(move);
            } else
                gameOver = true;
        }
        if (!draw)
            winCount.at(1 - (int) board.GetColor())++;
        printf("%llu - %llu\n", winCount.at(0), winCount.at(1) );
    }
}

#pragma clang diagnostic pop