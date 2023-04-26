#ifndef KING_GEN_HH
#define KING_GEN_HH

#include "piece_gen.hh"

class KingGen : public PieceGen {
public:
    explicit KingGen(Color color) : PieceGen(color),
    kingPos((color == Color::White) ? Square::E1 : Square::E8),
    castleKSide((color == Color::White) ? Square::G1 : Square::G8),
    castleQSide((color == Color::White) ? Square::C1 : Square::C8)  {
        if (color == Color::White) {
            castlingBlock[(int) Castling::King] = CastlingBlockSquares::KSideWhite;
            castlingBlock[(int) Castling::Queen] = CastlingBlockSquares::QSideWhite;
            castlingAttack[(int) Castling::King] = CastlingAttackSquares::KSideWhite;
            castlingAttack[(int) Castling::Queen] = CastlingAttackSquares::QSideWhite;
        } else {
            castlingBlock[(int) Castling::King] = CastlingBlockSquares::KSideBlack;
            castlingBlock[(int) Castling::Queen] = CastlingBlockSquares::QSideBlack;
            castlingAttack[(int) Castling::King] = CastlingAttackSquares::KSideBlack;
            castlingAttack[(int) Castling::Queen] = CastlingAttackSquares::QSideBlack;
        }
    };
    U8 GetQuietMoves (std::array<Move, MAXMOVECOUNT> &moves, const std::shared_ptr<Board> &board, U8 startIndex) final;
    U8 GetAttackMoves(std::array<Move, MAXMOVECOUNT> &moves, const std::shared_ptr<Board> &board, U8 startIndex) final;
private:
    CastlingBlockSquares castlingBlock[2]{};
    CastlingAttackSquares castlingAttack[2]{};
    Square kingPos;
    Square castleKSide;
    Square castleQSide;
};

#endif //KING_GEN_HH
