#include <engine/evaluator.hpp>

#include <chess/internal/constants.hpp>
#include <chess/internal/bit_shift.hpp>
#include <chess/board.hpp>

int Evaluator::EvaluatePieceCount(const Board &board) const {
    int value = 0;
    for (int i = 0; i < PIECECOUNT - 1; ++i) {
        const int whiteCount = board.GetPieceCount(Color::White, (PieceType) i);
        const int blackCount = board.GetPieceCount(Color::Black, (PieceType) i);
        const int pieceValue = sPieceValues.at(i);
        value += (whiteCount - blackCount) * pieceValue;
    }
    return value;
}

int Evaluator::EvaluatePositionValue(const Board &board, Color color) {
    int value   = 0;
    U64 pawns   = board.GetPiecePos(color, PieceType::Pawn);
    U64 knights = board.GetPiecePos(color, PieceType::Knight);
    U64 bishops = board.GetPiecePos(color, PieceType::Bishop);
    U64 rooks   = board.GetPiecePos(color, PieceType::Rook);
    U64 queens  = board.GetPiecePos(color, PieceType::Queen);
    U64 kings   = board.GetPiecePos(color, PieceType::King);
    
    if (color == Color::White) {
        while (pawns)   value += PosValuePawn      [Utilities::LSB_Pop(&pawns)];
        while (knights) value += PosValueKnight    [Utilities::LSB_Pop(&knights)];
        while (bishops) value += PosValueBishop    [Utilities::LSB_Pop(&bishops)];
        while (rooks)   value += PosValueRook      [Utilities::LSB_Pop(&rooks)];
        while (queens)  value += PosValueQueen     [Utilities::LSB_Pop(&queens)];
        while (kings)   value += PosValueKing_Early[Utilities::LSB_Pop(&kings)];
    } else {
        while (pawns)   value += IPosValuePawn     [Utilities::LSB_Pop(&pawns)];
        while (knights) value += IPosValueKnight   [Utilities::LSB_Pop(&knights)];
        while (bishops) value += IPosValueBishop   [Utilities::LSB_Pop(&bishops)];
        while (rooks)   value += IPosValueRook     [Utilities::LSB_Pop(&rooks)];
        while (queens)  value += IPosValueQueen    [Utilities::LSB_Pop(&queens)];
        while (kings)   value += IPosValueKing     [Utilities::LSB_Pop(&kings)];
    }
    return value;
}

