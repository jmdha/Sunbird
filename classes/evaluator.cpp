#include "headers/evaluator.hh"

Evaluator::~Evaluator() {
#ifdef STATS
    printf("Evaluator: %d - %llu evaluations\n", (int) oColor, stats.evalCount);
    printf("Evaluator: %d - %llu no move evaluations\n", (int) oColor, stats.noMoveEvalCount);
#endif
}

int Evaluator::EvaluatePieceCount(const Board &board) {
    int value = 0;
    for (int i = 0; i < PIECECOUNT; ++i)
        value += (board.GetPieceCount(Color::White, (PieceType) i) - board.GetPieceCount(Color::Black, (PieceType) i)) * Utilities::GetPieceValue((PieceType) i);
    return value;
}

int Evaluator::EvaluatePositionValue(const Board &board) {
    return EvaluatePositionValue(board, Color::White) - EvaluatePositionValue(board, Color::Black);
}

int Evaluator::EvaluatePositionValue(const Board &board, Color color) {
    int value   = 0;
    U64 pawns   = board.GetPiecePos(color, PieceType::Pawn)   & board.GetColorBB(color);
    U64 knights = board.GetPiecePos(color, PieceType::Knight) & board.GetColorBB(color);
    U64 bishops = board.GetPiecePos(color, PieceType::Bishop) & board.GetColorBB(color);
    U64 rooks   = board.GetPiecePos(color, PieceType::Rook)   & board.GetColorBB(color);
    U64 queens  = board.GetPiecePos(color, PieceType::Queen)  & board.GetColorBB(color);
    U64 kings   = board.GetPiecePos(color, PieceType::King)   & board.GetColorBB(color);
    
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
