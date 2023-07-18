#ifndef IMPORT
#define IMPORT

#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

#include "internal/constants.hpp"
#include "internal/utilities.hpp"
#include "internal/move.hpp"
#include "board.hpp"

namespace Import {
    // Format: rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1
    static Board FEN(std::string FEN) {
        Board board;

        // import position
        for(int y = HEIGHT - 1; y >= 0; y--) {
            int remainingSquares = WIDTH;
            while(remainingSquares > 0) {
                if(isdigit(FEN[0]))
                    remainingSquares -= (int) FEN[0] - 48;
                else
                    board.PlacePiece(Utilities::GetSquare(WIDTH - (remainingSquares--), y), (PieceChar) FEN[0]);

                FEN.erase(0, 1);
            }
            FEN.erase(0, 1);
        }

        // import turn
        board.SetTurn(Utilities::GetTurn(FEN[0]));
        FEN.erase(0, 2);

        while(FEN[0] != ' ') {
            switch (FEN[0]) {
                case 'K': board.EnableCastling(Color::White, Castling::King);  break;
                case 'k': board.EnableCastling(Color::Black, Castling::King);  break;
                case 'Q': board.EnableCastling(Color::White, Castling::Queen); break;
                case 'q': board.EnableCastling(Color::Black, Castling::Queen); break;
            }
            FEN.erase(0, 1);
        }
        FEN.erase(0, 1);

        // import en-passant
        if (FEN[0] != '-') {
            auto sq = Utilities::GetSquare(FEN[0], FEN[1]);
            board.SetEnPassant(Utilities::GetColumn(sq));
        }

        return board;
    }

    // Format: d2d4 e7e5 etc
    // HACK: Refactor this
    static Board MoveSequence(std::string moves) {
        Board board;
        board.Initialize();
        std::string move;
        for (int i = 0; i < moves.length(); i++) {
            if (moves[i] != ' ')
                move.push_back(moves[i]);

            if (moves[i] == ' ' || i == (int) moves.length() - 1) {
                Square fromSquare = Utilities::GetSquare(move[0], move[1]);
                Square toSquare = Utilities::GetSquare(move[2], move[3]);
                PieceType fromType = board.GetType(fromSquare);
                PieceType toType = board.GetType(toSquare);
                Color fromColor = board.GetColor(fromSquare);
                MoveType type;
                // Is castling?
                //// Is kingside castle
                if (fromType == PieceType::King && (move == "e1g1" || move == "e8g8"))
                    type = MoveType::KingCastle;
                //// Is queenside castle
                else if (fromType == PieceType::King && (move == "e1c1" || move == "e8c8"))
                    type = MoveType::QueenCastle;

                else if (fromType == PieceType::Pawn) {
                    // If double pawn push
                    if (std::abs((int) Utilities::GetRowIndex(fromSquare) - (int) Utilities::GetRowIndex(toSquare)) == 2)
                        type = MoveType::DoublePawnPush;
                    // If promotion
                    else if (Utilities::GetRow(toSquare) == ((fromColor == Color::White) ? Row::Row8 : Row::Row1)) {
                        // If none capture promotion
                        if (toType == PieceType::None) {
                            if (moves.length() == 5) {
                                if (move[4] == 'r')
                                    type = MoveType::RPromotion;
                                else if (move[4] == 'n')
                                    type = MoveType::NPromotion;
                                else if (move[4] == 'b')
                                    type = MoveType::BPromotion;
                            } else
                                type = MoveType::QPromotion;
                        } else {
                            // Capture promotion
                            if (moves.length() == 5) {
                                if (move[4] == 'r')
                                    type = MoveType::RPromotionCapture;
                                else if (move[4] == 'n')
                                    type = MoveType::NPromotionCapture;
                                else if (move[4] == 'b')
                                    type = MoveType::BPromotionCapture;
                            } else
                                type = MoveType::QPromotionCapture;
                        }
                    } else if (toType != PieceType::None)
                        type = MoveType::Capture; 
                    else {
                        if (Utilities::GetColumn(fromSquare) != Utilities::GetColumn(toSquare))
                            type = MoveType::EPCapture;
                        else
                            type = MoveType::Quiet;
                    }
                } else if (toType != PieceType::None)
                    type = MoveType::Capture; 
                else
                    type = MoveType::Quiet;
                Move tempMove;
                if (type == MoveType::Capture || type == MoveType::RPromotionCapture || type == MoveType::NPromotionCapture || type == MoveType::BPromotionCapture || type == MoveType::QPromotionCapture)
                    tempMove = Move(type, fromSquare, toSquare, board.GetType(toSquare));
                else
                    tempMove = Move(type, fromSquare, toSquare);
                board.DoMove(tempMove);
                move = "";
            }
        }
        return board;
    }

    // https://en.wikipedia.org/wiki/Algebraic_notation_(chess)
    static Board Algebraic(std::string game) {
        Board board;
        board.Initialize();
        
        // Clean moves list
        // 1. Nf3 Nf6 2. c4 etc -> Nf3 Nf6 c4
        size_t pos;
        while ((pos = game.find('.')) != std::string::npos) {
            auto delimFinder = [game, pos](int inc){
                size_t tempPos = pos;
                while (game[tempPos] != ' ' && 
                       tempPos != 0 &&
                       tempPos != game.size())
                    tempPos += inc;
                return tempPos;
            };
            const size_t startPos = delimFinder(-1);
            const size_t endPos = delimFinder(1);
            game.erase(startPos, endPos - startPos);
        }
        game.erase(0, 1);

        // Remove check and checkmate markers
        while((pos = game.find('#')) != std::string::npos)
            game.erase(pos, 1);
        while((pos = game.find('+')) != std::string::npos)
            game.erase(pos, 1);

        // Tokenize
        // Nf3 Nf6 c4 -> [Nf3, Nf6, c4]
        std::vector<std::string> tokens;
        std::istringstream ss(game);
        std::string s;
        while (std::getline(ss, s, ' '))
            tokens.push_back(s);
        
        // Parse
        Color color = Color::Black;
        std::vector<Move> moves;
        for (auto token : tokens) {
            color = Utilities::GetOppositeColor(color);

            if (token == "O-O")
                moves.push_back(Move(MoveType::KingCastle));
            else if (token == "O-O-O")
                moves.push_back(Move(MoveType::QueenCastle));

            bool isCapture = token.find('x') != std::string::npos;
            if (isCapture) { auto pos = token.find('x'); token.erase(pos, 1); }

            PieceType pType;
            switch (token[0]) {
                case 'K': pType = PieceType::King;   break;
                case 'Q': pType = PieceType::Queen;  break;
                case 'R': pType = PieceType::Rook;   break;
                case 'B': pType = PieceType::Bishop; break;
                case 'N': pType = PieceType::Knight; break;
                default:  pType = PieceType::Pawn;   break;
            }
            if (pType != PieceType::Pawn) token.erase(0, 1);
            printf("Move %s by %c\n", token.c_str(), (char) Utilities::GetPieceChar(pType, color));
        }

        // Do moves
        for (auto move : moves)
            board.DoMove(move);

        return board;
    }
};

#endif

