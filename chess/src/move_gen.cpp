#include <chess/internal/bit.hpp>
#include <chess/internal/bitboard.hpp>
#include <chess/internal/constants.hpp>
#include <chess/internal/utilities.hpp>
#include <chess/move_gen.hpp>
#include <functional>

namespace Chess::MoveGen {
namespace {
// HACK: This needs to be refactored
template <GenType gType> void GenerateKingMoves(const Position &pos, Color color, MoveList &moves) {
    const std::array<BB, 4> &blockSquares = CASTLING_BLOCK_SQUARES[static_cast<size_t>(color)];
    const std::array<BB, 4> &attackSquares = CASTLING_ATTACK_SQUARES[static_cast<size_t>(color)];

    const auto attackedSquares = pos.GenerateAttackSquares(~color);
    const Square kingPos = pos.GetKing(color);
    assert(kingPos != Square::None);
    if constexpr (gType == GenType::Quiet || gType == GenType::All) {
        BB qMoves = Ring(kingPos, 1) & ~pos.GetPieces() & ~attackedSquares;
        while (qMoves) {
            const Square move = Next(qMoves);
            if (pos.IsKingSafe((pos.GetPieces() ^ kingPos | move), pos.GetPieces(~color),
                               ToBB(move)))
                moves << Move(MoveType::Quiet, kingPos, move);
        }
        if (pos.AllowsCastling(Castling::King, color) &&
            !(pos.GetPieces() & blockSquares[static_cast<size_t>(Castling::King)]) &&
            !(attackedSquares & attackSquares[static_cast<size_t>(Castling::King)]))
            moves << Move(MoveType::KingCastle, kingPos,
                          First(Shift<Direction::East>(Shift<Direction::East>(ToBB(kingPos)))));
        if (pos.AllowsCastling(Castling::Queen, color) &&
            !(pos.GetPieces() & blockSquares[static_cast<size_t>(Castling::Queen)]) &&
            !(attackedSquares & attackSquares[static_cast<size_t>(Castling::Queen)]))
            moves << Move(MoveType::QueenCastle, kingPos,
                          First(Shift<Direction::West>(Shift<Direction::West>(ToBB(kingPos)))));
    }
    if constexpr (gType == GenType::Attack || gType == GenType::All) {
        BB aMoves = Ring(kingPos, 1) & pos.GetPieces(~color) & ~attackedSquares;
        while (aMoves) {
            const Square move = Next(aMoves);
            if (pos.IsKingSafe(pos.GetPieces() ^ kingPos, pos.GetPieces(~color) ^ move, ToBB(move)))
                moves << Move(MoveType::Capture, kingPos, move);
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
        if constexpr (gType == GenType::Quiet || gType == GenType::All) {
            Square to = static_cast<Square>(Bit::lsb(Ray(piece, dir) & Ring(piece, 1)));
            if (!(to & pos.GetPieces())) {
                if (pos.IsKingSafe(pos.GetPieces() ^ piece | to)) {
                    if (ToBB(piece) & PawnRow[static_cast<size_t>(~color)])
                        for (const auto prom : PromotionMoves)
                            moves << Move(prom, piece, to);
                    else
                        moves << Move(MoveType::Quiet, piece, to);
                }
                if (ToBB(piece) & PawnRow[static_cast<size_t>(color)]) {
                    to = First(Ray(piece, dir) & Ring(piece, 2));
                    if (!(to & pos.GetPieces()))
                        if (pos.IsKingSafe(pos.GetPieces() ^ piece | to))
                            moves << Move(MoveType::DoublePawnPush, piece, to);
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
                            moves << Move(prom, piece, attack);
                    else
                        moves << Move(MoveType::Capture, piece, attack);
                }
            }
            BB attack = PawnAttacks(piece, color) &
                        ((color == Color::White) ? Row::Row6 : Row::Row3) & pos.GetEP();
            if (attack) {
                const Square sq = Next(attack);
                const Square captured = First(Shift(ToBB(sq), dirs[static_cast<size_t>(~color)]));
                if (pos.IsKingSafe((pos.GetPieces() ^ piece ^ captured) | sq,
                                   pos.GetPieces(~color) ^ captured | sq))
                    moves << Move(MoveType::EPCapture, piece, sq);
            }
        }
    }
}

// Generates possible quiet moves for pieces of type pType
template <PieceType pType> void GenerateQuiet(const Position &pos, Color color, MoveList &moves) {
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

            while (blockers)
                unblocked = unblocked & ~Ray(piece, Next(blockers));

            while (potMoves) {
                const Square sq = Next(potMoves);
                if (pos.IsKingSafe((pos.GetPieces() ^ piece | sq)))
                    moves << Move(MoveType::Quiet, piece, sq);
            }
        }
    }
}

// Generates possible attack moves for pieces of type pType
template <PieceType pType> void GenerateAttack(const Position &pos, Color color, MoveList &moves) {
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
                    moves << Move(MoveType::Capture, static_cast<Square>(piece),
                                  static_cast<Square>(blocker));
            }
        }
    }
}
} // namespace

