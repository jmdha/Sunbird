#include "chess/internal/constants.hpp"
#include "jank/bit/bit.hpp"
#include <chess/board.hpp>
#include <chess/internal/bit_shift.hpp>

using namespace Chess;

void Board::Initialize() {
    for (U8 x = 0; x < WIDTH; ++x) {
        PlacePiece(Utilities::GetSquare(x, PAWNROWWHITE), PieceType::Pawn, Color::White);
        PlacePiece(Utilities::GetSquare(x, PAWNROWBLACK), PieceType::Pawn, Color::Black);
    }

    PlacePiece(Square::A1, PieceType::Rook, Color::White);
    PlacePiece(Square::H1, PieceType::Rook, Color::White);
    PlacePiece(Square::A8, PieceType::Rook, Color::Black);
    PlacePiece(Square::H8, PieceType::Rook, Color::Black);

    PlacePiece(Square::B1, PieceType::Knight, Color::White);
    PlacePiece(Square::G1, PieceType::Knight, Color::White);
    PlacePiece(Square::B8, PieceType::Knight, Color::Black);
    PlacePiece(Square::G8, PieceType::Knight, Color::Black);

    PlacePiece(Square::C1, PieceType::Bishop, Color::White);
    PlacePiece(Square::F1, PieceType::Bishop, Color::White);
    PlacePiece(Square::C8, PieceType::Bishop, Color::Black);
    PlacePiece(Square::F8, PieceType::Bishop, Color::Black);

    PlacePiece(Square::D1, PieceType::Queen, Color::White);
    PlacePiece(Square::D8, PieceType::Queen, Color::Black);

    PlacePiece(Square::E1, PieceType::King, Color::White);
    PlacePiece(Square::E8, PieceType::King, Color::Black);

    castling.push({Castling::All, Castling::All});
    turn = Color::White;
    oppColor = Color::Black;
}

void Board::DoMove(Move &move) {
    std::array<Castling, 2> nCastling = castling.top();
    PieceType fromType;
    if (move.GetType() == MoveType::KingCastle) [[unlikely]] {
        if (turn == Color::White) {
            PlacePiece(Square::G1, PieceType::King, Color::White);
            PlacePiece(Square::F1, PieceType::Rook, Color::White);
            RemovePiece(Square::E1, PieceType::King, Color::White);
            RemovePiece(Square::H1, PieceType::Rook, Color::White);
        } else {
            PlacePiece(Square::G8, PieceType::King, Color::Black);
            PlacePiece(Square::F8, PieceType::Rook, Color::Black);
            RemovePiece(Square::E8, PieceType::King, Color::Black);
            RemovePiece(Square::H8, PieceType::Rook, Color::Black);
        }
        fromType = PieceType::King;
    } else if (move.GetType() == MoveType::QueenCastle) [[unlikely]] {
        if (turn == Color::White) {
            PlacePiece(Square::C1, PieceType::King, Color::White);
            PlacePiece(Square::D1, PieceType::Rook, Color::White);
            RemovePiece(Square::E1, PieceType::King, Color::White);
            RemovePiece(Square::A1, PieceType::Rook, Color::White);
        } else {
            PlacePiece(Square::C8, PieceType::King, Color::Black);
            PlacePiece(Square::D8, PieceType::Rook, Color::Black);
            RemovePiece(Square::E8, PieceType::King, Color::Black);
            RemovePiece(Square::A8, PieceType::Rook, Color::Black);
        }
        fromType = PieceType::King;
    } else [[likely]] {
        fromType = GetType(move.GetFrom());
        RemovePiece(move.GetFrom(), fromType, turn);

        if (move.IsCapture()) {
            if (move.GetType() == MoveType::EPCapture) [[unlikely]] {
                captures.push(PieceType::Pawn);
                auto captureSquare = (Square)((turn == Color::White) ? (int)move.GetTo() - 8
                                                                     : (int)move.GetTo() + 8);
                RemovePiece(captureSquare, PieceType::Pawn, oppColor);
            } else [[likely]] {
                captures.push(GetType(move.GetTo()));
                RemovePiece(move.GetTo(), captures.top(), oppColor);
            }
            if (captures.top() == PieceType::Rook &&
                move.GetTo() == initRookPos[(U8)oppColor][(U8)Castling::King - 1])
                if (IsCastlingAllowed(oppColor, Castling::King))
                    nCastling[(int)oppColor] = nCastling[(int)oppColor] ^ Castling::King;
            if (captures.top() == PieceType::Rook &&
                move.GetTo() == initRookPos[(U8)oppColor][(U8)Castling::Queen - 1])
                if (IsCastlingAllowed(oppColor, Castling::Queen))
                    nCastling[(int)oppColor] = nCastling[(int)oppColor] ^ Castling::Queen;
        }
        if (move.IsPromotion()) [[unlikely]] {
            if (move.GetType() == MoveType::RPromotion ||
                move.GetType() == MoveType::RPromotionCapture)
                PlacePiece(move.GetTo(), PieceType::Rook, turn);
            else if (move.GetType() == MoveType::BPromotion ||
                     move.GetType() == MoveType::BPromotionCapture)
                PlacePiece(move.GetTo(), PieceType::Bishop, turn);
            else if (move.GetType() == MoveType::NPromotion ||
                     move.GetType() == MoveType::NPromotionCapture)
                PlacePiece(move.GetTo(), PieceType::Knight, turn);
            else if (move.GetType() == MoveType::QPromotion ||
                     move.GetType() == MoveType::QPromotionCapture)
                PlacePiece(move.GetTo(), PieceType::Queen, turn);
        } else
            PlacePiece(move.GetTo(), fromType, turn);
    }

    if (move.GetType() == MoveType::DoublePawnPush) [[unlikely]]
        PushEP(Utilities::GetColumn(move.GetFrom()));
    else
        PushEP(Column::None);

    // Castling rights
    if (fromType == PieceType::King) [[unlikely]] {
        nCastling[(int)turn] = Castling::None;
    } else if (fromType == PieceType::Rook) [[unlikely]] {
        if (IsCastlingAllowed(turn, Castling::King) &&
            move.GetFrom() == (turn == Color::White ? Square::H1 : Square::H8))
                    nCastling[(int)turn] = nCastling[(int)turn] ^ Castling::King;
        else if (IsCastlingAllowed(turn, Castling::Queen) &&
                 move.GetFrom() == (turn == Color::White ? Square::A1 : Square::A8))
                    nCastling[(int)turn] = nCastling[(int)turn] ^ Castling::Queen;
    }

    PushCastling(nCastling);
    SwitchTurn();
    zobrist.IncHash();
    ++ply;
}

