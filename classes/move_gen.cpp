#include "headers/move_gen.hh"

MoveGen::MoveGen(Color color) {
    this->color = color;
    this->oppColor = Utilities::GetOppositeColor(color);
    this->up = (color == Color::White) ? Direction::North : Direction::South;
    this->upRight = (up == Direction::North) ? Direction::NorthEast : Direction::SouthEast;
    this->upLeft = (up == Direction::North) ? Direction::NorthWest : Direction::SouthWest;
    this->doubleRank = (up == Direction::North) ? Row::Row4 : Row::Row5;
    GenerateKnightMoves();
    GenerateKingMoves();
}

int MoveGen::GetAllMoves(Move* moves, BitBoard board) {
    int moveCount = 0;
    moveCount += GetPawnMoves(moves, moveCount, board);
    moveCount += GetRookMoves(moves, moveCount, board);
    moveCount += GetBishopMoves(moves, moveCount, board);
    moveCount += GetQueenMoves(moves, moveCount, board);
    moveCount += GetKnightMoves(moves, moveCount, board);
    return moveCount;
}

int MoveGen::GetPawnMoves(Move* moves, int startIndex, BitBoard board) {
    Direction captureDirections[2] = { upRight, upLeft };
    // Generate single and double step moves
    U64 to = board.pieceBB[(int) PieceType::Pawn] & board.colorBB[(int) color];
    int moveCount = 0;
    for (int i = 1; i <= 2; i++) {
        to = BitShifts::Shift(to, up, 1) & ~board.occupiedBB;

        U64 toDup = to;
        if (i == 2)
            toDup = toDup & (U64)doubleRank;

        while (toDup) {
            U64 lsb = Utilities::LSB_Pop(&toDup);
            moves[startIndex + moveCount] = Move((Square) (lsb - (int) up * i), (Square) lsb, color, Color::None, PieceType::Pawn, PieceType::None);
            moveCount++;
        }
    }

    // Generate captures
    for (int i = 0; i < 2; i++) {
        to = BitShifts::Shift(board.pieceBB[(int) PieceType::Pawn] & board.colorBB[(int) color], captureDirections[i], 1) & board.colorBB[(int)oppColor];
        while (to) {
            U64 lsb = Utilities::LSB_Pop(&to);
            U64 from = lsb - (int) captureDirections[i];
            if (std::abs((int) lsb % 8 - (int) from % 8) <= 1) {
                moves[startIndex + moveCount] = Move((Square) (lsb - (int) captureDirections[i]), (Square) lsb, color, oppColor, PieceType::Pawn, board.GetType((Square) lsb, oppColor));
                moveCount++;
            }
        }
    }

    return moveCount;
}

int MoveGen::GetRookMoves(Move* moves, int startIndex, BitBoard board) {
    Direction directions[4] = { Direction::North, Direction::East, Direction::South, Direction::West };
    int moveCount = 0;

    U64 pieces = board.pieceBB[(int) PieceType::Rook] & board.colorBB[(int) color];
    for (int i = 0; i < 4; i++)
        moveCount += GetMoves(moves, startIndex, board, pieces, directions[i], PieceType::Rook);
    return moveCount;
}

int MoveGen::GetBishopMoves(Move* moves, int startIndex, BitBoard board) {
    Direction directions[4] = { Direction::NorthEast, Direction::NorthWest, Direction::SouthEast, Direction::SouthWest };
    int moveCount = 0;

    U64 pieces = board.pieceBB[(int) PieceType::Bishop] & board.colorBB[(int) color];
    for (int i = 0; i < 4; i++)
        moveCount += GetMoves(moves, startIndex, board, pieces, directions[i], PieceType::Bishop);
    return moveCount;
}

int MoveGen::GetQueenMoves(Move* moves, int startIndex, BitBoard board) {
    Direction directions[8] = { Direction::North, Direction::East, Direction::South, Direction::West, 
                                Direction::NorthEast, Direction::NorthWest, Direction::SouthEast, Direction::SouthWest };
    int moveCount = 0;

    U64 pieces = board.pieceBB[(int) PieceType::Queen] & board.colorBB[(int) color];
    for (int i = 0; i < 8; i++)
        moveCount += GetMoves(moves, startIndex, board, pieces, directions[i], PieceType::Queen);
    return moveCount;
}

int MoveGen::GetKnightMoves(Move* moves, int startIndex, BitBoard board) {
    U64 pieces = board.pieceBB[(int) PieceType::Knight] & board.colorBB[(int) color];
    int moveCount = 0;

    while (pieces) {
            int lsbPiece = Utilities::LSB_Pop(&pieces);
            U64 to = knightMoves[lsbPiece];
            // Attack moves
            U64 attackMoves = to & board.colorBB[(int) oppColor];
            to &= ~board.occupiedBB;

            while (attackMoves) {
                int lsb = Utilities::LSB_Pop(&attackMoves);
                moves[startIndex + moveCount] = Move((Square) lsbPiece, (Square) lsb, color, oppColor, PieceType::Knight, board.GetType((Square) lsb, oppColor));
                moveCount++;
            }

            // Quiet moves
            U64 quietMoves = to & (~board.occupiedBB);
            while (quietMoves) {
                int lsb = Utilities::LSB_Pop(&quietMoves);
                moves[startIndex + moveCount] = Move((Square) lsbPiece, (Square) lsb, color, Color::None, PieceType::Knight, PieceType::None);
                moveCount++;
            }
    }

    return moveCount;
}

