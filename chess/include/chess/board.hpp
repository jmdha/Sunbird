#ifndef BOARD
#define BOARD

#include <string>
#include <cassert>

#include "internal/constants.hpp"
#include "internal/utilities.hpp"
#include "internal/zobrist.hpp"
#include "move.hpp"

// Class representing the current state of a game of chess
class Board {
public:
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
    // Moves
    void DoMove(Move &move);
    void UndoMove(Move move);
    // King Safety
    bool IsKingSafe(U64 tempOccuracyBoard, U64 tempEnemyBoard, U64 tempKingBoard);
    inline bool IsKingSafe(U64 tempOccuracyBoard, U64 tempEnemyBoard);
    inline bool IsKingSafe(U64 tempOccuracyBoard);
    inline bool IsKingSafe();
    // Misc
    inline Color GetColor() const;
    inline Color GetOppColor() const;
    inline Color GetColor(Square sq) const;
    inline void SwitchTurn();
    inline Column GetEnPassant() const;
    inline bool IsCastlingAllowed(Color color, Castling side);
    inline U64 GetHash() const; 
    inline bool IsThreefoldRep() const;
    U64 GenerateAttackSquares(Color color) const;

private:
    Color turn = Color::None;
    Color oppColor = Color::None;
    U64 pieceBB[PIECECOUNT] { 0 };
    U64 colorBB[COLORCOUNT] { 0 };
    U64 occupiedBB = 0;
    Column enPassant = Column::None;
    U8 popCount[COLORCOUNT][PIECECOUNT] { 0 };
    bool castlingAllowed[2][2] { false };
    Zobrist zobrist = Zobrist();
    int ply = 0;

    // Pieces
    inline void PlacePiece(Square square, PieceChar pieceChar);
    inline void PlacePiece(Square square, PieceType type, Color color);
    inline void RemovePiece(Square square, PieceType type, Color color);
    // Castling
    void EnableCastling(Move &move);
    void DisableCastling(Move &move, Color color, Castling side);
    // Misc
    void SetEnPassant(Column col);

    friend class BoardImporter;
};

inline PieceType Board::GetType(Square square) const {
    for (U8 i = 0; i < PIECECOUNT; ++i)
        if (pieceBB[i] & C64(square))
            return (PieceType) i;
    return PieceType::None;	
}

inline int Board::GetPieceCount(const Color color, const PieceType type) const {
    assert(type != PieceType::None);
    return popCount[(int) color][(int) type];
}

U64 Board::GetPiecePos(PieceType type) const {
    assert(type != PieceType::None);
    return pieceBB[(int) type];
}

inline U64 Board::GetPiecePos(const Color color, const PieceType type) const {
    assert(type != PieceType::None);
    return pieceBB[(int) type] & colorBB[(int) color];
}

inline U64 Board::GetOccupiedBB() const {
    return occupiedBB;
}

inline bool Board::IsKingSafe(U64 tempOccuracyBoard, U64 tempEnemyBoard) {
    return IsKingSafe(tempOccuracyBoard, tempEnemyBoard, GetPiecePos(turn, PieceType::King));
}

inline bool Board::IsKingSafe(U64 tempOccuracyBoard) {
    return IsKingSafe(tempOccuracyBoard, GetColorBB(oppColor));
}

inline bool Board::IsKingSafe() {
    return IsKingSafe(GetOccupiedBB());
}

inline Color Board::GetColor() const {
    return turn;
}

inline Color Board::GetOppColor() const {
    return oppColor;
}

inline Color Board::GetColor(Square sq) const {
    if (colorBB[(U8) Color::White] & C64(sq))
        return Color::White;
    else if (colorBB[(U8) Color::Black] & C64(sq))
        return Color::Black;
    else
        return Color::None;
}

inline U64 Board::GetHash() const {
    return zobrist.GetHash();
}

inline int Board::GetPly() const {
    return ply;
}

inline bool Board::IsThreefoldRep() const {
    return zobrist.IsThreefoldRep();
}

inline void Board::PlacePiece(Square square, PieceChar pieceChar) {
    PlacePiece(square, Utilities::GetPieceType(pieceChar), Utilities::GetPieceColor(pieceChar));
}

inline void Board::PlacePiece(Square square, PieceType type, Color color) {
    assert(type != PieceType::None);
    U64 bit = C64(square);
    pieceBB[(U8) type] |= bit;
    colorBB[(U8) color] |= bit;
    occupiedBB |= bit;
    popCount[(U8) color][(U8) type]++;
    zobrist.FlipSquare(square, type, color);
}

inline void Board::RemovePiece(Square square, PieceType type, Color color) {
    assert(type != PieceType::None);
    U64 bit = C64(square);
    pieceBB[(U8) type] ^= bit;
    colorBB[(U8) color] ^= bit;
    occupiedBB ^= bit;
    popCount[(U8) color][(U8) type]--;
    zobrist.FlipSquare(square, type, color);
}

U64 Board::GetColorBB(Color color) const {
    return colorBB[(int) color];
}

Column Board::GetEnPassant() const {
    return enPassant;
}

bool Board::IsCastlingAllowed(Color color, Castling side) {
    return castlingAllowed[(int) color][(int) side];
}

void Board::SwitchTurn() {
    turn = Utilities::GetOppositeColor(turn);
    oppColor = Utilities::GetOppositeColor(turn);
}

#endif // BOARD
