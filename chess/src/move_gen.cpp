#include "chess/internal/bitboard.hpp"
#include "jank/bit/bit.hpp"
#include <chess/internal/constants.hpp>
#include <chess/internal/utilities.hpp>
#include <chess/move_gen.hpp>
#include <functional>

namespace Chess::MoveGen {
namespace {

// HACK: This needs to be refactored
template <GenType gType> void GenerateKingMoves(const Board &board, Color color, MoveList &moves) {
    constexpr static CastlingBlockSquares castlingBlock[2][2] = {
        {CastlingBlockSquares::KSideWhite, CastlingBlockSquares::QSideWhite},
        {CastlingBlockSquares::KSideBlack, CastlingBlockSquares::QSideBlack}};
    constexpr static CastlingAttackSquares castlingAttack[2][2] = {
        {CastlingAttackSquares::KSideWhite, CastlingAttackSquares::QSideWhite},
        {CastlingAttackSquares::KSideBlack, CastlingAttackSquares::QSideBlack}};
    const Color oppColor = Utilities::GetOppositeColor(color);
    const auto attackedSquares = board.GenerateAttackSquares(board.GetOppColor());
    const Square kingPos = (Square)jank::bit::lsb(board.GetPiecePos(color, PieceType::King));
    if constexpr (gType == GenType::Quiet || gType == GenType::All) {
        BB qMoves = Ring(kingPos, 1) & ~board.GetOccupiedBB() & ~attackedSquares;
        while (qMoves) {
            Square move = (Square)jank::bit::lsb_pop(qMoves);
            if (board.IsKingSafe((board.GetOccupiedBB() ^ kingPos | move),
                                 board.GetColorBB(oppColor), ToBB(move)))
                moves << Move(MoveType::Quiet, kingPos, move);
        }
        if (board.IsCastlingAllowed(color, Castling::King) &&
            !(board.GetOccupiedBB() & (BB)castlingBlock[(int)color][(int)Castling::King - 1]) &&
            !(attackedSquares & (BB)castlingAttack[(int)color][(int)Castling::King - 1]))
            moves << Move(MoveType::KingCastle, kingPos,
                          (Square)jank::bit::lsb(
                              Shift<Direction::East>(Shift<Direction::East>(ToBB(kingPos)))));
        if (board.IsCastlingAllowed(color, Castling::Queen) &&
            !(board.GetOccupiedBB() & (BB)castlingBlock[(int)color][(int)Castling::Queen - 1]) &&
            !(attackedSquares & (BB)castlingAttack[(int)color][(int)Castling::Queen - 1]))
            moves << Move(MoveType::QueenCastle, kingPos,
                          (Square)jank::bit::lsb(
                              Shift<Direction::West>(Shift<Direction::West>(ToBB(kingPos)))));
    }
    if constexpr (gType == GenType::Attack || gType == GenType::All) {
        BB aMoves = Ring(kingPos, 1) & board.GetColorBB(oppColor) & ~attackedSquares;
        while (aMoves) {
            Square move = (Square)jank::bit::lsb_pop(aMoves);
            if (board.IsKingSafe(board.GetOccupiedBB() ^ kingPos,
                                 board.GetColorBB(oppColor) ^ move, ToBB(move)))
                moves << Move(MoveType::Capture, kingPos, move);
        }
    }
}

// HACK: This needs to be refactored
template <GenType gType> void GeneratePawnMoves(const Board &board, Color color, MoveList &moves) {
    constexpr Direction dirs[2] = {Direction::North, Direction::South};
    Direction dir = dirs[(int)color];
    const Color oppColor = Utilities::GetOppositeColor(color);
    BB pieces = board.GetPiecePos(color, PieceType::Pawn);
    while (pieces) {
        Square piece = (Square)jank::bit::lsb_pop(pieces);
        if constexpr (gType == GenType::Quiet || gType == GenType::All) {
            BB to = Ray(piece, dir) & Ring(piece, 1);
            if (!(to & board.GetOccupiedBB())) {
                if (board.IsKingSafe(board.GetOccupiedBB() ^ piece | to)) {
                    if (ToBB(piece) & (BB)PawnRow[(int)oppColor])
                        for (const auto prom : PromotionMoves)
                            moves << Move(prom, piece, (Square)jank::bit::lsb(to));
                    else
                        moves << Move(MoveType::Quiet, piece, (Square)jank::bit::lsb(to));
                }
                to = Ray(piece, dir) & Ring(piece, 2);
                if (piece & (BB)PawnRow[(int)color] && !(to & board.GetOccupiedBB()))
                    if (board.IsKingSafe(board.GetOccupiedBB() ^ piece | to))
                        moves << Move(MoveType::DoublePawnPush, piece,
                                      (Square)jank::bit::lsb(to));
            }
        }
        if constexpr (gType == GenType::Attack || gType == GenType::All) {
            BB attacks = PawnAttacks(piece, color) & board.GetColorBB(oppColor);
            while (attacks) {
                Square attack = (Square)jank::bit::lsb_pop(attacks);
                assert(board.GetType((Square)attack) != PieceType::None);
                if (board.IsKingSafe((board.GetOccupiedBB() ^ piece) | attack,
                                     board.GetColorBB(oppColor) ^ attack)) {
                    if (piece & (BB)PawnRow[(int)oppColor])
                        for (const auto prom : PromotionCapturesMoves)
                            moves << Move(prom, piece, attack);
                    else
                        moves << Move(MoveType::Capture, piece, attack);
                }
            }
            BB attack = (BB)PawnAttacks(piece, color) &
                        (BB)((color == Color::White) ? Row::Row6 : Row::Row3) & (BB)board.GetEP();
            if (attack) {
                Square sq = (Square)jank::bit::lsb_pop(attack);
                Square captured = (Square)jank::bit::lsb(Shift(ToBB(sq), dirs[(int)oppColor]));
                if (board.IsKingSafe((board.GetOccupiedBB() ^ piece ^ captured) |
                                         sq,
                                     board.GetColorBB(oppColor) ^ captured | sq))
                    moves << Move(MoveType::EPCapture, piece, sq);
            }
        }
    }
}

// Generates possible quiet moves for pieces of type pType
template <PieceType pType> void GenerateQuiet(const Board &board, Color color, MoveList &moves) {
    static_assert(pType != PieceType::King && pType != PieceType::Pawn);

    BB pieces = board.GetPiecePos(color, pType);
    while (pieces) {
        Square piece = (Square)jank::bit::lsb_pop(pieces);
        BB unblocked = Attacks(piece, pType);
        for (int offset = 1; offset < 8; ++offset) {
            BB ring = Ring(piece, offset);
            BB potMoves = ring & unblocked;
            BB blockers = potMoves & board.GetOccupiedBB();
            potMoves ^= blockers;

            while (blockers)
                unblocked = unblocked & ~Ray(piece, (Square)jank::bit::lsb_pop(blockers));

            while (potMoves) {
                const Square sq = (Square)jank::bit::lsb_pop(potMoves);
                if (board.IsKingSafe((board.GetOccupiedBB() ^ piece | sq)))
                    moves << Move(MoveType::Quiet, piece, sq);
            }
        }
    }
};

// Generates possible attack moves for pieces of type pType
template <PieceType pType> void GenerateAttack(const Board &board, Color color, MoveList &moves) {
    static_assert(pType != PieceType::King && pType != PieceType::Pawn);
    Color oppColor = Utilities::GetOppositeColor(color);

    BB pieces = board.GetPiecePos(color, pType);
    while (pieces) {
        Square piece = (Square)jank::bit::lsb_pop(pieces);
        BB unblocked = Attacks(piece, pType);
        for (int offset = 1; offset < 8; ++offset) {
            BB ring = Ring(piece, offset);
            BB blockers = ring & unblocked & board.GetOccupiedBB();
            BB tempBlockers = blockers;
            while (tempBlockers)
                unblocked = unblocked & ~Ray(piece, (Square)jank::bit::lsb_pop(tempBlockers));
            blockers &= board.GetColorBB(oppColor);
            while (blockers) {
                const Square blocker = (Square)jank::bit::lsb_pop(blockers);
                if (board.IsKingSafe((board.GetOccupiedBB() ^ piece) | blocker,
                                     board.GetColorBB(oppColor) ^ blocker))
                    moves << Move(MoveType::Capture, (Square)piece, (Square)blocker);
            }
        }
    }
};
} // namespace

template <GenType gType, PieceType pType>
void Generate(const Board &board, Color color, MoveList &moves) {
    if constexpr (gType == GenType::All) {
        if constexpr (pType == PieceType::King)
            GenerateKingMoves<GenType::All>(board, color, moves);
        else if constexpr (pType == PieceType::Pawn)
            GeneratePawnMoves<GenType::All>(board, color, moves);
        else {
            GenerateAttack<pType>(board, color, moves);
            GenerateQuiet<pType>(board, color, moves);
        }

    } else if constexpr (gType == GenType::Attack) {
        if constexpr (pType == PieceType::King)
            GenerateKingMoves<GenType::Attack>(board, color, moves);
        else if constexpr (pType == PieceType::Pawn)
            GeneratePawnMoves<GenType::Attack>(board, color, moves);
        else
            GenerateAttack<pType>(board, color, moves);
    } else if constexpr (gType == GenType::Quiet) {
        if constexpr (pType == PieceType::King)
            GenerateKingMoves<GenType::Quiet>(board, color, moves);
        else if constexpr (pType == PieceType::Pawn)
            GeneratePawnMoves<GenType::Quiet>(board, color, moves);
        else
            GenerateQuiet<pType>(board, color, moves);
    }
}
// clang-format off
template void Generate<GenType::All, PieceType::Pawn>(const Board &, Color, MoveList &moves);
template void Generate<GenType::All, PieceType::Knight>(const Board &, Color, MoveList &moves);
template void Generate<GenType::All, PieceType::Bishop>(const Board &, Color, MoveList &moves);
template void Generate<GenType::All, PieceType::Rook>(const Board &, Color, MoveList &moves);
template void Generate<GenType::All, PieceType::Queen>(const Board &, Color, MoveList &moves);
template void Generate<GenType::All, PieceType::King>(const Board &, Color, MoveList &moves);
template void Generate<GenType::Quiet, PieceType::Pawn>(const Board &, Color, MoveList &moves);
template void Generate<GenType::Quiet, PieceType::Knight>(const Board &, Color, MoveList &moves);
template void Generate<GenType::Quiet, PieceType::Bishop>(const Board &, Color, MoveList &moves);
template void Generate<GenType::Quiet, PieceType::Rook>(const Board &, Color, MoveList &moves);
template void Generate<GenType::Quiet, PieceType::Queen>(const Board &, Color, MoveList &moves);
template void Generate<GenType::Quiet, PieceType::King>(const Board &, Color, MoveList &moves);
template void Generate<GenType::Attack, PieceType::Pawn>(const Board &, Color, MoveList &moves);
template void Generate<GenType::Attack, PieceType::Knight>(const Board &, Color, MoveList &moves);
template void Generate<GenType::Attack, PieceType::Bishop>(const Board &, Color, MoveList &moves);
template void Generate<GenType::Attack, PieceType::Rook>(const Board &, Color, MoveList &moves);
template void Generate<GenType::Attack, PieceType::Queen>(const Board &, Color, MoveList &moves);
template void Generate<GenType::Attack, PieceType::King>(const Board &, Color, MoveList &moves);
// clang-format on
}; // namespace Chess::MoveGen
