#include "headers/evaluator.hh"

Evaluator::~Evaluator() {
#ifdef STATS
    printf("Evaluator: %d - %llu evaluations\n", (int) oColor, stats.evalCount);
#endif
}

int Evaluator::Evalute(const Board board) {
#ifdef STATS
    stats.evalCount++;
#endif
    int value = EvaluatePieceCount(board) + EvaluatePositionValue(board);
    if (oColor == Color::Black)
        value *= -1;
    if (board.color != oColor)
        value *= -1;
    return value;
}

int Evaluator::EvaluatePieceCount(const Board board) {
    int value = 0;
    for (int i = 0; i < PIECECOUNT; i++)
        value += (board.popCount[(int) Color::White][i] - board.popCount[(int) Color::Black][i]) * Utilities::GetPieceValue((PieceType) i);
    return value;
}

int Evaluator::EvaluatePositionValue(const Board board) {
    return EvaluatePositionValue(board, Color::White) - EvaluatePositionValue(board, Color::Black);
}

int Evaluator::EvaluatePositionValue(const Board board, Color color) {
    int value   = 0;
    U64 pawns   = board.pieceBB[(int) PieceType::Pawn]      & board.colorBB[(int) color];
    U64 knights = board.pieceBB[(int) PieceType::Knight]    & board.colorBB[(int) color];
    U64 bishops = board.pieceBB[(int) PieceType::Bishop]    & board.colorBB[(int) color];
    U64 rooks   = board.pieceBB[(int) PieceType::Rook]      & board.colorBB[(int) color];
    U64 queens  = board.pieceBB[(int) PieceType::Queen]     & board.colorBB[(int) color];
    U64 kings   = board.pieceBB[(int) PieceType::King]      & board.colorBB[(int) color];

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
