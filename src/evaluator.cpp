#include "evaluator.hh"

#include "bit_shifts.hh"

Evaluator::~Evaluator() = default;

int Evaluator::EvaluatePieceCount(const Board &board) const {
    int value = 0;
    for (int i = 0; i < PIECECOUNT - 1; ++i)
        value +=
                (board.GetPieceCount(Color::White, (PieceType) i) - board.GetPieceCount(Color::Black, (PieceType) i)) *
                sPieceValues.at(i);
    return value;
}

int Evaluator::EvaluatePawnStructure(const Board &board, Color color) {
    int value = 0;
    const U64 pawns = board.GetPiecePos(color, PieceType::Pawn);
    U64 tempPawns = board.GetPiecePos(color, PieceType::Pawn);
    while (tempPawns) {
        const U8 pawn = Utilities::LSB_Pop(&tempPawns);
        // Doubled Pawns
        if (pawns & BitShifts::GetDoubled(color, pawn))
            value += (int) PawnStructureValue::Doubled;
        // Connected Pawns
        value += (int) PawnStructureValue::Connected *
                Utilities::PopCount(board.GetPiecePos(color, PieceType::Pawn) & BitShifts::GetConnected(color, pawn));
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
        while (pawns)   value += IPosValuePawn      [Utilities::LSB_Pop(&pawns)];
        while (knights) value += IPosValueKnight    [Utilities::LSB_Pop(&knights)];
        while (bishops) value += IPosValueBishop    [Utilities::LSB_Pop(&bishops)];
        while (rooks)   value += IPosValueRook      [Utilities::LSB_Pop(&rooks)];
        while (queens)  value += IPosValueQueen     [Utilities::LSB_Pop(&queens)];
        while (kings)   value += IPosValueKing      [Utilities::LSB_Pop(&kings)];
    }
    return value;
}
