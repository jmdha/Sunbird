#include "chess/internal/move_list.hpp"
#include <chess/internal/bit.hpp>
#include <chess/internal/bitboard.hpp>
#include <chess/internal/constants.hpp>
#include <chess/internal/utilities.hpp>
#include <chess/move_gen.hpp>

namespace Chess::MoveGen {
enum class GenType { Attack, All };
// HACK: This needs to be refactored
template <GenType gType> void GenerateKingMoves(const Position &pos, Color color, MoveList &moves) {
    const std::array<BB, 4> &blockSquares = CASTLING_BLOCK_SQUARES[static_cast<size_t>(color)];
    const std::array<BB, 4> &attackSquares = CASTLING_ATTACK_SQUARES[static_cast<size_t>(color)];

    const auto attackedSquares = pos.GenerateAttackSquares(~color);
    const Square kingPos = pos.GetKing(color);
    assert(kingPos != Square::None);
    if constexpr (gType == GenType::All) {
        BB qMoves = Ring(kingPos, 1) & ~pos.GetPieces() & ~attackedSquares;
        while (qMoves) {
            const Square move = Next(qMoves);
            if (pos.IsKingSafe((pos.GetPieces() ^ kingPos | move), pos.GetPieces(~color),
                               ToBB(move)))
                moves.push<MoveList::Quiet>(Move(MoveType::Quiet, kingPos, move));
        }
        if (pos.AllowsCastling(Castling::King, color) &&
            !(pos.GetPieces() & blockSquares[static_cast<size_t>(Castling::King)]) &&
            !(attackedSquares & attackSquares[static_cast<size_t>(Castling::King)]))
            moves.push<MoveList::Quiet>(
                Move(MoveType::KingCastle, kingPos,
                     First(Shift<Direction::East>(Shift<Direction::East>(ToBB(kingPos))))));
        if (pos.AllowsCastling(Castling::Queen, color) &&
            !(pos.GetPieces() & blockSquares[static_cast<size_t>(Castling::Queen)]) &&
            !(attackedSquares & attackSquares[static_cast<size_t>(Castling::Queen)]))
            moves.push<MoveList::Quiet>(
                Move(MoveType::QueenCastle, kingPos,
                     First(Shift<Direction::West>(Shift<Direction::West>(ToBB(kingPos))))));
    }
    if constexpr (gType == GenType::Attack || gType == GenType::All) {
        BB aMoves = Ring(kingPos, 1) & pos.GetPieces(~color) & ~attackedSquares;
        while (aMoves) {
            const Square move = Next(aMoves);
            if (pos.IsKingSafe(pos.GetPieces() ^ kingPos, pos.GetPieces(~color) ^ move, ToBB(move)))
                moves.push<MoveList::Attack>(Move(MoveType::Capture, kingPos, move));
        }
    }
}

// HACK: This needs to be refactored
template <GenType gType> void GeneratePawnMoves(const Position &pos, Color color, MoveList &moves) {
    constexpr Direction dirs[2] = {Direction::North, Direction::South};
    Direction dir = dirs[static_cast<size_t>(color)];
    BB pieces = pos.GetPieces(color, PieceType::Pawn);
    while (pieces) {
        const Square piece = Next(pieces);
        if constexpr (gType == GenType::All) {
            Square to = static_cast<Square>(Bit::lsb(Ray(piece, dir) & Ring(piece, 1)));
            if (!(to & pos.GetPieces())) {
                if (pos.IsKingSafe(pos.GetPieces() ^ piece | to)) {
                    if (ToBB(piece) & PawnRow[static_cast<size_t>(~color)])
                        for (const auto prom : PromotionMoves)
                            moves.push<MoveList::Quiet>(Move(prom, piece, to));
                    else
                        moves.push<MoveList::Quiet>(Move(MoveType::Quiet, piece, to));
                }
                if (ToBB(piece) & PawnRow[static_cast<size_t>(color)]) {
                    to = First(Ray(piece, dir) & Ring(piece, 2));
                    if (!(to & pos.GetPieces()))
                        if (pos.IsKingSafe(pos.GetPieces() ^ piece | to))
                            moves.push<MoveList::Quiet>(Move(MoveType::DoublePawnPush, piece, to));
                }
            }
        }
        if constexpr (gType == GenType::Attack || gType == GenType::All) {
            BB attacks = PawnAttacks(piece, color) & pos.GetPieces(~color);
            while (attacks) {
                const Square attack = Next(attacks);
                assert(pos.GetType((Square)attack) != PieceType::None);
                if (pos.IsKingSafe((pos.GetPieces() ^ piece) | attack,
                                   pos.GetPieces(~color) ^ attack)) {
                    if (piece & PawnRow[static_cast<size_t>(~color)])
                        for (const auto prom : PromotionCapturesMoves)
                            moves.push<MoveList::Attack>(Move(prom, piece, attack));
                    else
                        moves.push<MoveList::Attack>(Move(MoveType::Capture, piece, attack));
                }
            }
            BB attack = PawnAttacks(piece, color) &
                        ((color == Color::White) ? Row::Row6 : Row::Row3) & pos.GetEP();
            if (attack) {
                const Square sq = Next(attack);
                const Square captured = First(Shift(ToBB(sq), dirs[static_cast<size_t>(~color)]));
                if (pos.IsKingSafe((pos.GetPieces() ^ piece ^ captured) | sq,
                                   pos.GetPieces(~color) ^ captured | sq))
                    moves.push<MoveList::Attack>(Move(MoveType::EPCapture, piece, sq));
            }
        }
    }
}

// Generates possible attack moves for pieces of type pType
template <PieceType pType> void SliderAttack(const Position &pos, Color color, MoveList &moves) {
    static_assert(pType != PieceType::King && pType != PieceType::Pawn);

    BB pieces = pos.GetPieces(color, pType);
    while (pieces) {
        const Square piece = Next(pieces);
        BB unblocked = Attacks(piece, pType);
        for (size_t offset = 1; offset < 8; ++offset) {
            BB ring = Ring(piece, offset);
            BB blockers = ring & unblocked & pos.GetPieces();
            BB temp = blockers;
            while (temp)
                unblocked = unblocked & ~Ray(piece, Next(temp));
            blockers &= pos.GetPieces(~color);
            while (blockers) {
                const Square blocker = Next(blockers);
                if (pos.IsKingSafe((pos.GetPieces() ^ piece) | blocker,
                                   pos.GetPieces(~color) ^ blocker))
                    moves.push<MoveList::Attack>(Move(MoveType::Capture, static_cast<Square>(piece),
                                                      static_cast<Square>(blocker)));
            }
        }
    }
}
template <PieceType pType> void SliderAll(const Position &pos, Color color, MoveList &moves) {
    static_assert(pType != PieceType::King && pType != PieceType::Pawn);

    BB pieces = pos.GetPieces(color, pType);
    while (pieces) {
        const Square piece = Next(pieces);
        BB unblocked = Attacks(piece, pType);
        for (size_t offset = 1; offset < 8; ++offset) {
            BB ring = Ring(piece, offset);
            BB potMoves = ring & unblocked;
            BB blockers = potMoves & pos.GetPieces();
            potMoves ^= blockers;

            while (potMoves) {
                const Square sq = Next(potMoves);
                if (pos.IsKingSafe((pos.GetPieces() ^ piece | sq)))
                    moves.push<MoveList::Quiet>(Move(MoveType::Quiet, piece, sq));
            }

            BB temp = blockers;
            while (temp)
                unblocked = unblocked & ~Ray(piece, Next(temp));

            blockers &= pos.GetPieces(~color);
            while (blockers) {
                const Square blocker = Next(blockers);
                if (pos.IsKingSafe((pos.GetPieces() ^ piece) | blocker,
                                   pos.GetPieces(~color) ^ blocker))
                    moves.push<MoveList::Attack>(Move(MoveType::Capture, static_cast<Square>(piece),
                                                      static_cast<Square>(blocker)));
            }
        }
    }
}
template <GenType g> void GenerateKnightMoves(const Position &pos, Color color, MoveList &moves) {
    BB pieces = pos.GetPieces(color, PieceType::Knight);
    while (pieces) {
        const Square piece = Next(pieces);
        if constexpr (g == GenType::All || g == GenType::Attack) {
            for (BB attacks = Attacks(piece, PieceType::Knight) & pos.GetPieces(~color); attacks;) {
                const Square attack = Next(attacks);
                if (pos.IsKingSafe((pos.GetPieces() ^ piece) | attack,
                                   pos.GetPieces(~color) ^ attack))
                    moves.push<MoveList::Attack>(Move(MoveType::Capture, static_cast<Square>(piece),
                                                      static_cast<Square>(attack)));
            }
        }
        if constexpr (g == GenType::All) {
            for (BB attacks = Attacks(piece, PieceType::Knight) & ~pos.GetPieces(); attacks;) {
                const Square attack = Next(attacks);
                if (pos.IsKingSafe((pos.GetPieces() ^ piece) | attack))
                    moves.push<MoveList::Quiet>(Move(MoveType::Quiet, static_cast<Square>(piece),
                                                     static_cast<Square>(attack)));
            }
        }
    }
}

MoveList GenerateAll(const Position &pos, Color color) {
    MoveList moves;

    SliderAll<PieceType::Queen>(pos, color, moves);
    SliderAll<PieceType::Rook>(pos, color, moves);
    SliderAll<PieceType::Bishop>(pos, color, moves);

    GenerateKnightMoves<GenType::All>(pos, color, moves);
    GenerateKingMoves<GenType::All>(pos, color, moves);
    GeneratePawnMoves<GenType::All>(pos, color, moves);

    moves.finish();
    return moves;
}

MoveList GenerateAttack(const Position &pos, Color color) {
    MoveList moves;

    SliderAttack<PieceType::Queen>(pos, color, moves);
    SliderAttack<PieceType::Rook>(pos, color, moves);
    SliderAttack<PieceType::Bishop>(pos, color, moves);

    GenerateKnightMoves<GenType::Attack>(pos, color, moves);
    GenerateKingMoves<GenType::Attack>(pos, color, moves);
    GeneratePawnMoves<GenType::Attack>(pos, color, moves);

    moves.finish();
    return moves;
}
} // namespace Chess::MoveGen
