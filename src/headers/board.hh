#ifndef BOARD
#define BOARD

#include <string>

#include "constants.hh"
#include "utilities.hh"
#include "move.hh"
#include "zobrist.hh"

// Class representing the current state of a game of chess
class Board {
public:
    struct Stats {
        U64 totalMoves = 0;
        U64 epMoves = 0;
        U64 castlingMoves = 0;
    };
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
    inline Color GetOriginalColor() const;
    inline void SwitchTurn();
    inline void SetOriginalColor(Color color);
    inline Column GetEnPassant() const;
    inline bool IsCastlingAllowed(Color color, Castling side);
    inline U64 GetHash() const; 
    inline Stats GetStats() const;
    inline bool IsThreefoldRep() const;
    U64 GenerateAttackSquares(Color color) const;

    bool friend operator==(const Board & lhs, const Board & rhs) {
        if (lhs.turn != rhs.turn || lhs.originalColor != rhs.originalColor ||
        lhs.occupiedBB != rhs.occupiedBB || lhs.GetHash() != rhs.GetHash() ||
        lhs.enPassant != rhs.enPassant)
            return false;
        for (U8 i = 0; i < PIECECOUNT; ++i)
            if (lhs.pieceBB[i] != rhs.pieceBB[i])
                return false;
        for (U8 i = 0; i < COLORCOUNT; ++i)
            if (lhs.colorBB[i] != rhs.colorBB[i])
                return false;
        for (int i = 0; i < COLORCOUNT; ++i)
            for (int t = 0; t < PIECECOUNT; ++t)
                if (lhs.popCount[i][t] != rhs.popCount[i][t])
                    return false;
        for (int i = 0; i < 2; ++i)
            for (int t = 0; t < 2; ++t)
                if (lhs.castlingAllowed[i][t] != rhs.castlingAllowed[i][t])
                    return false;
        return true;
    }

private:
    Color turn = Color::None;
    Color oppColor = Color::None;
    Color originalColor = Color::None;
    U64 pieceBB[PIECECOUNT] = { 0 };
    U64 colorBB[COLORCOUNT] = { 0 };
    U64 occupiedBB = 0;
    Column enPassant = Column::None;
    U8 popCount[COLORCOUNT][PIECECOUNT] = { 0 };
    bool castlingAllowed[2][2] = { 0 };
    Stats stats = Stats();
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
    return popCount[(int) color][(int) type];
}

U64 Board::GetPiecePos(PieceType type) const {
    return pieceBB[(int) type];
}

inline U64 Board::GetPiecePos(const Color color, const PieceType type) const {
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
};

inline Color Board::GetOppColor() const {
    return oppColor;
};

inline Color Board::GetColor(Square sq) const {
    if (colorBB[(U8) Color::White] & C64(sq))
        return Color::White;
    else if (colorBB[(U8) Color::Black] & C64(sq))
        return Color::Black;
    else
        return Color::None;
};

inline Color Board::GetOriginalColor() const {
    return originalColor;
};

inline U64 Board::GetHash() const {
    return zobrist.GetHash();
};

inline Board::Stats Board::GetStats() const {
    return stats;
};

inline int Board::GetPly() const {
    return ply;
};

inline bool Board::IsThreefoldRep() const {
    return zobrist.IsThreefoldRep();
}

inline void Board::PlacePiece(Square square, PieceChar pieceChar) {
    PlacePiece(square, Utilities::GetPieceType(pieceChar), Utilities::GetPieceColor(pieceChar));
}

inline void Board::PlacePiece(Square square, PieceType type, Color color) {
    U64 bit = C64(square);
    pieceBB[(U8) type] |= bit;
    colorBB[(U8) color] |= bit;
    occupiedBB |= bit;
    popCount[(U8) color][(U8) type]++;
    zobrist.FlipSquare(square, type, color);
}

inline void Board::RemovePiece(Square square, PieceType type, Color color) {
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

void Board::SetOriginalColor(Color color) {
    originalColor = color;
}

void Board::SwitchTurn() {
    turn = Utilities::GetOppositeColor(turn);
    oppColor = Utilities::GetOppositeColor(turn);
}

#endif // BOARD
