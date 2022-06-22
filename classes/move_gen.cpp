#include "headers/move_gen.hh"

std::vector<Move> MoveGen::GetPawnMoves(Color color, BitBoard board) {
    Color oppColor = (color == Color::White) ? Color::Black : Color::White;
    Direction up = (color == Color::White) ? Direction::North : Direction::South;
    Direction upRight = (up == Direction::North) ? Direction::NorthEast : Direction::SouthEast;
    Direction upLeft = (up == Direction::North) ? Direction::NorthWest : Direction::SouthWest;
    Direction captureDirections[2] = { upRight, upLeft };
    std::vector<Move> moves = std::vector<Move>();

    // Generate single and double step moves
    U64 to = board.pieceBB[(int) PieceType::Pawn] & board.colorBB[(int) color];
    for (int i = 1; i <= 2; i++) {
        to = BitShifts::Shift(to, up, 1) & ~board.occupiedBB;

        U64 toDup = to;
        while (toDup) {
            U64 lsb = Utilities::LSB_Pop(&toDup);
            moves.push_back(Move((Square) (lsb - (int) up * i), (Square) lsb, color, Color::None));
        }
    }

    // Generate captures
    for (int i = 0; i < 2; i++) {
        to = BitShifts::Shift(board.pieceBB[(int) PieceType::Pawn] & board.colorBB[(int) color], captureDirections[i], 1) & board.colorBB[(int)oppColor];
        while (to) {
            U64 lsb = Utilities::LSB_Pop(&to);
            U64 from = lsb - (int) captureDirections[i];
            if (std::abs((int) lsb % 8 - (int) from % 8) <= 1)
                moves.push_back(Move((Square) (lsb - (int) captureDirections[i]), (Square) lsb, color, oppColor));
        }
    }

    

    return moves;
}