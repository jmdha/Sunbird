#ifndef BOARD
#define BOARD

#include <string>

#include "constants.hh"
#include "utilities.hh"
#include "move.hh"

// Class representing the current state of a game of chess
class Board {
public:
    struct Stats {
        U64 totalMoves = 0;
    };
    // Initialization
    void Initialize();
    // Pieces
    inline PieceType GetType(Square square) const;
    // Moves
    void DoMove(Move move);
    void UndoMove(Move move);
    // Misc
    inline Color GetColor() const;
    inline Color GetColor(Square sq) const;
    inline Color GetOriginalColor() const;
    inline U64 GetHash() const; 
    inline Stats GetStats() const;

private:
    Color color = Color::None;
    Color originalColor = Color::None;
    U64 pieceBB[PIECECOUNT] = { 0 };
    U64 colorBB[COLORCOUNT] = { 0 };
    U64 occupiedBB = 0;
    U64 enPassant = 0;
    int popCount[COLORCOUNT][PIECECOUNT] = { 0 };
    bool castlingAllowed[2][2] = { 0 };
    int movesSinceCastlingDisallowed[2][2] = { 0 };
    Stats stats = Stats();

    // Pieces
    void PlacePiece(Square square, PieceChar pieceChar);
    void PlacePiece(Square square, PieceType type, Color color);
    void RemovePiece(Square square, PieceType type, Color color);
    // Castling
    void EnableCastling(Color color, Castling side);
    void DisableCastling(Color color, Castling side);
    void IncrementCastling();
    void DecrementCastling();
    friend class Evaluator;
    friend class MoveGen;
    friend class BoardImporter;
};

inline PieceType Board::GetType(Square square) const {
    for (int i = 0; i < PIECECOUNT; i++)
        if (pieceBB[i] & C64(square))
            return (PieceType) i;
    return PieceType::None;	
}

inline Color Board::GetColor() const {
    return color;
};

inline Color Board::GetColor(Square sq) const {
    if (colorBB[(int) Color::White] & C64(sq))
        return Color::White;
    else if (colorBB[(int) Color::Black] & C64(sq))
        return Color::Black;
    else
        return Color::None;
};

inline Color Board::GetOriginalColor() const {
    return originalColor;
};

inline U64 Board::GetHash() const {
    return occupiedBB;
};

inline Board::Stats Board::GetStats() const {
    return stats;
};

#endif
