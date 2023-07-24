#include <chess/import.hpp>
#include <chess/internal/utilities.hpp>

namespace Chess::Import {
Board FEN(std::string FEN) {
    Board board;

    // import position
    for (int y = HEIGHT - 1; y >= 0; y--) {
        int remainingSquares = WIDTH;
        while (remainingSquares > 0) {
            if (isdigit(FEN[0]))
                remainingSquares -= (int)FEN[0] - 48;
            else
                board.PlacePiece(Utilities::GetSquare(WIDTH - (remainingSquares--), y),
                                 (PieceChar)FEN[0]);

            FEN.erase(0, 1);
        }
        FEN.erase(0, 1);
    }

    // import turn
    board.SetTurn(Utilities::GetTurn(FEN[0]));
    FEN.erase(0, 2);

    while (FEN[0] != ' ') {
        switch (FEN[0]) {
        case 'K':
            board.EnableCastling(Color::White, Castling::King);
            break;
        case 'k':
            board.EnableCastling(Color::Black, Castling::King);
            break;
        case 'Q':
            board.EnableCastling(Color::White, Castling::Queen);
            break;
        case 'q':
            board.EnableCastling(Color::Black, Castling::Queen);
            break;
        }
        FEN.erase(0, 1);
    }
    FEN.erase(0, 1);

    // import en-passant
    if (FEN.size() > 0 && FEN[0] != '-' && FEN[0] != ' ') {
        auto sq = Utilities::GetSquare(FEN[0], FEN[1]);
        board.SetEnPassant(Utilities::GetColumn(sq));
    }

    return board;
}
} // namespace Chess::Import
