#include "headers/evaluator.hh"

Evaluator::Evaluator(Color color) :
    color(color),
    oppColor(Utilities::GetOppositeColor(color)) {}

int Evaluator::Evalute(const BitBoard board) {
    return EvaluatePieceCount(board) + EvaluatePositionValue(board);
}

int Evaluator::EvaluatePieceCount(const BitBoard board) {
    int value = 0;
    for (int i = 0; i < PIECECOUNT; i++) {
        int pieceValue = Utilities::GetPieceValue((PieceType) i);
        value += board.popCount[(int) color][i] * pieceValue;
        value -= board.popCount[(int) oppColor][i] * pieceValue;
    }
    return value;
}

int Evaluator::EvaluatePositionValue(const BitBoard board) {
    return EvaluatePositionValue(board, board.color) - EvaluatePositionValue(board, Utilities::GetOppositeColor(board.color));
}

int Evaluator::EvaluatePositionValue(const BitBoard board, Color color) {
    int value   = 0;
    U64 pawns   = board.pieceBB[(int) PieceType::Pawn]      & board.colorBB[(int) color];
    U64 knights = board.pieceBB[(int) PieceType::Knight]    & board.colorBB[(int) color];
    U64 bishops = board.pieceBB[(int) PieceType::Bishop]    & board.colorBB[(int) color];
    U64 rooks   = board.pieceBB[(int) PieceType::Rook]      & board.colorBB[(int) color];
    U64 queens  = board.pieceBB[(int) PieceType::Queen]     & board.colorBB[(int) color];
    U64 kings   = board.pieceBB[(int) PieceType::King]      & board.colorBB[(int) color];

    int pawnIndex;
    while (pawns)   value += GetPiecePositionValue(PieceType::Pawn,     (Square) Utilities::LSB_Pop(&pawns),    color);
    int knightIndex;
    while (knights) value += GetPiecePositionValue(PieceType::Knight,   (Square) Utilities::LSB_Pop(&knights),  color);
    int bishopIndex;
    while (bishops) value += GetPiecePositionValue(PieceType::Bishop,   (Square) Utilities::LSB_Pop(&bishops),  color);
    int rookIndex;
    while (rooks)   value += GetPiecePositionValue(PieceType::Rook,     (Square) Utilities::LSB_Pop(&rooks),    color);
    int queenIndex;
    while (queens)  value += GetPiecePositionValue(PieceType::Queen,    (Square) Utilities::LSB_Pop(&queens),   color);
    int kingIndex;
    while (kings)   value += GetPiecePositionValue(PieceType::King,     (Square) Utilities::LSB_Pop(&kings),    color);

    return value;
}

int Evaluator::GetPiecePositionValue(PieceType type, Square position, Color color) {
    switch (type)
    {
    case PieceType::Pawn:
        if (color == Color::White)
            return PosValuePawn[(int) position];
        else
            return PosValuePawn[63 - (int) position];
    case PieceType::Knight:
        if (color == Color::White)
            return PosValueKnight[(int) position];
        else
            return PosValueKnight[63 - (int) position];
    case PieceType::Bishop:
        if (color == Color::White)
            return PosValueBishop[(int) position];
        else
            return PosValueBishop[63 - (int) position];
    case PieceType::Rook:
        if (color == Color::White)
            return PosValueRook[(int) position];
        else
            return PosValueRook[63 - (int) position];
    case PieceType::Queen:
        if (color == Color::White)
            return PosValueQueen[(int) position];
        else
            return PosValueQueen[63 - (int) position];
    case PieceType::King:
        if (color == Color::White)
            return PosValueKing_Early[(int) position];
        else
            return PosValueKing_Early[63 - (int) position];
    }

    throw std::logic_error("Unexpected piece type " + std::to_string((int) type));
}
