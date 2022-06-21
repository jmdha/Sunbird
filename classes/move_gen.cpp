#include "headers/move_gen.hh"

std::vector<Move> MoveGen::GetPawnMoves(BitBoard board) {
    U64 to = BitShifts::NOne(board.pieceBB[(int) PieceType::Pawn] & board.colorBB[(int)Color::White]);
    std::vector<Move> moves = std::vector<Move>();
    printf("Moves %llu\n", board.pieceBB[(int) PieceType::Pawn]);
    while (to) {
        U64 lsb = ffsll(to) - 1;
        to ^= (C64(0) << lsb);
        moves.push_back(Move((Square) (lsb << 8), (Square) lsb, PieceType::Pawn, PieceType::None, Color::White, Color::Black));
    }
    return std::vector<Move>();
}