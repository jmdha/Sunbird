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
        {CastlingBlockSquares::KSideBlack, CastlingBlockSquares::QSideBlack}
    };
    constexpr static CastlingAttackSquares castlingAttack[2][2] = {
        {CastlingAttackSquares::KSideWhite, CastlingAttackSquares::QSideWhite},
        {CastlingAttackSquares::KSideBlack, CastlingAttackSquares::QSideBlack}
    };
    const Color oppColor = Utilities::GetOppositeColor(color);
    const auto attackedSquares = board.GenerateAttackSquares(board.GetOppColor());
    const U8 kingPos = jank::bit::lsb(board.GetPiecePos(color, PieceType::King));
    if constexpr (gType == GenType::Quiet || gType == GenType::All) {
        U64 qMoves = BitShift::RINGS[kingPos][1] & ~board.GetOccupiedBB() & ~attackedSquares;
        while (qMoves) {
            U8 move = jank::bit::lsb_pop(qMoves);
            if (board.IsKingSafe((board.GetOccupiedBB() ^ C64(kingPos) | C64(move)),
                                 board.GetColorBB(oppColor), C64(move)))
                moves << Move(MoveType::Quiet, (Square)kingPos, (Square)move);
        }
        if (board.IsCastlingAllowed(color, Castling::King) &&
            !(board.GetOccupiedBB() & (U64)castlingBlock[(int)color][(int)Castling::King]) &&
            !(attackedSquares & (U64)castlingAttack[(int)color][(int)Castling::King]))
            moves << Move(
                MoveType::KingCastle, (Square)kingPos,
                (Square)jank::bit::lsb(BitShift::Shift(C64(kingPos), Direction::East, 2)));
        if (board.IsCastlingAllowed(color, Castling::Queen) &&
            !(board.GetOccupiedBB() & (U64)castlingBlock[(int)color][(int)Castling::Queen]) &&
            !(attackedSquares & (U64)castlingAttack[(int)color][(int)Castling::Queen]))
            moves << Move(
                MoveType::QueenCastle, (Square)kingPos,
                (Square)jank::bit::lsb(BitShift::Shift(C64(kingPos), Direction::West, 2)));
    }
    if constexpr (gType == GenType::Attack || gType == GenType::All) {
        U64 aMoves =
            BitShift::RINGS[kingPos][1] & board.GetColorBB(oppColor) & ~attackedSquares;
        while (aMoves) {
            U8 move = jank::bit::lsb_pop(aMoves);
            if (board.IsKingSafe(board.GetOccupiedBB() ^ C64(kingPos),
                                 board.GetColorBB(oppColor) ^ C64(move), C64(move)))
                moves << Move(MoveType::Capture, (Square)kingPos, (Square)move);
        }
    }
}

