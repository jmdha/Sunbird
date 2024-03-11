#include "board.hpp"
#include "bb.hpp"
#include "masks.hpp"
#include "zobrist.hpp"
#include <cctype>
#include <cstring>
#include <sstream>

// CONSTRUCTOR

Board::Board(std::string FEN) noexcept {
    ClearBoard();
    // import pieces
    for (int y = HEIGHT - 1; y >= 0; y--) {
        int remainingSquares = WIDTH;
        while (remainingSquares > 0) {
            if (isdigit(FEN[0]))
                remainingSquares -= FEN[0] - 48;
            else {
                const Color color = islower(FEN[0]) ? BLACK : WHITE;
                const Piece piece = ToPiece(FEN[0]);
                assert(piece != PIECE_NONE);
                const Square sq = static_cast<Square>(8 * y + WIDTH - remainingSquares--);
                PlacePiece(color, piece, sq);
            }
            FEN.erase(0, 1);
        }
        FEN.erase(0, 1);
    }

    this->turn = tolower(FEN[0]) == 'w' ? WHITE : BLACK;

    FEN.erase(0, 2);
    std::array<Castling, COLOR_COUNT> &castling = this->history[0].castling;

    while (FEN[0] != ' ' && !FEN.empty()) {
        switch (FEN[0]) {
        case 'K':
        case 'k': castling[!!islower(FEN[0])] |= Castling::King; break;
        case 'Q':
        case 'q': castling[!!islower(FEN[0])] |= Castling::Queen; break;
        case '-': break;
        }
        FEN.erase(0, 1);
    }
    FEN.erase(0, 1);
}

Board::Board(const std::string &FEN, const std::string &moves) noexcept {
    *this = Board(FEN);
    std::istringstream ss(moves);
    std::string s;
    while (std::getline(ss, s, ' '))
        ApplyMove(Move(Pieces(), Pieces(KING), Pieces(PAWN), s));
}

// ACCESS

size_t Board::MoveCount() const noexcept { return this->move_count; }
size_t Board::Ply() const noexcept { return this->ply; }
Color Board::Turn() const noexcept { return this->turn; }
Square Board::EP() const noexcept { return this->history[ply].ep; }
Castling Board::GetCastling(Color color) const noexcept {
    return this->history[ply].castling[color];
}
Hash Board::GetHash() const noexcept { return this->history[ply].hash; }
BB Board::Pieces() const noexcept { return Pieces(WHITE) | Pieces(BLACK); };
BB Board::Pieces(Piece piece) const noexcept { return this->pieces[piece]; }
BB Board::Pieces(Color color) const noexcept { return this->colors[color]; }
BB Board::Pieces(Color color, Piece piece) const noexcept { return Pieces(color) & Pieces(piece); }
Piece Board::SquarePiece(Square square) const noexcept { return this->square_pieces[square]; }
Color Board::SquareColor(Square square) const noexcept {
    if (square & Pieces(WHITE)) return WHITE;
    if (square & Pieces(BLACK)) return BLACK;
    return COLOR_NONE;
}

bool Board::IsKingSafe(Color color) const noexcept {
    const Square king = lsb(Pieces(color, KING));

    const BB occ     = Pieces();
    const BB pawns   = Pieces(!color, PAWN);
    const BB knights = Pieces(!color, KNIGHT);
    const BB kings   = Pieces(!color, KING);
    const BB bishops = Pieces(!color, BISHOP) | Pieces(!color, QUEEN);
    const BB rooks   = Pieces(!color, ROOK) | Pieces(!color, QUEEN);

    if (PAWN_ATTACKS[color][king] & pawns) return false;
    if (PSEUDO_ATTACKS[KNIGHT][king] & knights) return false;
    if (PSEUDO_ATTACKS[KING][king] & kings) return false;

    if (const BB ray = DIR_RAYS[king][NORTH]; ray & rooks)
        if (lsb(ray & rooks) == lsb(ray & occ)) return false;
    if (const BB ray = DIR_RAYS[king][EAST]; ray & rooks)
        if (lsb(ray & rooks) == lsb(ray & occ)) return false;
    if (const BB ray = DIR_RAYS[king][SOUTH]; ray & rooks)
        if (msb(ray & rooks) == msb(ray & occ)) return false;
    if (const BB ray = DIR_RAYS[king][WEST]; ray & rooks)
        if (msb(ray & rooks) == msb(ray & occ)) return false;

    if (const BB ray = DIR_RAYS[king][NORTH_EAST]; ray & bishops)
        if (lsb(ray & bishops) == lsb(ray & occ)) return false;
    if (const BB ray = DIR_RAYS[king][NORTH_WEST]; ray & bishops)
        if (lsb(ray & bishops) == lsb(ray & occ)) return false;
    if (const BB ray = DIR_RAYS[king][SOUTH_EAST]; ray & bishops)
        if (msb(ray & bishops) == msb(ray & occ)) return false;
    if (const BB ray = DIR_RAYS[king][SOUTH_WEST]; ray & bishops)
        if (msb(ray & bishops) == msb(ray & occ)) return false;

    return true;
}

