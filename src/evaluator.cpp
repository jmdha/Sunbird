#include "evaluator.hh"

#include "bit_shifts.hh"

Evaluator::~Evaluator() {
#ifdef STATS
    printf("Evaluator: %d - %llu evaluations\n", (int) oColor, stats.evalCount);
    printf("Evaluator: %d - %llu no move evaluations\n", (int) oColor, stats.noMoveEvalCount);
#endif
}

int Evaluator::EvaluatePieceCount(const Board &board) {
    int value = 0;
    for (int i = 0; i < PIECECOUNT - 1; ++i)
        value += (board.GetPieceCount(Color::White, (PieceType) i) - board.GetPieceCount(Color::Black, (PieceType) i)) * Utilities::GetPieceValue((PieceType) i);
    return value;
}

int Evaluator::EvaluatePawnStructure(const Board &board) {
    return EvaluatePawnStructure(board, Color::White) - EvaluatePawnStructure(board, Color::Black);
}

int Evaluator::EvaluatePawnStructure(const Board &board, Color color) {
    int value = 0;
    U64 pawns = board.GetPiecePos(color, PieceType::Pawn);
    while (pawns) {
        const U8 pawn = Utilities::LSB_Pop(&pawns);
        // Doubled Pawns
        if (C64(pawn) & BitShifts::GetDoubled(color, pawn))
            value += (int) PawnStructureValue::Doubled;
        // Connected Pawns
        value += (int) PawnStructureValue::Connected *
                Utilities::PopCount(board.GetPiecePos(color, PieceType::Pawn) & BitShifts::GetConnected(color, pawn));
    }
    return value;
}

int Evaluator::EvaluatePositionValue(const Board &board) {
    return EvaluatePositionValue(board, Color::White) - EvaluatePositionValue(board, Color::Black);
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
        while (pawns)   value += PosValuePawn      [63 - Utilities::LSB_Pop(&pawns)];
        while (knights) value += PosValueKnight    [63 - Utilities::LSB_Pop(&knights)];
        while (bishops) value += PosValueBishop    [63 - Utilities::LSB_Pop(&bishops)];
        while (rooks)   value += PosValueRook      [63 - Utilities::LSB_Pop(&rooks)];
        while (queens)  value += PosValueQueen     [63 - Utilities::LSB_Pop(&queens)];
        while (kings)   value += PosValueKing_Early[63 - Utilities::LSB_Pop(&kings)];
    }
    return value;
}
