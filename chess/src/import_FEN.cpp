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

    std::array<Castling, 2> castling{Castling::None, Castling::None};
    while (FEN[0] != ' ') {
        switch (FEN[0]) {
        case 'K':
            castling[(int)Color::White] = castling[(int)Color::White] ^ Castling::King;
            break;
        case 'k':
            castling[(int)Color::Black] = castling[(int)Color::Black] ^ Castling::King;
            break;
        case 'Q':
            castling[(int)Color::White] = castling[(int)Color::White] ^ Castling::Queen;
            break;
        case 'q':
            castling[(int)Color::Black] = castling[(int)Color::Black] ^ Castling::Queen;
            break;
        }
        FEN.erase(0, 1);
    }
    FEN.erase(0, 1);
    board.PushCastling(castling);

    // import en-passant
    if (FEN.size() > 0 && FEN[0] != '-' && FEN[0] != ' ') {
        auto sq = Utilities::GetSquare(FEN[0], FEN[1]);
        board.PushEP(Utilities::GetColumn(sq));
    }

    return board;
}
} // namespace Chess::Import
