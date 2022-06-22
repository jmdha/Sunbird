#include "headers/move_gen.hh"

std::vector<Move> MoveGen::GetPawnMoves(Color color, BitBoard board) {
    Direction up = (color == Color::White) ? Direction::North : Direction::South;
    std::vector<Move> moves = std::vector<Move>();

    // Generate single and double step moves
    for (int i = 1; i <= 2; i++) {
        U64 to = BitShifts::Shift(board.pieceBB[(int) PieceType::Pawn] & board.colorBB[(int) color], up, i) & board.emptyBB;

        while (to) {
            U64 lsb = Utilities::LSB_Pop(&to);
            moves.push_back(Move((Square) (lsb - (int) up * i), (Square) lsb, PieceType::Pawn, PieceType::None, color, Color::Black));
        }
    }

    return std::vector<Move>();
}