void Board::UndoMove(Move move) {
    zobrist.DecHash();
    PieceType toType;

    if (move.GetType() == MoveType::KingCastle) [[unlikely]] {
        if (turn == Color::Black) {
            RemovePiece(Square::G1, PieceType::King, Color::White);
            RemovePiece(Square::F1, PieceType::Rook, Color::White);
            PlacePiece(Square::E1, PieceType::King, Color::White);
            PlacePiece(Square::H1, PieceType::Rook, Color::White);
        } else {
            RemovePiece(Square::G8, PieceType::King, Color::Black);
            RemovePiece(Square::F8, PieceType::Rook, Color::Black);
            PlacePiece(Square::E8, PieceType::King, Color::Black);
            PlacePiece(Square::H8, PieceType::Rook, Color::Black);
        }
    } else if (move.GetType() == MoveType::QueenCastle) [[unlikely]] {
        if (turn == Color::Black) {
            RemovePiece(Square::C1, PieceType::King, Color::White);
            RemovePiece(Square::D1, PieceType::Rook, Color::White);
            PlacePiece(Square::E1, PieceType::King, Color::White);
            PlacePiece(Square::A1, PieceType::Rook, Color::White);
        } else {
            RemovePiece(Square::C8, PieceType::King, Color::Black);
            RemovePiece(Square::D8, PieceType::Rook, Color::Black);
            PlacePiece(Square::E8, PieceType::King, Color::Black);
            PlacePiece(Square::A8, PieceType::Rook, Color::Black);
        }
    } else {
        toType = GetType(move.GetTo());
        if (move.IsPromotion()) [[unlikely]] {
            RemovePiece(move.GetTo(), toType, oppColor);
            PlacePiece(move.GetFrom(), PieceType::Pawn, oppColor);
        } else [[likely]] {
            RemovePiece(move.GetTo(), toType, oppColor);
            PlacePiece(move.GetFrom(), toType, oppColor);
        }

        if (move.IsCapture()) {
            if (move.GetType() == MoveType::EPCapture) [[unlikely]] {
                auto captureSquare = (Square)((turn == Color::Black) ? (int)move.GetTo() - 8
                                                                     : (int)move.GetTo() + 8);
                PlacePiece(captureSquare, PieceType::Pawn, turn);
            } else [[likely]]
                PlacePiece(move.GetTo(), captures.top(), turn);
            captures.pop();
        }
    }

    PopEP();
    PopCastling();
    SwitchTurn();
    --ply;
}

U64 Board::GenerateAttackSquares(Color color) const {
    U64 attacks = 0;

    U64 tempPieces[PIECECOUNT];
    for (U8 pIndex = 0; pIndex < PIECECOUNT; pIndex++)
        tempPieces[pIndex] = GetPiecePos(color, (PieceType)pIndex);

    while (tempPieces[(U8)PieceType::Pawn])
        attacks |= PawnAttacks[(int)color][jank::bit::lsb_pop(tempPieces[(U8)PieceType::Pawn])];

    for (U8 pIndex = 1; pIndex < PIECECOUNT; ++pIndex)
        while (tempPieces[pIndex]) {
            unsigned short pos = jank::bit::lsb_pop(tempPieces[pIndex]);
            U64 attacks1 = BitShift::MOVES[pIndex][pos];

            for (U64 b = occupiedBB & BitShift::BB[pIndex][pos]; b != 0; b &= (b - 1))
                attacks1 &= ~BitShift::XRAYS[pos][jank::bit::lsb(b)];

            attacks |= attacks1;
        }

    return attacks;
}

