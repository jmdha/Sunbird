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
    PieceType DoMove(Move move);
    void UndoMove(Move move, PieceType capturedType);
    // Misc
    inline Color GetColor() const;
    inline Color GetColor(Square sq) const;
    inline Color GetOriginalColor() const;
    inline U64 GetHash() const; 
    inline Stats GetStats() const;

    bool friend operator==(const Board & lhs, const Board & rhs) {
        if (!(lhs.color == rhs.color && lhs.occupiedBB == rhs.occupiedBB))
            return false;
        for (int i = 0; i < PIECECOUNT; i++)
            if (lhs.pieceBB[i] != rhs.pieceBB[i])
                return false;
        for (int i = 0; i < COLORCOUNT; i++)
            if (lhs.colorBB[i] != rhs.colorBB[i])
                return false;
        for (int i = 0; i < COLORCOUNT; i++)
            for (int t = 0; t < PIECECOUNT; t++)
                if (lhs.popCount[i][t] != rhs.popCount[i][t])
                    return false;
        for (int i = 0; i < 2; i++)
            for (int t = 0; t < 2; t++)
                if (lhs.castlingAllowed[i][t] != rhs.castlingAllowed[i][t])
                    return false;
        return true;
    }

private:
    Color color = Color::None;
    Color originalColor = Color::None;
    U64 pieceBB[PIECECOUNT] = { 0 };
    U64 colorBB[COLORCOUNT] = { 0 };
    U64 occupiedBB = 0;
    U64 enPassant = 0;
    int popCount[COLORCOUNT][PIECECOUNT] = { 0 };
    bool castlingAllowed[2][2] = { 0 };
    Stats stats = Stats();

    // Pieces
    inline void PlacePiece(Square square, PieceChar pieceChar);
    inline void PlacePiece(Square square, PieceType type, Color color);
    inline void RemovePiece(Square square, PieceType type, Color color);
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

inline void Board::PlacePiece(Square square, PieceChar pieceChar) {
    PlacePiece(square, Utilities::GetPieceType(pieceChar), Utilities::GetPieceColor(pieceChar));
}

inline void Board::PlacePiece(Square square, PieceType type, Color color) {
    U64 bit = C64(square);
    pieceBB[(int) type] |= bit;
    colorBB[(int) color] |= bit;
    occupiedBB |= bit;
    popCount[(int) color][(int) type]++;
}

inline void Board::RemovePiece(Square square, PieceType type, Color color) {
    U64 bit = C64(square);
    pieceBB[(int) type] ^= bit;
    colorBB[(int) color] ^= bit;
    occupiedBB ^= bit;
    popCount[(int) color][(int) type]--;
}

#endif // BOARD
