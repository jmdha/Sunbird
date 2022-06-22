#include "headers/move_gen.hh"

std::vector<Move> MoveGen::GetPawnMoves(Color color, BitBoard board) {
    Color oppColor = (color == Color::White) ? Color::Black : Color::White;
    Direction up = (color == Color::White) ? Direction::North : Direction::South;
    Direction upRight = (up == Direction::North) ? Direction::NorthEast : Direction::SouthEast;
    Direction upLeft = (up == Direction::North) ? Direction::NorthWest : Direction::SouthWest;
    Row doubleRank = (up == Direction::North) ? Row::Row4 : Row::Row5;
    Direction captureDirections[2] = { upRight, upLeft };
    std::vector<Move> moves = std::vector<Move>();

    // Generate single and double step moves
    U64 to = board.pieceBB[(int) PieceType::Pawn] & board.colorBB[(int) color];
    for (int i = 1; i <= 2; i++) {
        to = BitShifts::Shift(to, up, 1) & ~board.occupiedBB;

        U64 toDup = to;
        if (i == 2)
            toDup = toDup & (U64)doubleRank;

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

std::vector<Move> MoveGen::GetRookMoves(Color color, BitBoard board) {
    Direction directions[4] = { Direction::North, Direction::East, Direction::South, Direction::West };
    std::vector<Move> moves = std::vector<Move>();
    U64 pieces = board.pieceBB[(int) PieceType::Rook] & board.colorBB[(int) color];
    for (int i = 0; i < 4; i++) {
        std::vector<Move> tempMoves = GetMoves(color, board, pieces, directions[i]);
        moves.insert(moves.end(), tempMoves.begin(), tempMoves.end());
    }
    return moves;
}

std::vector<Move> MoveGen::GetBishopMoves(Color color, BitBoard board) {
    Direction directions[4] = { Direction::NorthEast, Direction::NorthWest, Direction::SouthEast, Direction::SouthWest };
    std::vector<Move> moves = std::vector<Move>();
    U64 pieces = board.pieceBB[(int) PieceType::Bishop] & board.colorBB[(int) color];
    for (int i = 0; i < 4; i++) {
        std::vector<Move> tempMoves = GetMoves(color, board, pieces, directions[i]);
        moves.insert(moves.end(), tempMoves.begin(), tempMoves.end());
    }
    return moves;
}

std::vector<Move> MoveGen::GetMoves(Color color, BitBoard board, U64 pieces, Direction direction) {
    Color oppColor = (color == Color::White) ? Color::Black : Color::White;
    std::vector<Move> moves = std::vector<Move>();

    U64 to = pieces;
    int counter = 1;
    while (to) {
        to = BitShifts::Shift(to & Utilities::NotEdge(direction), direction, 1) & ~board.colorBB[(int) color];
        
        U64 attackMoves = to & board.occupiedBB;
        to &= ~board.occupiedBB;

        while (attackMoves) {
            U64 lsb = Utilities::LSB_Pop(&attackMoves);
            moves.push_back(Move((Square) (lsb - (int) direction * counter), (Square) lsb, color, Color::None));
        }

        U64 quietMoves = to;

        while (quietMoves) {
            U64 lsb = Utilities::LSB_Pop(&quietMoves);
            moves.push_back(Move((Square) (lsb - (int) direction * counter), (Square) lsb, color, Color::None));
        }

        counter++;
    }

    return moves;
}