BB Board::GenerateAttacks(Color color) const noexcept {
    const BB occ = Pieces();
    BB pawns     = Pieces(color, PAWN);
    BB knights   = Pieces(color, KNIGHT);
    BB kings     = Pieces(color, KING);

    BB attacks = 0;

    while (pawns)
        attacks |= PAWN_ATTACKS[color][lsb_pop(pawns)];
    while (knights)
        attacks |= PSEUDO_ATTACKS[KNIGHT][lsb_pop(knights)];
    while (kings)
        attacks |= PSEUDO_ATTACKS[KING][lsb_pop(kings)];

    BB bishops    = Pieces(color, BISHOP) | Pieces(color, QUEEN);
    BB rooks      = Pieces(color, ROOK) | Pieces(color, QUEEN);
    BB sliders[2] = {bishops, rooks};
    const std::array<std::array<BB, SQUARE_COUNT>, 2> &p_attacks = {
        PSEUDO_ATTACKS[BISHOP], PSEUDO_ATTACKS[ROOK]};
    for (int i = 0; i < 2; i++) {
        while (sliders[i]) {
            const Square piece = lsb_pop(sliders[i]);
            BB unblocked       = p_attacks[i][piece];
            for (int offset = 1; offset < 8 && unblocked; offset++) {
                BB p_ring   = RINGS[piece][offset] & unblocked;
                BB blockers = p_ring & occ;

                attacks |= p_ring;

                while (blockers)
                    unblocked &= ~SQ_RAYS[piece][lsb_pop(blockers)];
            }
        }
    }
    return attacks;
}

bool Board::IsThreefold() const noexcept {
    size_t count = 0;
    for (size_t i = 0; i < ply; i++)
        if (history[i].hash == history[ply].hash) count++;
    return count > 1;
}

// MODIFIERS

void Board::ClearBoard() {
    memset(this, 0, sizeof(Board));
    for (size_t i = 0; i < MAX_PLY; i++) {
        this->history[i].ep       = SQUARE_NONE;
        this->history[i].captured = PIECE_NONE;
        this->history[i].hash     = 0;
    }
    for (const auto sq : SQUARES)
        this->square_pieces[sq] = PIECE_NONE;
}

