#ifndef CHESS_BOARD
#define CHESS_BOARD

#include <cassert>
#include <string>

#include "internal/constants.hpp"
#include "internal/move.hpp"
#include "internal/utilities.hpp"
#include "internal/zobrist.hpp"
#include "jank/bit/bit.hpp"
#include "jank/container/fixed_stack.hpp"

namespace Chess {
// Class representing the current state of a game of chess
class Board {
public:
    Board() { EP.push(Column::None); }
    // Initialization
    void Initialize();
    // Pieces
    inline PieceType GetType(Square square) const;
    inline int GetPieceCount(Color color, PieceType type) const;
    inline U64 GetPiecePos(PieceType type) const;
    inline U64 GetPiecePos(Color color, PieceType type) const;
    inline U64 GetOccupiedBB() const;
    inline U64 GetColorBB(Color color) const;
    inline int GetPly() const;
    inline void PlacePiece(Square square, PieceChar pieceChar);
    inline void PlacePiece(Square square, PieceType type, Color color);
    inline void RemovePiece(Square square, PieceType type, Color color);
    // Moves
    void DoMove(Move &move);
    void UndoMove(Move move);
    // King Safety
    bool IsKingSafe(U64 tempOccuracyBoard, U64 tempEnemyBoard, U64 tempKingBoard) const;
    inline bool IsKingSafe(U64 tempOccuracyBoard, U64 tempEnemyBoard) const;
    inline bool IsKingSafe(U64 tempOccuracyBoard) const;
    inline bool IsKingSafe() const;
    // Castling
    inline bool IsCastlingAllowed(Color color, Castling side) const;
    inline void EnableCastling(Color color, Castling castling);
    void EnableCastling(Move &move);
    void DisableCastling(Move &move, Color color, Castling side);
    // EnPassant
    inline Column GetEP() const;
    inline void PushEP(Column col) {
        if (EP.top() != Column::None) [[unlikely]]
            zobrist.FlipEnPassant(EP.top());
        if (col != Column::None) [[unlikely]]
            zobrist.FlipEnPassant(col);
        EP.push(Column(col));
    }
    inline void PopEP() {
        if (EP.top() != Column::None) [[unlikely]]
            zobrist.FlipEnPassant(EP.top());
        EP.pop();
        if (EP.top() != Column::None) [[unlikely]]
            zobrist.FlipEnPassant(EP.top());
    }
    // Misc
    inline Color GetColor() const;
    inline Color GetOppColor() const;
    inline Color GetColor(Square sq) const;
    inline U64 GetHash() const;
    inline bool IsThreefoldRep() const;
    U64 GenerateAttackSquares(Color color) const;
    inline void SetTurn(Color color);
    inline void SwitchTurn();

private:
    Color turn = Color::None;
    Color oppColor = Color::None;
    U64 pieceBB[PIECECOUNT]{0};
    U64 colorBB[COLORCOUNT]{0};
    U64 occupiedBB = 0;
    std::array<PieceType, SQUARECOUNT> pieceBoard = [] {
        std::array<PieceType, SQUARECOUNT> array;
        for (auto square : SQUARES)
            array[(int)square] = PieceType::None;
        return array;
    }();
    bool castlingAllowed[2][2]{false};
    Zobrist zobrist = Zobrist();
    int ply = 0;

    jank::container::fixed_stack<PieceType, 32> captures;
    jank::container::fixed_stack<Column, MAX_PLY> EP;
};

inline PieceType Board::GetType(Square square) const {
    assert(square != Square::None);
    return pieceBoard[(int)square];
}

inline int Board::GetPieceCount(const Color color, const PieceType type) const {
    assert(type != PieceType::None);
    return jank::bit::popcount(GetPiecePos(color, type));
}

U64 Board::GetPiecePos(PieceType type) const {
    assert(type != PieceType::None);
    return pieceBB[(int)type];
}

inline U64 Board::GetPiecePos(const Color color, const PieceType type) const {
    assert(type != PieceType::None);
    return pieceBB[(int)type] & colorBB[(int)color];
}

inline U64 Board::GetOccupiedBB() const { return occupiedBB; }

inline bool Board::IsKingSafe(U64 tempOccuracyBoard, U64 tempEnemyBoard) const {
    return IsKingSafe(tempOccuracyBoard, tempEnemyBoard, GetPiecePos(turn, PieceType::King));
}

inline bool Board::IsKingSafe(U64 tempOccuracyBoard) const {
    return IsKingSafe(tempOccuracyBoard, GetColorBB(oppColor));
}

inline bool Board::IsKingSafe() const { return IsKingSafe(GetOccupiedBB()); }

inline Color Board::GetColor() const { return turn; }

inline Color Board::GetOppColor() const { return oppColor; }

inline Color Board::GetColor(Square sq) const {
    if (colorBB[(U8)Color::White] & C64(sq))
        return Color::White;
    else if (colorBB[(U8)Color::Black] & C64(sq))
        return Color::Black;
    else
        return Color::None;
}

inline U64 Board::GetHash() const { return zobrist.GetHash(); }

inline int Board::GetPly() const { return ply; }

inline bool Board::IsThreefoldRep() const { return zobrist.IsThreefoldRep(); }

U64 Board::GetColorBB(Color color) const { return colorBB[(int)color]; }

inline Column Board::GetEP() const { return EP.top(); }

inline void Board::PlacePiece(Square square, PieceChar pieceChar) {
    PlacePiece(square, Utilities::GetPieceType(pieceChar), Utilities::GetPieceColor(pieceChar));
}

inline void Board::PlacePiece(Square square, PieceType type, Color color) {
    assert(type != PieceType::None);
    U64 bit = C64(square);
    pieceBB[(U8)type] |= bit;
    colorBB[(U8)color] |= bit;
    occupiedBB |= bit;
    pieceBoard[(int)square] = type;
    zobrist.FlipSquare(square, type, color);
    assert(occupiedBB == (colorBB[0] | colorBB[1]));
}

inline void Board::RemovePiece(Square square, PieceType type, Color color) {
    assert(type != PieceType::None);
    U64 bit = C64(square);
    pieceBB[(U8)type] ^= bit;
    colorBB[(U8)color] ^= bit;
    occupiedBB ^= bit;
    pieceBoard[(int)square] = PieceType::None;
    zobrist.FlipSquare(square, type, color);
    assert(occupiedBB == (colorBB[0] | colorBB[1]));
}

bool Board::IsCastlingAllowed(Color color, Castling side) const {
    return castlingAllowed[(int)color][(int)side];
}

void Board::SetTurn(Color color) {
    turn = color;
    oppColor = Utilities::GetOppositeColor(color);
}

void Board::SwitchTurn() {
    turn = Utilities::GetOppositeColor(turn);
    oppColor = Utilities::GetOppositeColor(turn);
}

void Board::EnableCastling(Color color, Castling castling) {
    castlingAllowed[(int)color][(int)castling] = true;
}

} // namespace Chess

#endif // CHESS_BOARD
