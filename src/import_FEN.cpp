#include "import.hpp"
#include "position.hpp"
#include "utilities.hpp"

namespace Import {
Board FEN(std::string FEN) {
    Position pos;

    // import position
    for (int y = HEIGHT - 1; y >= 0; y--) {
        int remainingSquares = WIDTH;
        while (remainingSquares > 0) {
            if (isdigit(FEN[0]))
                remainingSquares -= (int)FEN[0] - 48;
            else {
                const Piece pType  = ToPiece(FEN[0]);
                const Color pColor = ToColor(FEN[0]);
                pos.PlacePiece(
                    Utilities::GetSquare(WIDTH - (remainingSquares--), y), pType, pColor
                );
            }

            FEN.erase(0, 1);
        }
        FEN.erase(0, 1);
    }

    // import turn
    pos.SetTurn(Utilities::GetTurn(FEN[0]));
    FEN.erase(0, 2);

    std::array<Castling, 2> castling{Castling::None, Castling::None};
    while (FEN[0] != ' ') {
        switch (FEN[0]) {
        case 'K': castling[(int)WHITE] = castling[(int)WHITE] ^ Castling::King; break;
        case 'k': castling[(int)BLACK] = castling[(int)BLACK] ^ Castling::King; break;
        case 'Q': castling[(int)WHITE] = castling[(int)WHITE] ^ Castling::Queen; break;
        case 'q': castling[(int)BLACK] = castling[(int)BLACK] ^ Castling::Queen; break;
        }
        FEN.erase(0, 1);
    }
    FEN.erase(0, 1);
    for (auto color : {WHITE, BLACK}) {
        if (!((int)castling[(int)color] & (int)Castling::King))
            pos.DisallowCastling(Castling::King, color);
        if (!((int)castling[(int)color] & (int)Castling::Queen))
            pos.DisallowCastling(Castling::Queen, color);
    }

    // import en-passant
    if (FEN.size() > 0 && FEN[0] != '-' && FEN[0] != ' ') {
        auto sq = Utilities::GetSquare(FEN[0], FEN[1]);
        pos.SetEP(Utilities::GetColumn(sq));
    } else {
        pos.SetEP(Column::None);
    }

    return Board(pos);
}
} // namespace Import
