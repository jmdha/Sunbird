#include "move_gen.hpp"
#include "bb.hpp"
#include "masks.hpp"
#include <optional>

void BuildPawnMoves(MoveList &moves, BB targets, int delta, Move::Type type) noexcept {
    while (targets) {
        const Square dst = lsb_pop(targets);
        const Square ori = static_cast<Square>(dst - delta);
        moves << Move(ori, dst, type);
    }
}

void BuildPromotionMoves(MoveList &moves, BB targets, int delta, bool capture) noexcept {
    BuildPawnMoves(moves, targets, delta, (Move::Type)(Move::NPromotion + 4 * capture));
    BuildPawnMoves(moves, targets, delta, (Move::Type)(Move::BPromotion + 4 * capture));
    BuildPawnMoves(moves, targets, delta, (Move::Type)(Move::RPromotion + 4 * capture));
    BuildPawnMoves(moves, targets, delta, (Move::Type)(Move::QPromotion + 4 * capture));
}

void GeneratePawnQuiet(MoveList &moves, Color turn, BB pawns, BB empty) noexcept {
    static const int DELTA[COLOR_COUNT]        = {8, -8};
    static const BB ADVANCED_ONCE[COLOR_COUNT] = {RANK_3, RANK_6};
    static const BB PROMOTED_RANK              = RANK_8 | RANK_1;

    BB advanced       = shift_up(pawns, turn) & empty;
    BB advanced_twice = shift_up(advanced & ADVANCED_ONCE[turn], turn) & empty;
    BB promoted       = advanced & PROMOTED_RANK;
    advanced          = advanced ^ promoted;

    BuildPawnMoves(moves, advanced, DELTA[turn], Move::Quiet);
    BuildPawnMoves(moves, advanced_twice, 2 * DELTA[turn], Move::DoublePawnPush);
    BuildPromotionMoves(moves, promoted, DELTA[turn], false);
}

void GeneratePawnTactical(MoveList &moves, Color turn, BB pawns, BB nus, Square ep) noexcept {
    static const int LEFT_DELTA[COLOR_COUNT]  = {7, -9};
    static const int RIGHT_DELTA[COLOR_COUNT] = {9, -7};
    static const BB PROMOTED_RANK             = RANK_8 | RANK_1;

    // Left and right from the perspective of white
    BB left_capture    = nus & (turn == WHITE ? pawns << 7 : pawns >> 9) & ~FILE_H;
    BB right_capture   = nus & (turn == WHITE ? pawns << 9 : pawns >> 7) & ~FILE_A;
    BB left_promotion  = left_capture & PROMOTED_RANK;
    BB right_promotion = right_capture & PROMOTED_RANK;
    left_capture       = left_capture ^ left_promotion;
    right_capture      = right_capture ^ right_promotion;

    BuildPawnMoves(moves, left_capture, LEFT_DELTA[turn], Move::Capture);
    BuildPawnMoves(moves, right_capture, RIGHT_DELTA[turn], Move::Capture);
    BuildPromotionMoves(moves, left_promotion, LEFT_DELTA[turn], true);
    BuildPromotionMoves(moves, right_promotion, RIGHT_DELTA[turn], true);

    BB ep_pawns = PAWN_ATTACKS[!turn][ep] & pawns;
    while (ep_pawns) [[unlikely]]
        moves << Move(lsb_pop(ep_pawns), ep, Move::EPCapture);
}

void BuildMoves(MoveList &moves, Square ori, BB targets, Move::Type type) noexcept {
    while (targets)
        moves << Move(ori, lsb_pop(targets), type);
}

void BuildJumperMoves(
    MoveList &moves, const std::array<BB, SQUARE_COUNT> &attacks, BB pieces, BB targets,
    Move::Type type
) noexcept {
    while (pieces) {
        const Square ori = lsb_pop(pieces);
        BuildMoves(moves, ori, attacks[ori] & targets, type);
    }
}

void GenerateSliderMoves(
    MoveList &moves, const std::array<BB, SQUARE_COUNT> &attacks, BB pieces, BB targets, BB occ,
    Move::Type type
) noexcept {
    while (pieces) {
        const Square piece = lsb_pop(pieces);
        BB unblocked       = attacks[piece];

        for (int i = 1; i < 8 && unblocked; i++) {
            BB p_ring    = RINGS[piece][i] & unblocked;
            BB pot_moves = p_ring & targets;
            BB blocked   = p_ring & occ;

            while (blocked)
                unblocked &= ~SQ_RAYS[piece][lsb_pop(blocked)];

            BuildMoves(moves, piece, pot_moves, type);
        }
    }
}