// HACK: This needs to be refactored
template <GenType gType> void GeneratePawnMoves(const Board &board, Color color, MoveList &moves) {
    constexpr DirectionIndex dir[2] = {DirectionIndex::North, DirectionIndex::South};
    const Color oppColor = Utilities::GetOppositeColor(color);
    U64 pieces = board.GetPiecePos(color, PieceType::Pawn);
    while (pieces) {
        int piece = jank::bit::lsb_pop(pieces);
        if constexpr (gType == GenType::Quiet || gType == GenType::All) {
            U64 to = BitShift::RAYS[(U8)piece][(U8)dir[(U8)color]] &
                     BitShift::RINGS[(U8)piece][1];
            if (!(to & board.GetOccupiedBB())) {
                if (board.IsKingSafe(board.GetOccupiedBB() ^ C64(piece) | to)) {
                    if (C64(piece) & (U64)PawnRow[(U8)oppColor])
                        for (const auto prom : PromotionMoves)
                            moves << Move(
                                prom, (Square)piece,
                                (Square)jank::bit::lsb(BitShift::Shift(
                                    C64(piece), DIRECTIONS[(U8)dir[(U8)color]])));
                    else
                        moves << Move(MoveType::Quiet, (Square)piece,
                                      (Square)jank::bit::lsb(BitShift::Shift(
                                          C64(piece), DIRECTIONS[(U8)dir[(U8)color]])));
                }
                to = BitShift::RAYS[(U8)piece][(U8)dir[(U8)color]] &
                     BitShift::RINGS[(U8)piece][2];
                if (C64(piece) & (U64)PawnRow[(U8)color] &&
                    !(to & board.GetOccupiedBB()))
                    if (board.IsKingSafe(board.GetOccupiedBB() ^ C64(piece) | to))
                        moves << Move(
                            MoveType::DoublePawnPush, (Square)piece,
                            (Square)jank::bit::lsb(BitShift::Shift(
                                C64(piece), DIRECTIONS[(U8)dir[(U8)color]], 2)));
            }
        }
        if constexpr (gType == GenType::Attack || gType == GenType::All) {
            U64 attacks =
                PawnAttacks[(U8)board.GetColor()][piece] & board.GetColorBB(oppColor);
            while (attacks) {
                U8 attack = jank::bit::lsb_pop(attacks);
                assert(board.GetType((Square)attack) != PieceType::None);
                if (board.IsKingSafe((board.GetOccupiedBB() ^ C64(piece)) | C64(attack),
                                     board.GetColorBB(oppColor) ^ C64(attack))) {
                    if (C64(piece) & (U64)PawnRow[(U8)oppColor])
                        for (const auto prom : PromotionCapturesMoves)
                            moves << Move(prom, (Square)piece, (Square)attack);
                    else
                        moves << Move(MoveType::Capture, (Square)piece, (Square)attack);
                }
            }
            U64 attack = (U64)PawnAttacks[(U8)color][piece] &
                         (U64)((color == Color::White) ? Row::Row6 : Row::Row3) &
                         (U64)board.GetEP();
            if (attack) {
                U8 sq = jank::bit::lsb_pop(attack);
                U8 captured = jank::bit::lsb(
                    BitShift::Shift(C64(sq), DIRECTIONS[(U8)dir[(U8)oppColor]]));
                if (board.IsKingSafe((board.GetOccupiedBB() ^ C64(piece) ^ C64(captured)) | C64(sq),
                                     board.GetColorBB(oppColor) ^ C64(captured) |
                                         C64(sq)))
                    moves << Move(MoveType::EPCapture, (Square)piece, (Square)sq);
            }
        }
    }
}

// Generates possible quiet moves for pieces of type pType
template <PieceType pType> void GenerateQuiet(const Board &board, Color color, MoveList &moves) {
    static_assert(pType != PieceType::King && pType != PieceType::Pawn);

    U64 pieces = board.GetPiecePos(color, pType);
    while(pieces) {
        int piece = jank::bit::lsb_pop(pieces);
        U64 unblocked = BitShift::MOVES[(int)pType][piece];
        for (U8 offset = 1; offset < 8; ++offset) {
            U64 ring = BitShift::RINGS[piece][offset];
            U64 potMoves = ring & unblocked;
            U64 blockers = potMoves & board.GetOccupiedBB();
            potMoves ^= blockers;

            while (blockers)
                unblocked = unblocked & ~BitShift::SQ_RAYS[piece][jank::bit::lsb_pop(blockers)];

            while (potMoves) {
                const U8 sq = jank::bit::lsb_pop(potMoves);
                if (board.IsKingSafe((board.GetOccupiedBB() ^ C64(piece) | C64(sq))))
                    moves << Move(MoveType::Quiet, (Square)piece, (Square)sq);
            }
        }
    }
};

// Generates possible attack moves for pieces of type pType
template <PieceType pType> void GenerateAttack(const Board &board, Color color, MoveList &moves) {
    static_assert(pType != PieceType::King && pType != PieceType::Pawn);
    Color oppColor = Utilities::GetOppositeColor(color);

    U64 pieces = board.GetPiecePos(color, pType);
    while (pieces) {
        int piece = jank::bit::lsb_pop(pieces);
        U64 unblocked = BitShift::MOVES[(int)pType][piece];
        for (U8 offset = 1; offset < 8; ++offset) {
            U64 ring = BitShift::RINGS[piece][offset];
            U64 blockers = ring & unblocked & board.GetOccupiedBB();
            U64 tempBlockers = blockers;
            while (tempBlockers)
                unblocked =
                    unblocked & ~BitShift::SQ_RAYS[piece][jank::bit::lsb_pop(tempBlockers)];
            blockers &= board.GetColorBB(oppColor);
            while (blockers) {
                const U64 blocker = jank::bit::lsb_pop(blockers);
                if (board.IsKingSafe((board.GetOccupiedBB() ^ C64(piece)) | C64(blocker),
                                     board.GetColorBB(oppColor) ^ C64(blocker)))
                    moves << Move(MoveType::Capture, (Square)piece, (Square)blocker);
            }
        }
    }
};
} // namespace

template <GenType gType, PieceType pType> void Generate(const Board &board, Color color, MoveList &moves) {
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
}; // namespace MoveGen
