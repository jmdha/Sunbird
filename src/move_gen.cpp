#include "move_gen.hpp"
#include "bit.hpp"
#include "bitboard.hpp"
#include "move_list.hpp"
#include "types.hpp"

typedef BB (*AttackFunc)(Square);
enum class GenType { Attack, All };
template <MoveList::Type t>
void BuildMoves(MoveList &moves, Square sq, BB targets, Move::Type move_type) {
    while (targets)
        moves.push<t>(Move(sq, (Square)lsb_pop(targets), move_type));
}
// HACK: This needs to be refactored
template <GenType gType>
void GeneratePawnMoves(const Board &board, Color color, MoveList &moves) {
    constexpr Direction dirs[2] = {NORTH, SOUTH};
    Direction dir               = dirs[color];
    BB pieces                   = board.Pieces(color, PAWN);
    while (pieces) {
        const Square piece = lsb_pop(pieces);
        if constexpr (gType == GenType::All) {
            Square to = static_cast<Square>(lsb(Ray(piece, dir) & Ring(piece, 1)));
            if (!(to & board.Pieces())) {
                if (ToBB(piece) & PawnRow[static_cast<size_t>(~color)]) {
                    moves.push<MoveList::Quiet>(Move(piece, to, Move::NPromotion));
                    moves.push<MoveList::Quiet>(Move(piece, to, Move::BPromotion));
                    moves.push<MoveList::Quiet>(Move(piece, to, Move::RPromotion));
                    moves.push<MoveList::Quiet>(Move(piece, to, Move::QPromotion));
                } else
                    moves.push<MoveList::Quiet>(Move(piece, to, Move::Quiet));
                if (ToBB(piece) & PawnRow[static_cast<size_t>(color)]) {
                    to = lsb(Ray(piece, dir) & Ring(piece, 2));
                    if (!(to & board.Pieces()))
                        moves.push<MoveList::Quiet>(Move(piece, to, Move::DoublePawnPush));
                }
            }
        }
        if constexpr (gType == GenType::Attack || gType == GenType::All) {
            BB attacks = PawnAttacks(piece, color) & board.Pieces(~color);
            while (attacks) {
                const Square attack = lsb_pop(attacks);
                assert(board.SquarePiece((Square)attack) != PIECE_NONE);
                if (piece & PawnRow[static_cast<size_t>(~color)]) {
                    moves.push<MoveList::Attack>(Move(piece, attack, Move::NPromotionCapture));
                    moves.push<MoveList::Attack>(Move(piece, attack, Move::BPromotionCapture));
                    moves.push<MoveList::Attack>(Move(piece, attack, Move::RPromotionCapture));
                    moves.push<MoveList::Attack>(Move(piece, attack, Move::QPromotionCapture));
                } else
                    moves.push<MoveList::Attack>(Move(piece, attack, Move::Capture));
            }
            if (const Square sq = board.EP(); sq != SQUARE_NONE && PawnAttacks(piece, color) & sq)
                moves.push<MoveList::Attack>(Move(piece, sq, Move::EPCapture));
        }
    }
}

void SliderAttack(MoveList &moves, AttackFunc F, BB pieces, BB occ, BB nus) {
    while (pieces) {
        const Square piece = lsb_pop(pieces);
        BB unblocked       = F(piece);
        for (size_t offset = 1; offset < 8; ++offset) {
            BB ring     = Ring(piece, offset);
            BB blockers = ring & unblocked & occ;

            BuildMoves<MoveList::Attack>(moves, piece, blockers & nus, Move::Capture);

            while (blockers)
                unblocked &= ~Ray(piece, (Square)lsb_pop(blockers));
        }
    }
}

void SliderAll(MoveList &moves, AttackFunc F, BB pieces, BB occ, BB nus) {
    while (pieces) {
        const Square piece = lsb_pop(pieces);
        BB unblocked       = F(piece);
        for (size_t offset = 1; offset < 8; ++offset) {
            BB ring          = Ring(piece, offset);
            BB pot_moves     = ring & unblocked;
            BB quiet_moves   = pot_moves & (~occ);
            BB capture_moves = pot_moves & nus;

            BuildMoves<MoveList::Quiet>(moves, piece, quiet_moves, Move::Quiet);
            BuildMoves<MoveList::Attack>(moves, piece, capture_moves, Move::Capture);

            BB blockers = pot_moves & occ;
            while (blockers)
                unblocked &= ~Ray(piece, (Square)lsb_pop(blockers));
        }
    }
}

template <MoveList::Type t>
void BuildJumperMoves(MoveList &moves, AttackFunc F, BB pieces, BB targets, Move::Type move_type) {
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
        moves.push<MoveList::Quiet>(Move(KING_POS[turn], KING_CASTLE_POS[turn], Move::KingCastle));
    if ((bool)(castling & Castling::Queen) && !(occ & QUEEN_BLOCKERS[turn]) &&
        !(attacks & QUEEN_ATTACKERS[turn]))
        moves.push<MoveList::Quiet>(Move(KING_POS[turn], QUEEN_CASTLE_POS[turn], Move::QueenCastle)
        );
}

MoveList GenerateMovesAll(const Board &board, Color color) {
    MoveList moves;

    const BB nus   = board.Pieces(~color);
    const BB occ   = board.Pieces();
    const BB empty = ~occ;

    const BB knights = board.Pieces(color, KNIGHT);
    const BB kings   = board.Pieces(color, KING);
    const BB queens  = board.Pieces(color, QUEEN);
    const BB bishops = board.Pieces(color, BISHOP) | queens;
    const BB rooks   = board.Pieces(color, ROOK) | queens;

    GeneratePawnMoves<GenType::All>(board, color, moves);

    SliderAll(moves, BishopAttacks, bishops, occ, nus);
    SliderAll(moves, RookAttacks, rooks, occ, nus);

    BuildJumperMoves<MoveList::Attack>(moves, KnightAttacks, knights, nus, Move::Capture);
    BuildJumperMoves<MoveList::Quiet>(moves, KnightAttacks, knights, empty, Move::Quiet);

    BB attacks = board.GenerateAttacks(~color);
    BuildJumperMoves<MoveList::Attack>(moves, KingAttacks, kings, nus & (~attacks), Move::Capture);
    BuildJumperMoves<MoveList::Quiet>(moves, KingAttacks, kings, empty & (~attacks), Move::Quiet);

    if (!(attacks & kings))
        GenerateCastlingMoves(moves, board.GetCastling(color), color, occ, attacks);

    moves.finish();
    return moves;
}

MoveList GenerateMovesTactical(const Board &board, Color color) {
    MoveList moves;

    const BB nus = board.Pieces(~color);
    const BB occ = board.Pieces();

    const BB knights = board.Pieces(color, KNIGHT);
    const BB kings   = board.Pieces(color, KING);
    const BB queens  = board.Pieces(color, QUEEN);
    const BB bishops = board.Pieces(color, BISHOP) | queens;
    const BB rooks   = board.Pieces(color, ROOK) | queens;

    GeneratePawnMoves<GenType::Attack>(board, color, moves);

    SliderAttack(moves, BishopAttacks, bishops, occ, nus);
    SliderAttack(moves, RookAttacks, rooks, occ, nus);

    BuildJumperMoves<MoveList::Attack>(moves, KnightAttacks, knights, nus, Move::Capture);
    BB attacks = board.GenerateAttacks(~color);
    BuildJumperMoves<MoveList::Attack>(moves, KingAttacks, kings, nus & (~attacks), Move::Capture);

    return moves;
}
