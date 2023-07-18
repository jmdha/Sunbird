#include "chess/internal/constants.hpp"
#include "chess/internal/utilities.hpp"
#include <chess/move_gen.hpp>
#include <functional>

namespace MoveGen {
namespace {

// Calls the callback on on all pieces of type pType of own colour
template <PieceType pType> void PieceIter(const Board &board, std::function<void(U8)> callback) {
    U64 pieces = board.GetPiecePos(board.GetColor(), pType);
    while (pieces)
        callback(Utilities::LSB_Pop(&pieces));
}

// HACK: This needs to be refactored
template <GenType gType> MoveList GenerateKingMoves(const Board &board) {
    MoveList moves;

    const auto color = board.GetColor();
    const auto attackedSquares = board.GenerateAttackSquares(board.GetOppColor());
    const CastlingBlockSquares castlingBlock[2] = {
        (color == Color::White) ? CastlingBlockSquares::KSideWhite
                                : CastlingBlockSquares::KSideBlack,
        (color == Color::White) ? CastlingBlockSquares::QSideWhite
                                : CastlingBlockSquares::QSideBlack};
    const CastlingAttackSquares castlingAttack[2] = {
        (color == Color::White) ? CastlingAttackSquares::KSideWhite
                                : CastlingAttackSquares::KSideBlack,
        (color == Color::White) ? CastlingAttackSquares::QSideWhite
                                : CastlingAttackSquares::QSideBlack};
    const U8 kingPos = Utilities::LSB(board.GetPiecePos(color, PieceType::King));
    if constexpr (gType == GenType::Quiet || gType == GenType::All) {
        U64 qMoves = BitShift::RINGS[kingPos][1] & ~board.GetOccupiedBB() & ~attackedSquares;
        while (qMoves) {
            U8 move = Utilities::LSB_Pop(&qMoves);
            if (board.IsKingSafe((board.GetOccupiedBB() ^ C64(kingPos) | C64(move)),
                                 board.GetColorBB(board.GetOppColor()), C64(move)))
                moves << Move(MoveType::Quiet, (Square)kingPos, (Square)move);
        }
        if (board.IsCastlingAllowed(color, Castling::King) &&
            !(board.GetOccupiedBB() & (U64)castlingBlock[(int)Castling::King]) &&
            !(attackedSquares & (U64)castlingAttack[(int)Castling::King]))
            moves << Move(
                MoveType::KingCastle, (Square)kingPos,
                (Square)Utilities::LSB(BitShift::Shift(C64(kingPos), Direction::East, 2)));
        if (board.IsCastlingAllowed(color, Castling::Queen) &&
            !(board.GetOccupiedBB() & (U64)castlingBlock[(int)Castling::Queen]) &&
            !(attackedSquares & (U64)castlingAttack[(int)Castling::Queen]))
            moves << Move(
                MoveType::QueenCastle, (Square)kingPos,
                (Square)Utilities::LSB(BitShift::Shift(C64(kingPos), Direction::West, 2)));
    }
    if constexpr (gType == GenType::Attack || gType == GenType::All) {
        U64 aMoves =
            BitShift::RINGS[kingPos][1] & board.GetColorBB(board.GetOppColor()) & ~attackedSquares;
        while (aMoves) {
            U8 move = Utilities::LSB_Pop(&aMoves);
            if (board.IsKingSafe(board.GetOccupiedBB() ^ C64(kingPos),
                                 board.GetColorBB(board.GetOppColor()) ^ C64(move), C64(move)))
                moves << Move(MoveType::Capture, (Square)kingPos, (Square)move,
                              board.GetType((Square)move));
        }
    }

    return moves;
}

// HACK: This needs to be refactored
template <GenType gType> MoveList GeneratePawnMoves(const Board &board) {
    constexpr DirectionIndex dir[2] = {DirectionIndex::North, DirectionIndex::South};
    MoveList moves;
    PieceIter<PieceType::Pawn>(board, [&](U8 piece) {
        if constexpr (gType == GenType::Quiet || gType == GenType::All) {
            U64 to = BitShift::RAYS[(U8)piece][(U8)dir[(U8)board.GetColor()]] & BitShift::RINGS[(U8)piece][1];
            if (!(to & board.GetOccupiedBB())) {
                if (board.IsKingSafe(board.GetOccupiedBB() ^ C64(piece) | to)) {
                    if (C64(piece) & (U64)PawnRow[(U8)board.GetOppColor()])
                        for (const auto prom : PromotionMoves)
                            moves << Move(prom, (Square)piece,
                                          (Square)Utilities::LSB(
                                              BitShift::Shift(C64(piece), DIRECTIONS[(U8)dir[(U8)board.GetColor()]])));
                    else
                        moves << Move(MoveType::Quiet, (Square)piece,
                                      (Square)Utilities::LSB(
                                          BitShift::Shift(C64(piece), DIRECTIONS[(U8)dir[(U8)board.GetColor()]])));
                }
                to = BitShift::RAYS[(U8)piece][(U8)dir[(U8)board.GetColor()]] & BitShift::RINGS[(U8)piece][2];
                if (C64(piece) & (U64)PawnRow[(U8)board.GetColor()] &&
                    !(to & board.GetOccupiedBB()))
                    if (board.IsKingSafe(board.GetOccupiedBB() ^ C64(piece) | to))
                        moves << Move(MoveType::DoublePawnPush, (Square)piece,
                                      (Square)Utilities::LSB(
                                          BitShift::Shift(C64(piece), DIRECTIONS[(U8)dir[(U8)board.GetColor()]], 2)));
            }
        }
        if constexpr (gType == GenType::Attack || gType == GenType::All) {
            U64 attacks =
                PawnAttacks[(U8)board.GetColor()][piece] & board.GetColorBB(board.GetOppColor());
            while (attacks) {
                U8 attack = Utilities::LSB_Pop(&attacks);
                assert(board.GetType((Square)attack) != PieceType::None);
                if (board.IsKingSafe((board.GetOccupiedBB() ^ C64(piece)) | C64(attack),
                                     board.GetColorBB(board.GetOppColor()) ^ C64(attack))) {
                    if (C64(piece) & (U64)PawnRow[(U8)board.GetOppColor()])
                        for (const auto prom : PromotionCapturesMoves)
                            moves << Move(prom, (Square)piece, (Square)attack,
                                          board.GetType((Square)attack));
                    else
                        moves << Move(MoveType::Capture, (Square)piece, (Square)attack,
                                      board.GetType((Square)attack));
                }
            }
            U64 attack = (U64)PawnAttacks[(U8)board.GetColor()][piece] &
                         (U64)((board.GetColor() == Color::White) ? Row::Row6 : Row::Row3) &
                         (U64)board.GetEP();
            if (attack) {
                U8 sq = Utilities::LSB_Pop(&attack);
                U8 captured = Utilities::LSB(BitShift::Shift(C64(sq), DIRECTIONS[(U8)dir[(U8)board.GetOppColor()]]));
                if (board.IsKingSafe((board.GetOccupiedBB() ^ C64(piece) ^ C64(captured)) | C64(sq),  board.GetColorBB(board.GetOppColor()) ^ C64(captured) | C64(sq)))
                    moves << Move(MoveType::EPCapture, (Square)piece, (Square)sq,
                                  PieceType::Pawn);
            }
        }
    });
    return moves;
}

// Generates possible quiet moves for pieces of type pType
template <PieceType pType> MoveList GenerateQuiet(const Board &board) {
    static_assert(pType != PieceType::King && pType != PieceType::Pawn);
    MoveList moves;

    PieceIter<pType>(board, [&](U8 piece) {
        U64 unblocked = BitShift::MOVES[(int)pType][piece];
        for (U8 offset = 1; offset < 8; ++offset) {
            U64 ring = BitShift::RINGS[piece][offset];
            U64 potMoves = ring & unblocked;
            U64 blockers = potMoves & board.GetOccupiedBB();
            potMoves ^= blockers;

            while (blockers)
                unblocked = unblocked & ~BitShift::SQ_RAYS[piece][Utilities::LSB_Pop(&blockers)];

            while (potMoves) {
                const U8 sq = Utilities::LSB_Pop(&potMoves);
                if (board.IsKingSafe((board.GetOccupiedBB() ^ C64(piece) | C64(sq))))
                    moves << Move(MoveType::Quiet, (Square)piece, (Square)sq);
            }
        }
    });

    return moves;
};

// Generates possible attack moves for pieces of type pType
template <PieceType pType> MoveList GenerateAttack(const Board &board) {
    static_assert(pType != PieceType::King && pType != PieceType::Pawn);
    MoveList moves;

    PieceIter<pType>(board, [board, &moves](U8 piece) {
        U64 unblocked = BitShift::MOVES[(int)pType][piece];
        for (U8 offset = 1; offset < 8; ++offset) {
            U64 ring = BitShift::RINGS[piece][offset];
            U64 blockers = ring & unblocked & board.GetOccupiedBB();
            U64 tempBlockers = blockers;
            while (tempBlockers)
                unblocked =
                    unblocked & ~BitShift::SQ_RAYS[piece][Utilities::LSB_Pop(&tempBlockers)];
            blockers &= board.GetColorBB(board.GetOppColor());
            while (blockers) {
                const U64 blocker = Utilities::LSB_Pop(&blockers);
                if (board.IsKingSafe((board.GetOccupiedBB() ^ C64(piece)) | C64(blocker),
                                     board.GetColorBB(board.GetOppColor()) ^ C64(blocker)))
                    moves << Move(MoveType::Capture, (Square)piece, (Square)blocker,
                                  board.GetType((Square)blocker));
            }
        }
    });

    return moves;
};
} // namespace

template <GenType gType, PieceType pType> MoveList Generate(const Board &board) {
    MoveList moves;

    if constexpr (gType == GenType::Attack || gType == GenType::All) {
        if constexpr (pType == PieceType::King) {
            moves << GenerateKingMoves<GenType::Attack>(board);
        } else if constexpr (pType == PieceType::Pawn) {
            moves << GeneratePawnMoves<GenType::Attack>(board);
        } else
            moves << GenerateAttack<pType>(board);
    }

    if constexpr (gType == GenType::Quiet || gType == GenType::All) {
        if constexpr (pType == PieceType::King) {
            moves << GenerateKingMoves<GenType::Quiet>(board);
        } else if constexpr (pType == PieceType::Pawn) {
            moves << GeneratePawnMoves<GenType::Quiet>(board);
        } else
            moves << GenerateQuiet<pType>(board);
    }

    return moves;
}
// clang-format off
template MoveList Generate<GenType::All, PieceType::Pawn>(const Board &);
template MoveList Generate<GenType::All, PieceType::Knight>(const Board &);
template MoveList Generate<GenType::All, PieceType::Bishop>(const Board &);
template MoveList Generate<GenType::All, PieceType::Rook>(const Board &);
template MoveList Generate<GenType::All, PieceType::Queen>(const Board &);
template MoveList Generate<GenType::All, PieceType::King>(const Board &);
template MoveList Generate<GenType::Quiet, PieceType::Pawn>(const Board &);
template MoveList Generate<GenType::Quiet, PieceType::Knight>(const Board &);
template MoveList Generate<GenType::Quiet, PieceType::Bishop>(const Board &);
template MoveList Generate<GenType::Quiet, PieceType::Rook>(const Board &);
template MoveList Generate<GenType::Quiet, PieceType::Queen>(const Board &);
template MoveList Generate<GenType::Quiet, PieceType::King>(const Board &);
template MoveList Generate<GenType::Attack, PieceType::Pawn>(const Board &);
template MoveList Generate<GenType::Attack, PieceType::Knight>(const Board &);
template MoveList Generate<GenType::Attack, PieceType::Bishop>(const Board &);
template MoveList Generate<GenType::Attack, PieceType::Rook>(const Board &);
template MoveList Generate<GenType::Attack, PieceType::Queen>(const Board &);
template MoveList Generate<GenType::Attack, PieceType::King>(const Board &);
// clang-format on
}; // namespace MoveGen
