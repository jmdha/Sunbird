#include "headers/move_gen.hh"

MoveGen::MoveGen(Color color) {
    this->color = color;
    this->oppColor = Utilities::GetOppositeColor(color);
    this->up = (color == Color::White) ? Direction::North : Direction::South;
    this->upRight = (up == Direction::North) ? Direction::NorthEast : Direction::SouthEast;
    this->upLeft = (up == Direction::North) ? Direction::NorthWest : Direction::SouthWest;
    this->doubleRank = (up == Direction::North) ? Row::Row4 : Row::Row5;
}

std::vector<Move> MoveGen::GetAllMoves(BitBoard board) {
    std::vector<Move> moves = std::vector<Move>();
    std::vector<Move> tempMoves = GetPawnMoves(board);
    moves.insert(moves.end(), tempMoves.begin(), tempMoves.end());
    tempMoves = GetRookMoves(board);
    moves.insert(moves.end(), tempMoves.begin(), tempMoves.end());
    tempMoves = GetBishopMoves(board);
    moves.insert(moves.end(), tempMoves.begin(), tempMoves.end());
    tempMoves = GetQueenMoves(board);
    moves.insert(moves.end(), tempMoves.begin(), tempMoves.end());
    return moves;
}

std::vector<Move> MoveGen::GetPawnMoves(BitBoard board) {
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
            moves.push_back(Move((Square) (lsb - (int) up * i), (Square) lsb, color, Color::None, PieceType::Pawn, PieceType::None));
        }
    }

    // Generate captures
    for (int i = 0; i < 2; i++) {
        to = BitShifts::Shift(board.pieceBB[(int) PieceType::Pawn] & board.colorBB[(int) color], captureDirections[i], 1) & board.colorBB[(int)oppColor];
        while (to) {
            U64 lsb = Utilities::LSB_Pop(&to);
            U64 from = lsb - (int) captureDirections[i];
            if (std::abs((int) lsb % 8 - (int) from % 8) <= 1)
                moves.push_back(Move((Square) (lsb - (int) captureDirections[i]), (Square) lsb, color, oppColor, PieceType::Pawn, board.GetType((Square) lsb, oppColor)));
        }
    }

    return moves;
}

std::vector<Move> MoveGen::GetRookMoves(BitBoard board) {
    Direction directions[4] = { Direction::North, Direction::East, Direction::South, Direction::West };
    std::vector<Move> moves = std::vector<Move>();
    U64 pieces = board.pieceBB[(int) PieceType::Rook] & board.colorBB[(int) color];
    for (int i = 0; i < 4; i++) {
        std::vector<Move> tempMoves = GetMoves(board, pieces, directions[i], PieceType::Rook);
        moves.insert(moves.end(), tempMoves.begin(), tempMoves.end());
    }
    return moves;
}

std::vector<Move> MoveGen::GetBishopMoves(BitBoard board) {
    Direction directions[4] = { Direction::NorthEast, Direction::NorthWest, Direction::SouthEast, Direction::SouthWest };
    std::vector<Move> moves = std::vector<Move>();
    U64 pieces = board.pieceBB[(int) PieceType::Bishop] & board.colorBB[(int) color];
    for (int i = 0; i < 4; i++) {
        std::vector<Move> tempMoves = GetMoves(board, pieces, directions[i], PieceType::Bishop);
        moves.insert(moves.end(), tempMoves.begin(), tempMoves.end());
    }
    return moves;
}

std::vector<Move> MoveGen::GetQueenMoves(BitBoard board) {
    Direction directions[8] = { Direction::North, Direction::East, Direction::South, Direction::West, 
                                Direction::NorthEast, Direction::NorthWest, Direction::SouthEast, Direction::SouthWest };
    std::vector<Move> moves = std::vector<Move>();
    U64 pieces = board.pieceBB[(int) PieceType::Queen] & board.colorBB[(int) color];
    for (int i = 0; i < 8; i++) {
        std::vector<Move> tempMoves = GetMoves(board, pieces, directions[i], PieceType::Queen);
        moves.insert(moves.end(), tempMoves.begin(), tempMoves.end());
    }
    return moves;
}

std::vector<Move> MoveGen::GetMoves(BitBoard board, U64 pieces, Direction direction, PieceType type) {
    std::vector<Move> moves = std::vector<Move>();

    U64 to = pieces;
    int counter = 1;
    while (to) {
        to = BitShifts::Shift(to & Utilities::NotEdge(direction), direction, 1) & ~board.colorBB[(int) color];
        
        U64 attackMoves = to & board.occupiedBB;
        to &= ~board.occupiedBB;

        while (attackMoves) {
            U64 lsb = Utilities::LSB_Pop(&attackMoves);
            moves.push_back(Move((Square) (lsb - (int) direction * counter), (Square) lsb, color, oppColor, type, board.GetType((Square) lsb, oppColor)));
        }

        U64 quietMoves = to;

        while (quietMoves) {
            U64 lsb = Utilities::LSB_Pop(&quietMoves);
            moves.push_back(Move((Square) (lsb - (int) direction * counter), (Square) lsb, color, Color::None, type, PieceType::None));
        }

        counter++;
    }

    return moves;
}
