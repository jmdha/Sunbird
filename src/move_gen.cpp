#include "move_gen.hpp"
#include "bit.hpp"
#include "bitboard.hpp"
#include "move_list.hpp"
#include "types.hpp"
#include "utilities.hpp"

namespace MoveGen {
typedef BB (*AttackFunc)(Square);
enum class GenType { Attack, All };
template <MoveList::Type t>
void BuildMoves(MoveList &moves, Square sq, BB targets, MoveType move_type) {
    while (targets)
        moves.push<t>(Move(move_type, sq, (Square)lsb_pop(targets)));
}
// HACK: This needs to be refactored
template <GenType gType>
void GeneratePawnMoves(const Position &pos, Color color, MoveList &moves) {
    constexpr Direction dirs[2] = {Direction::North, Direction::South};
    Direction dir               = dirs[static_cast<size_t>(color)];
    BB pieces                   = pos.GetPieces(color, PieceType::Pawn);
    while (pieces) {
        const Square piece = Next(pieces);
        if constexpr (gType == GenType::All) {
            Square to = static_cast<Square>(lsb(Ray(piece, dir) & Ring(piece, 1)));
            if (!(to & pos.GetPieces())) {
                if (ToBB(piece) & PawnRow[static_cast<size_t>(~color)])
                    for (const auto prom : PromotionMoves)
                        moves.push<MoveList::Quiet>(Move(prom, piece, to));
                else
                    moves.push<MoveList::Quiet>(Move(MoveType::Quiet, piece, to));
                if (ToBB(piece) & PawnRow[static_cast<size_t>(color)]) {
                    to = First(Ray(piece, dir) & Ring(piece, 2));
                    if (!(to & pos.GetPieces()))
                        moves.push<MoveList::Quiet>(Move(MoveType::DoublePawnPush, piece, to));
                }
            }
        }
        if constexpr (gType == GenType::Attack || gType == GenType::All) {
            BB attacks = PawnAttacks(piece, color) & pos.GetPieces(~color);
            while (attacks) {
                const Square attack = Next(attacks);
                assert(pos.GetType((Square)attack) != PieceType::None);
                if (piece & PawnRow[static_cast<size_t>(~color)])
                    for (const auto prom : PromotionCapturesMoves)
                        moves.push<MoveList::Attack>(Move(prom, piece, attack));
                else
                    moves.push<MoveList::Attack>(Move(MoveType::Capture, piece, attack));
            }
            BB attack = PawnAttacks(piece, color) & ((color == WHITE) ? Row::Row6 : Row::Row3) &
                        pos.GetEP();
            if (attack) {
                const Square sq = Next(attack);
                moves.push<MoveList::Attack>(Move(MoveType::EPCapture, piece, sq));
            }
        }
    }
}

void SliderAttack(MoveList &moves, AttackFunc F, BB pieces, BB occ, BB nus) {
    while (pieces) {
        const Square piece = Next(pieces);
        BB unblocked       = F(piece);
        for (size_t offset = 1; offset < 8; ++offset) {
            BB ring     = Ring(piece, offset);
            BB blockers = ring & unblocked & occ;

            BuildMoves<MoveList::Attack>(moves, piece, blockers & nus, MoveType::Capture);

            while (blockers)
                unblocked &= ~Ray(piece, (Square)lsb_pop(blockers));
        }
    }
}

void SliderAll(MoveList &moves, AttackFunc F, BB pieces, BB occ, BB nus) {
    while (pieces) {
        const Square piece = Next(pieces);
        BB unblocked       = F(piece);
        for (size_t offset = 1; offset < 8; ++offset) {
            BB ring          = Ring(piece, offset);
            BB pot_moves     = ring & unblocked;
            BB quiet_moves   = pot_moves & (~occ);
            BB capture_moves = pot_moves & nus;

            BuildMoves<MoveList::Quiet>(moves, piece, quiet_moves, MoveType::Quiet);
            BuildMoves<MoveList::Attack>(moves, piece, capture_moves, MoveType::Capture);

            BB blockers = pot_moves & occ;
            while (blockers)
                unblocked &= ~Ray(piece, (Square)lsb_pop(blockers));
        }
    }
}

template <MoveList::Type t>
void BuildJumperMoves(MoveList &moves, AttackFunc F, BB pieces, BB targets, MoveType move_type) {
    while (pieces) {
        Square sq = (Square)lsb_pop(pieces);
        BuildMoves<t>(moves, sq, F(sq) & targets, move_type);
    }
}

void GenerateCastlingMoves(MoveList &moves, Castling castling, Color turn, BB occ, BB attacks) {
    static const Square KING_POS[2]         = {Square::E1, Square::E8};
    static const Square KING_CASTLE_POS[2]  = {Square::G1, Square::G8};
    static const Square QUEEN_CASTLE_POS[2] = {Square::C1, Square::C8};
    static const BB KING_BLOCKERS[2]        = {0x60lu, 0x6000000000000000lu};
    static const BB QUEEN_BLOCKERS[2]       = {0xelu, 0xe00000000000000lu};
    static const BB QUEEN_ATTACKERS[2]      = {0xclu, 0xc00000000000000lu};

    if ((bool)(castling & Castling::King) && !(occ & KING_BLOCKERS[turn]) &&
        !(attacks & KING_BLOCKERS[turn]))
        moves.push<MoveList::Quiet>(
            Move(MoveType::KingCastle, KING_POS[turn], KING_CASTLE_POS[turn])
        );
    if ((bool)(castling & Castling::Queen) && !(occ & QUEEN_BLOCKERS[turn]) &&
        !(attacks & QUEEN_ATTACKERS[turn]))
        moves.push<MoveList::Quiet>(
            Move(MoveType::QueenCastle, KING_POS[turn], QUEEN_CASTLE_POS[turn])
        );
}

MoveList GenerateAll(const Position &pos, Color color) {
    MoveList moves;

    const BB us    = pos.GetPieces(color);
    const BB nus   = pos.GetPieces(~color);
    const BB occ   = pos.GetPieces();
    const BB empty = ~occ;

    const BB pawns   = pos.GetPieces(color, PieceType::Pawn);
    const BB knights = pos.GetPieces(color, PieceType::Knight);
    const BB kings   = pos.GetPieces(color, PieceType::King);
    const BB queens  = pos.GetPieces(color, PieceType::Queen);
    const BB bishops = pos.GetPieces(color, PieceType::Bishop) | queens;
    const BB rooks   = pos.GetPieces(color, PieceType::Rook) | queens;

    GeneratePawnMoves<GenType::All>(pos, color, moves);

    SliderAll(moves, BishopAttacks, bishops, occ, nus);
    SliderAll(moves, RookAttacks, rooks, occ, nus);

    BuildJumperMoves<MoveList::Attack>(moves, KnightAttacks, knights, nus, MoveType::Capture);
    BuildJumperMoves<MoveList::Quiet>(moves, KnightAttacks, knights, empty, MoveType::Quiet);

    BB attacks = pos.GenerateAttackSquares(~color);
    BuildJumperMoves<MoveList::Attack>(
        moves, KingAttacks, kings, nus & (~attacks), MoveType::Capture
    );
    BuildJumperMoves<MoveList::Quiet>(
        moves, KingAttacks, kings, empty & (~attacks), MoveType::Quiet
    );

    if (!(attacks & kings))
        GenerateCastlingMoves(moves, pos.GetCastling(color), color, occ, attacks);

    moves.finish();
    return moves;
}

MoveList GenerateAttack(const Position &pos, Color color) {
    MoveList moves;

    const BB us    = pos.GetPieces(color);
    const BB nus   = pos.GetPieces(~color);
    const BB occ   = pos.GetPieces();
    const BB empty = ~occ;

    const BB pawns   = pos.GetPieces(color, PieceType::Pawn);
    const BB knights = pos.GetPieces(color, PieceType::Knight);
    const BB kings   = pos.GetPieces(color, PieceType::King);
    const BB queens  = pos.GetPieces(color, PieceType::Queen);
    const BB bishops = pos.GetPieces(color, PieceType::Bishop) | queens;
    const BB rooks   = pos.GetPieces(color, PieceType::Rook) | queens;

    GeneratePawnMoves<GenType::Attack>(pos, color, moves);

    SliderAttack(moves, BishopAttacks, bishops, occ, nus);
    SliderAttack(moves, RookAttacks, rooks, occ, nus);

    BuildJumperMoves<MoveList::Attack>(moves, KnightAttacks, knights, nus, MoveType::Capture);
    BB attacks = pos.GenerateAttackSquares(~color);
    BuildJumperMoves<MoveList::Attack>(
        moves, KingAttacks, kings, nus & (~attacks), MoveType::Capture
    );

    return moves;
}
} // namespace MoveGen