void GenerateCastlingMoves(MoveList &moves, const Board &board, Color color) noexcept {
    static const Square KING_POS[2]         = {E1, E8};
    static const Square KING_CASTLE_POS[2]  = {G1, G8};
    static const Square QUEEN_CASTLE_POS[2] = {C1, C8};
    static const BB KING_BLOCKERS[2]        = {ToBB(F1) | G1, ToBB(F8) | G8};
    static const BB QUEEN_BLOCKERS[2]       = {ToBB(B1) | C1 | D1, ToBB(B8) | C8 | D8};
    static const BB QUEEN_ATTACKERS[2]      = {ToBB(C1) | D1, ToBB(C8) | D8};

    const Castling &castling = board.GetCastling(color);
    const BB occ             = board.Pieces();
    std::optional<BB> attacks;

    if ((bool)(castling & Castling::King) && !(occ & KING_BLOCKERS[color])) {
        attacks = board.GenerateAttacks(!color);
        if (attacks.value() & KING_POS[color]) return;
        if (!(attacks.value() & KING_BLOCKERS[color]))
            moves << Move(KING_POS[color], KING_CASTLE_POS[color], Move::KingCastle);
    }
    if ((bool)(castling & Castling::Queen) && !(occ & QUEEN_BLOCKERS[color])) {
        if (!attacks.has_value()) {
            attacks = board.GenerateAttacks(!color);
            if (attacks.value() & KING_POS[color]) return;
        }
        if (!(attacks.value() & QUEEN_ATTACKERS[color]))
            moves << Move(KING_POS[color], QUEEN_CASTLE_POS[color], Move::QueenCastle);
    }
}

void GenerateMovesQuiet(MoveList &moves, const Board &board, Color color) noexcept {
    const BB occ   = board.Pieces();
    const BB empty = ~occ;

    const BB pawns   = board.Pieces(color, PAWN);
    const BB knights = board.Pieces(color, KNIGHT);
    const BB kings   = board.Pieces(color, KING);
    const BB bishops = board.Pieces(color, BISHOP) | board.Pieces(color, QUEEN);
    const BB rooks   = board.Pieces(color, ROOK) | board.Pieces(color, QUEEN);

    GeneratePawnQuiet(moves, color, pawns, empty);
    GenerateSliderMoves(moves, PSEUDO_ATTACKS[BISHOP], bishops, empty, occ, Move::Quiet);
    GenerateSliderMoves(moves, PSEUDO_ATTACKS[ROOK], rooks, empty, occ, Move::Quiet);
    BuildJumperMoves(moves, PSEUDO_ATTACKS[KNIGHT], knights, empty, Move::Quiet);
    BuildJumperMoves(moves, PSEUDO_ATTACKS[KING], kings, empty, Move::Quiet);
    GenerateCastlingMoves(moves, board, color);
}

void GenerateMovesTactical(MoveList &moves, const Board &board, Color color) noexcept {
    const BB nus = board.Pieces(!color);
    const BB occ = board.Pieces();

    const BB pawns   = board.Pieces(color, PAWN);
    const BB knights = board.Pieces(color, KNIGHT);
    const BB kings   = board.Pieces(color, KING);
    const BB bishops = board.Pieces(color, BISHOP) | board.Pieces(color, QUEEN);
    const BB rooks   = board.Pieces(color, ROOK) | board.Pieces(color, QUEEN);

    GeneratePawnTactical(moves, color, pawns, nus, board.EP());
    GenerateSliderMoves(moves, PSEUDO_ATTACKS[BISHOP], bishops, nus, occ, Move::Capture);
    GenerateSliderMoves(moves, PSEUDO_ATTACKS[ROOK], rooks, nus, occ, Move::Capture);
    BuildJumperMoves(moves, PSEUDO_ATTACKS[KNIGHT], knights, nus, Move::Capture);
    BuildJumperMoves(moves, PSEUDO_ATTACKS[KING], kings, nus, Move::Capture);
}

void GenerateMovesAll(MoveList &moves, const Board &board, Color color) noexcept {
    GenerateMovesTactical(moves, board, color);
    GenerateMovesQuiet(moves, board, color);
}

MoveList GenerateMovesAll(const Board &board, Color color) noexcept {
    MoveList moves;
    GenerateMovesAll(moves, board, color);
    return moves;
}

MoveList GenerateLegalMoves(const Board &board, Color color) noexcept {
    Board temp_board      = board;
    MoveList pseudo_moves = GenerateMovesAll(board, color);
    MoveList legal_moves;
    for (auto &move : pseudo_moves) {
        temp_board.ApplyMove(move);
        if (board.IsKingSafe(color)) legal_moves << move;
        temp_board.UndoMove(move);
    }
    return legal_moves;
}
