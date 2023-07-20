#include "chess/import.hpp"
#include "chess/internal/utilities.hpp"
#include "chess/move_gen.hpp"
#include <cctype>

namespace Import {
namespace {
// Removes unnecessary information from the game string
void Clean(std::string &game) {
    // 1. Nf3 Nf6 2. c4 etc -> Nf3 Nf6 c4
    size_t pos;
    while ((pos = game.find('.')) != std::string::npos) {
        auto delimFinder = [game, pos](int inc) {
            size_t tempPos = pos;
            while (game[tempPos] != ' ' && tempPos != 0 && tempPos != game.size())
                tempPos += inc;
            return tempPos;
        };
        const size_t startPos = delimFinder(-1);
        const size_t endPos = delimFinder(1);
        game.erase(startPos, endPos - startPos);
    }
    game.erase(0, 1);

    // Remove check and checkmate markers
    while ((pos = game.find('#')) != std::string::npos)
        game.erase(pos, 1);
    while ((pos = game.find('+')) != std::string::npos)
        game.erase(pos, 1);
}

// Splits game string into individual moves
// Nf3 Nf6 c4 -> [Nf3, Nf6, c4]
std::vector<std::string> Tokenize(const std::string &game) {
    std::vector<std::string> tokens;
    std::istringstream ss(game);
    std::string s;
    while (std::getline(ss, s, ' '))
        tokens.push_back(s);
    return tokens;
}

// Parses a move from a token
std::optional<Move> ParseToken(const Board &board, std::string token) {
    Color color = board.GetColor();
    color = Utilities::GetOppositeColor(color);

    if (token == "O-O")
        return Move(MoveType::KingCastle);
    else if (token == "O-O-O")
        return Move(MoveType::QueenCastle);

    bool isCapture = token.find('x') != std::string::npos;
    if (isCapture) {
        auto pos = token.find('x');
        token.erase(pos, 1);
    }

    bool isPromotion = token.find('=') != std::string::npos;
    std::optional<PieceType> promotionType;
    if (isPromotion) {
        auto pos = token.find('=');
        PieceChar pieceChar = (PieceChar)(char)token[pos + 1];
        promotionType = Utilities::GetPieceType(pieceChar);
        token.erase(pos, 2);
    }

    PieceType pType;
    switch (token[0]) {
    case 'K':
        pType = PieceType::King;
        break;
    case 'Q':
        pType = PieceType::Queen;
        break;
    case 'R':
        pType = PieceType::Rook;
        break;
    case 'B':
        pType = PieceType::Bishop;
        break;
    case 'N':
        pType = PieceType::Knight;
        break;
    default:
        pType = PieceType::Pawn;
        break;
    }
    if (pType != PieceType::Pawn)
        token.erase(0, 1);
    assert(token.size() < 4);
    std::optional<Square> toSquare;
    std::optional<Column> fromCol;
    if (token.size() == 3) {
        fromCol = Utilities::GetColumnByChar(token[0]);
        token.erase(0, 1);
    }
    if (token.size() == 2)
        toSquare = Utilities::GetSquare(token[0], token[1]);
    MoveList tempMoves = MoveGen::GenerateMoves<MoveGen::GenType::All>(board, pType);
    for (const auto tempMove : tempMoves) {
        if (fromCol.has_value() && (Utilities::GetColumn(tempMove.GetFrom()) != fromCol))
            continue;
        if (tempMove.IsCapture() == isCapture && tempMove.GetTo() == toSquare) {
            if (promotionType.has_value()) {
                switch (promotionType.value()) {
                    case PieceType::Knight:
                        if (tempMove.GetType() != (!isCapture ? MoveType::NPromotion : MoveType::NPromotionCapture))
                            continue;
                        break;
                    case PieceType::Bishop:
                        if (tempMove.GetType() != (!isCapture ? MoveType::BPromotion : MoveType::BPromotionCapture))
                            continue;
                        break;
                    case PieceType::Rook:
                        if (tempMove.GetType() != (!isCapture ? MoveType::RPromotion : MoveType::RPromotionCapture))
                            continue;
                        break;
                    case PieceType::Queen:
                        if (tempMove.GetType() != (!isCapture ? MoveType::QPromotion : MoveType::QPromotionCapture))
                            continue;
                        break;
                    default:
                        throw std::logic_error("Unexpected promotion type");
                }
            }
            return tempMove;
        }
    }
    return {};
}
} // namespace
Board Algebraic(std::string game) {
    Board board;
    board.Initialize();

    Clean(game);
    std::vector<std::string> tokens = Tokenize(game);

    // Parse
    for (auto token : tokens) {
        std::optional<Move> move = ParseToken(board, token);
        if (!move.has_value())
            throw std::logic_error("No matching move for \"" + token + "\" in game \"" + game + "\"");
        board.DoMove(move.value());
    }

    return board;
}

void Algebraic(std::string game, std::function<void(const Board&)> callback) {
    Board board;
    board.Initialize();

    Clean(game);
    std::vector<std::string> tokens = Tokenize(game);

    // Parse
    for (auto token : tokens) {
        std::optional<Move> move = ParseToken(board, token);
        if (!move.has_value())
            throw std::logic_error("No matching move for \"" + token + "\" in game \"" + game + "\"");
        board.DoMove(move.value());
        callback(board);
    }
}
} // namespace Import
