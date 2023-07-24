#include <chess/export.hpp>

namespace Chess::Export {
std::string FEN(const Board &board) {
    std::string fen = "";

    for (int y = HEIGHT - 1; y >= 0; --y) {
        int count = 0;
        for (int x = 0; x < WIDTH; ++x) {
            const Square sq = Utilities::GetSquare(x, y);
            const PieceType piece = board.GetType(sq);
            if (piece == PieceType::None)
                ++count;
            else {
                if (count != 0) {
                    fen += std::to_string(count);
                    count = 0;
                }
                const Color color = board.GetColor(sq);
                fen += (char)Utilities::GetPieceChar(piece, color);
            }
        }
        if (count != 0)
            fen += std::to_string(count);
        if (y != 0)
            fen += "/";
    }

    if (board.GetColor() == Color::White)
        fen += " w ";
    else
        fen += " b ";

    bool anyCastling = false;
    auto addCastlingToFen = [&](char c) {
        fen += c;
        anyCastling = true;
    };
    if (board.IsCastlingAllowed(Color::White, Castling::King))
        addCastlingToFen('K');
    if (board.IsCastlingAllowed(Color::White, Castling::Queen))
        addCastlingToFen('Q');
    if (board.IsCastlingAllowed(Color::Black, Castling::King))
        addCastlingToFen('k');
    if (board.IsCastlingAllowed(Color::Black, Castling::Queen))
        addCastlingToFen('q');
    if (!anyCastling)
        fen += '-';

    fen += ' ';

    return fen;
}
} // namespace Export
