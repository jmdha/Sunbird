#include "headers/evaluator.hh"

Evaluator::Evaluator(Color color) :
    color(color),
    oppColor(Utilities::GetOppositeColor(color)) {}

int Evaluator::Evalute(const Board board) {
    return EvaluatePieceCount(board) + EvaluatePositionValue(board);
}

int Evaluator::EvaluatePieceCount(const Board board) {
    int value = 0;
    for (int i = 0; i < PIECECOUNT; i++) {
        int pieceValue = Utilities::GetPieceValue((PieceType) i);
        value += board.popCount[(int) color][i] * pieceValue;
        value -= board.popCount[(int) oppColor][i] * pieceValue;
    }
    return value;
}

int Evaluator::EvaluatePositionValue(const Board board) {
    return EvaluatePositionValue(board, board.color) - EvaluatePositionValue(board, Utilities::GetOppositeColor(board.color));
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