template <GenType gType, PieceType pType>
void Generate(const Position &pos, Color color, MoveList &moves) {
    if constexpr (gType == GenType::All) {
        if constexpr (pType == PieceType::King)
            GenerateKingMoves<GenType::All>(pos, color, moves);
        else if constexpr (pType == PieceType::Pawn)
            GeneratePawnMoves<GenType::All>(pos, color, moves);
        else {
            GenerateAttack<pType>(pos, color, moves);
            GenerateQuiet<pType>(pos, color, moves);
        }

    } else if constexpr (gType == GenType::Attack) {
        if constexpr (pType == PieceType::King)
            GenerateKingMoves<GenType::Attack>(pos, color, moves);
        else if constexpr (pType == PieceType::Pawn)
            GeneratePawnMoves<GenType::Attack>(pos, color, moves);
        else
            GenerateAttack<pType>(pos, color, moves);
    } else if constexpr (gType == GenType::Quiet) {
        if constexpr (pType == PieceType::King)
            GenerateKingMoves<GenType::Quiet>(pos, color, moves);
        else if constexpr (pType == PieceType::Pawn)
            GeneratePawnMoves<GenType::Quiet>(pos, color, moves);
        else
            GenerateQuiet<pType>(pos, color, moves);
    }
}
// clang-format off
template void Generate<GenType::All,    PieceType::Pawn>  (const Position &, Color, MoveList &moves);
template void Generate<GenType::All,    PieceType::Knight>(const Position &, Color, MoveList &moves);
template void Generate<GenType::All,    PieceType::Bishop>(const Position &, Color, MoveList &moves);
template void Generate<GenType::All,    PieceType::Rook>  (const Position &, Color, MoveList &moves);
template void Generate<GenType::All,    PieceType::Queen> (const Position &, Color, MoveList &moves);
template void Generate<GenType::All,    PieceType::King>  (const Position &, Color, MoveList &moves);
template void Generate<GenType::Quiet,  PieceType::Pawn>  (const Position &, Color, MoveList &moves);
template void Generate<GenType::Quiet,  PieceType::Knight>(const Position &, Color, MoveList &moves);
template void Generate<GenType::Quiet,  PieceType::Bishop>(const Position &, Color, MoveList &moves);
template void Generate<GenType::Quiet,  PieceType::Rook>  (const Position &, Color, MoveList &moves);
template void Generate<GenType::Quiet,  PieceType::Queen> (const Position &, Color, MoveList &moves);
template void Generate<GenType::Quiet,  PieceType::King>  (const Position &, Color, MoveList &moves);
template void Generate<GenType::Attack, PieceType::Pawn>  (const Position &, Color, MoveList &moves);
template void Generate<GenType::Attack, PieceType::Knight>(const Position &, Color, MoveList &moves);
template void Generate<GenType::Attack, PieceType::Bishop>(const Position &, Color, MoveList &moves);
template void Generate<GenType::Attack, PieceType::Rook>  (const Position &, Color, MoveList &moves);
template void Generate<GenType::Attack, PieceType::Queen> (const Position &, Color, MoveList &moves);
template void Generate<GenType::Attack, PieceType::King>  (const Position &, Color, MoveList &moves);
// clang-format on
} // namespace Chess::MoveGen