void Board::FlipPiece(Color color, Piece piece, Square square) noexcept {
    assert(color != COLOR_NONE);
    assert(piece != PIECE_NONE);
    assert(square != SQUARE_NONE);
    this->colors[color] ^= square;
    this->pieces[piece] ^= square;
    FlipSquare(this->history[ply].hash, color, piece, square);
}
void Board::PlacePiece(Color color, Piece piece, Square square) noexcept {
    FlipPiece(color, piece, square);
    this->square_pieces[square] = piece;
}
void Board::RemovePiece(Color color, Piece piece, Square square) noexcept {
    FlipPiece(color, piece, square);
    this->square_pieces[square] = PIECE_NONE;
}
void Board::ApplyMove(Move move) noexcept {
    this->ply++;
    this->history[ply].castling = this->history[ply - 1].castling;
    this->history[ply].hash     = this->history[ply - 1].hash;
    const Color us              = Turn();
    const Color nus             = !us;
    const Square ori            = move.Origin();
    const Square dst            = move.Destination();
    Piece piece                 = SquarePiece(ori);
    Piece target                = PIECE_NONE;
    Square target_square        = dst;
    Square ep                   = SQUARE_NONE;

    RemovePiece(us, piece, ori);

    switch (move.GetType()) {
    case Move::KingCastle:
    case Move::QueenCastle: {
        const Square ROOK_ORI[2][2] = {{A1, A8}, {H1, H8}};
        const Square ROOK_DST[2][2] = {{D1, D8}, {F1, F8}};
        const bool king_side        = dst > ori;
        const Square rook_ori       = ROOK_ORI[king_side][us];
        const Square rook_dst       = ROOK_DST[king_side][us];
        RemovePiece(us, ROOK, rook_ori);
        PlacePiece(us, ROOK, rook_dst);
        break;
    }
    case Move::NPromotion: piece = KNIGHT; break;
    case Move::BPromotion: piece = BISHOP; break;
    case Move::RPromotion: piece = ROOK; break;
    case Move::QPromotion: piece = QUEEN; break;
    case Move::NPromotionCapture: piece = KNIGHT; goto CAPTURE;
    case Move::BPromotionCapture: piece = BISHOP; goto CAPTURE;
    case Move::RPromotionCapture: piece = ROOK; goto CAPTURE;
    case Move::QPromotionCapture: piece = QUEEN; goto CAPTURE;
    case Move::EPCapture:
        target_square = static_cast<Square>(this->history[ply - 1].ep + (us == WHITE ? -8 : 8));
    case Move::Capture: {
    CAPTURE:
        target = SquarePiece(target_square);
        RemovePiece(nus, target, target_square);
        if (target_square == CORNER_A[nus])
            this->history[ply].castling[nus] &= Castling::King;
        else if (target_square == CORNER_H[nus])
            this->history[ply].castling[nus] &= Castling::Queen;
        break;
    }
    case Move::DoublePawnPush: ep = static_cast<Square>((us == WHITE) ? ori + 8 : ori - 8); break;
    default: break;
    }

    PlacePiece(us, piece, dst);

    if (piece == KING) [[unlikely]]
        this->history[ply].castling[us] = Castling::None;
    else if (piece == ROOK) [[unlikely]] {
        if (ori == CORNER_A[us])
            this->history[ply].castling[us] &= Castling::King;
        else if (ori == CORNER_H[us])
            this->history[ply].castling[us] &= Castling::Queen;
    }

    if (auto p_ep = this->history[ply - 1].ep; p_ep != ep) {
        FlipEnpassant(this->history[ply].hash, ep);
        FlipEnpassant(this->history[ply].hash, p_ep);
    }

    this->move_count++;
    this->history[ply].ep       = ep;
    this->history[ply].captured = target;
    this->turn                  = !this->Turn();
    FlipTurn(this->history[ply].hash);
}
void Board::UndoMove(Move move) noexcept {
    this->turn           = !this->Turn();
    const Color us       = Turn();
    const Color nus      = !us;
    const Square ori     = move.Origin();
    const Square dst     = move.Destination();
    Piece piece          = SquarePiece(dst);
    Piece target         = this->history[ply].captured;
    Square target_square = dst;

    RemovePiece(us, piece, dst);

    switch (move.GetType()) {
    case Move::KingCastle:
    case Move::QueenCastle: {
        const Square ROOK_ORI[2][2] = {{A1, A8}, {H1, H8}};
        const Square ROOK_DST[2][2] = {{D1, D8}, {F1, F8}};
        const bool king_side        = dst > ori;
        const Square rook_ori       = ROOK_ORI[king_side][us];
        const Square rook_dst       = ROOK_DST[king_side][us];
        RemovePiece(us, ROOK, rook_dst);
        PlacePiece(us, ROOK, rook_ori);
        break;
    }
    case Move::NPromotion:
    case Move::BPromotion:
    case Move::RPromotion:
    case Move::QPromotion: piece = PAWN; break;
    case Move::NPromotionCapture:
    case Move::BPromotionCapture:
    case Move::RPromotionCapture:
    case Move::QPromotionCapture: piece = PAWN; goto CAPTURE;
    case Move::EPCapture:
        target_square = static_cast<Square>(this->history[ply - 1].ep + (us == WHITE ? -8 : 8));
    case Move::Capture:
    CAPTURE:
        PlacePiece(nus, target, target_square);
        break;
    default: break;
    }

    PlacePiece(us, piece, ori);
    this->ply--;
}
