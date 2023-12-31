#include "chess/internal/utilities.hpp"
#include <chess/export.hpp>

namespace Chess::Export {
std::string FEN(const Position &pos) {
    std::string fen = "";

    for (int y = HEIGHT - 1; y >= 0; --y) {
        int count = 0;
        for (int x = 0; x < WIDTH; ++x) {
            const Square sq = Utilities::GetSquare(x, y);
            const PieceType piece = pos.GetType(sq);
            if (piece == PieceType::None)
                ++count;
            else {
                if (count != 0) {
                    fen += std::to_string(count);
                    count = 0;
                }
                const Color color = pos.GetColor(sq);
                fen += (char)Utilities::GetPieceChar(piece, color);
            }
        }
        if (count != 0)
            fen += std::to_string(count);
        if (y != 0)
            fen += "/";
    }

    if (pos.GetTurn() == Color::White)
        fen += " w ";
    else
        fen += " b ";

    bool anyCastling = false;
    auto addCastlingToFen = [&](char c) {
        fen += c;
        anyCastling = true;
    };
    if (pos.AllowsCastling(Castling::King, Color::White))
        addCastlingToFen('K');
    if (pos.AllowsCastling(Castling::Queen, Color::White))
        addCastlingToFen('Q');
    if (pos.AllowsCastling(Castling::King, Color::Black))
        addCastlingToFen('k');
    if (pos.AllowsCastling(Castling::Queen, Color::Black))
        addCastlingToFen('q');
    if (!anyCastling)
        fen += '-';

    fen += ' ';

    return fen;
}
} // namespace Chess::Export