bool Board::IsKingSafe(U64 tempOccuracyBoard, U64 tempEnemyBoard, U64 tempKingBoard) const {
    if (tempKingBoard == 0)
        return true;
    U64 kingPosIndex = jank::bit::lsb_pop(tempKingBoard);

    U64 enemyRooks =
        (GetPiecePos(PieceType::Rook) | GetPiecePos(PieceType::Queen)) & tempEnemyBoard;
    U64 enemyBishops =
        (GetPiecePos(PieceType::Bishop) | GetPiecePos(PieceType::Queen)) & tempEnemyBoard;
    U64 enemyKnights = GetPiecePos(PieceType::Knight) & tempEnemyBoard;
    U64 enemyPawns = GetPiecePos(PieceType::Pawn) & tempEnemyBoard;

    // clang-format off
    if (BitShift::RAYS[kingPosIndex][(int)DirectionIndex::North] & enemyRooks) [[unlikely]]
        if (jank::bit::lsb(BitShift::RAYS[kingPosIndex][(int)DirectionIndex::North] & enemyRooks) == jank::bit::lsb(BitShift::RAYS[kingPosIndex][(int)DirectionIndex::North] & tempOccuracyBoard))
            return false;
    if (BitShift::RAYS[kingPosIndex][(int)DirectionIndex::East] & enemyRooks) [[unlikely]]
        if (jank::bit::lsb(BitShift::RAYS[kingPosIndex][(int)DirectionIndex::East] & enemyRooks) == jank::bit::lsb(BitShift::RAYS[kingPosIndex][(int)DirectionIndex::East] & tempOccuracyBoard))
            return false;
    if (BitShift::RAYS[kingPosIndex][(int)DirectionIndex::South] & enemyRooks) [[unlikely]]
        if (jank::bit::msb(BitShift::RAYS[kingPosIndex][(int)DirectionIndex::South] & enemyRooks) == jank::bit::msb(BitShift::RAYS[kingPosIndex][(int)DirectionIndex::South] & tempOccuracyBoard))
            return false;
    if (BitShift::RAYS[kingPosIndex][(int)DirectionIndex::West] & enemyRooks) [[unlikely]]
        if (jank::bit::msb(BitShift::RAYS[kingPosIndex][(int)DirectionIndex::West] & enemyRooks) == jank::bit::msb(BitShift::RAYS[kingPosIndex][(int)DirectionIndex::West] & tempOccuracyBoard))
            return false;

    if (BitShift::RAYS[kingPosIndex][(int)DirectionIndex::NorthEast] & enemyBishops) [[unlikely]]
        if (jank::bit::lsb(BitShift::RAYS[kingPosIndex][(int)DirectionIndex::NorthEast] & enemyBishops) == jank::bit::lsb(BitShift::RAYS[kingPosIndex][(int)DirectionIndex::NorthEast] & tempOccuracyBoard))
            return false;
    if (BitShift::RAYS[kingPosIndex][(int)DirectionIndex::NorthWest] & enemyBishops) [[unlikely]]
        if (jank::bit::lsb(BitShift::RAYS[kingPosIndex][(int)DirectionIndex::NorthWest] & enemyBishops) == jank::bit::lsb(BitShift::RAYS[kingPosIndex][(int)DirectionIndex::NorthWest] & tempOccuracyBoard))
            return false;
    if (BitShift::RAYS[kingPosIndex][(int)DirectionIndex::SouthEast] & enemyBishops) [[unlikely]]
        if (jank::bit::msb(BitShift::RAYS[kingPosIndex][(int)DirectionIndex::SouthEast] & enemyBishops) == jank::bit::msb(BitShift::RAYS[kingPosIndex][(int)DirectionIndex::SouthEast] & tempOccuracyBoard))
            return false;
    if (BitShift::RAYS[kingPosIndex][(int)DirectionIndex::SouthWest] & enemyBishops) [[unlikely]]
        if (jank::bit::msb(BitShift::RAYS[kingPosIndex][(int)DirectionIndex::SouthWest] & enemyBishops) == jank::bit::msb(BitShift::RAYS[kingPosIndex][(int)DirectionIndex::SouthWest] & tempOccuracyBoard))
            return false;
    // clang-format on

    if (BitShift::MOVES[(int)PieceType::Knight][kingPosIndex] & enemyKnights) [[unlikely]]
        return false;

    if (PawnAttacks[(int)turn][kingPosIndex] & enemyPawns) [[unlikely]]
        return false;

    return true;
}