int MoveGen::GetKingMoves(Move* moves, int startIndex, BitBoard board) {
    U64 pieces = board.pieceBB[(int) PieceType::King] & board.colorBB[(int) color];
    int moveCount = 0;

    while (pieces) {
            int lsbPiece = Utilities::LSB_Pop(&pieces);
            U64 to = kingMoves[lsbPiece];
            // Attack moves
            U64 attackMoves = to & board.colorBB[(int) oppColor];
            to &= ~board.occupiedBB;

            while (attackMoves) {
                int lsb = Utilities::LSB_Pop(&attackMoves);
                moves[startIndex + moveCount] = Move((Square) lsbPiece, (Square) lsb, color, oppColor, PieceType::King, board.GetType((Square) lsb, oppColor));
                moveCount++;
            }

            // Quiet moves
            U64 quietMoves = to & (~board.occupiedBB);
            while (quietMoves) {
                int lsb = Utilities::LSB_Pop(&quietMoves);
                moves[startIndex + moveCount] = Move((Square) lsbPiece, (Square) lsb, color, Color::None, PieceType::King, PieceType::None);
                moveCount++;
            }
    }

    return moveCount;
}

int MoveGen::GetMoves(Move* moves, int startIndex, BitBoard board, U64 pieces, Direction direction, PieceType type) {
    U64 to = pieces;
    int moveCount = 0;
    int counter = 1;
    while (to) {
        to = BitShifts::Shift(to & Utilities::NotEdge(direction), direction, 1) & ~board.colorBB[(int) color];
        
        U64 attackMoves = to & board.occupiedBB;
        to &= ~board.occupiedBB;

        while (attackMoves) {
            int lsb = Utilities::LSB_Pop(&attackMoves);
            moves[startIndex + moveCount] = Move((Square) (lsb - (int) direction * counter), (Square) lsb, color, oppColor, type, board.GetType((Square) lsb, oppColor));
            moveCount++;
        }

        U64 quietMoves = to;

        while (quietMoves) {
            int lsb = Utilities::LSB_Pop(&quietMoves);
            moves[startIndex + moveCount] = Move((Square) (lsb - (int) direction * counter), (Square) lsb, color, Color::None, type, PieceType::None);
            moveCount++;
        }

        counter++;
    }

    return moveCount;
}

void MoveGen::GenerateKnightMoves() {
    for (int i = 0; i < 64; i++) {
        U64 bit = C64(i);
        if (bit & (U64) NotEdgeKnight::North) {
            if (bit & (U64) NotEdge::East) {
                knightMoves[i] |= BitShifts::Shift(BitShifts::Shift(bit, Direction::North, 1), Direction::NorthEast, 1);
            }
            if (bit & (U64) NotEdge::West) {
                knightMoves[i] |= BitShifts::Shift(BitShifts::Shift(bit, Direction::North, 1), Direction::NorthWest, 1);
            }
        }
        if (bit & (U64) NotEdgeKnight::South) {
            if (bit & (U64) NotEdge::East) {
                knightMoves[i] |= BitShifts::Shift(BitShifts::Shift(bit, Direction::South, 1), Direction::SouthEast, 1);
            }
            if (bit & (U64) NotEdge::West) {
                knightMoves[i] |= BitShifts::Shift(BitShifts::Shift(bit, Direction::South, 1), Direction::SouthWest, 1);
            }
        }
        if (bit & (U64) NotEdgeKnight::East) {
            if (bit & (U64) NotEdge::North) {
                knightMoves[i] |= BitShifts::Shift(BitShifts::Shift(bit, Direction::North, 1), Direction::East, 2);
            }
            if (bit & (U64) NotEdge::South) {
                knightMoves[i] |= BitShifts::Shift(BitShifts::Shift(bit, Direction::South, 1), Direction::East, 2);
            }
        }
        if (bit & (U64) NotEdgeKnight::West) {
            if (bit & (U64) NotEdge::North) {
                knightMoves[i] |= BitShifts::Shift(BitShifts::Shift(bit, Direction::North, 1), Direction::West, 2);
            }
            if (bit & (U64) NotEdge::South) {
                knightMoves[i] |= BitShifts::Shift(BitShifts::Shift(bit, Direction::South, 1), Direction::West, 2);
            }
        }
    }
}

void MoveGen::GenerateKingMoves() {
    for (int i = 0; i < 64; i++) {
        U64 bit = C64(i);
        if (bit & (U64) NotEdge::North) {
            kingMoves[i] |= BitShifts::Shift(bit, Direction::North, 1);
            if (bit & (U64) NotEdge::East)
                kingMoves[i] |= BitShifts::Shift(bit, Direction::NorthEast, 1);
            if (bit & (U64) NotEdge::West)
                kingMoves[i] |= BitShifts::Shift(bit, Direction::NorthWest, 1);
        }
        if (bit & (U64) NotEdge::South) {
            kingMoves[i] |= BitShifts::Shift(bit, Direction::South, 1);
            if (bit & (U64) NotEdge::East)
                kingMoves[i] |= BitShifts::Shift(bit, Direction::SouthEast, 1);
            if (bit & (U64) NotEdge::West)
                kingMoves[i] |= BitShifts::Shift(bit, Direction::SouthWest, 1);
        }
        if (bit & (U64) NotEdge::East) {
            kingMoves[i] |= BitShifts::Shift(bit, Direction::East, 1);
        }
        if (bit & (U64) NotEdge::West) {
            kingMoves[i] |= BitShifts::Shift(bit, Direction::West, 1);
        }
    }
}
