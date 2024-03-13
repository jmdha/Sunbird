#include <board.hpp>
#include <ios>
#include <iostream>
#include <ostream>
#include <search.hpp>
#include <sstream>
#include <string>
#include <tt.hpp>

int main(int argc, char **argv) {
    TT::Init();
    Board board = Board();

    std::string command;
    std::string token;
    while (std::getline(std::cin, command)) {
        std::istringstream is(command);
        is >> std::skipws >> token;
        if (token == "quit")
            break;
        else if (token == "isready")
            std::cout << "readyok" << std::endl;
        else if (token == "uci") {
            std::cout << "id name " << _NAME << " v" << _VERSION << std::endl;
            std::cout << "id author " << _AUTHOR << std::endl;
            std::cout << "option name Hash type spin default 32 min 1 max 512" << std::endl;
            std::cout << "option name Threads type spin default 1 min 1 max 1" << std::endl;
            std::cout << "uciok" << std::endl;
            std::flush(std::cout);
        } else if (token == "ucinewgame") {
            TT::Clear();
        } else if (token == "position") {
            is >> std::skipws >> token;
            std::string fen;
            if (token == "startpos")
                fen = FEN_START;
            else if (token == "fen") {
                fen = "";
                for (int i = 0; i < 6; i++) {
                    is >> std::skipws >> token;
                    fen += token + " ";
                }
            }
            board = Board(fen);
            is >> std::skipws >> token;
            if (token == "moves") {
                while (is >> std::skipws >> token)
                    board.ApplyMove(
                        Move(board.Pieces(), board.Pieces(KING), board.Pieces(PAWN), token)
                    );
            }
        } else if (token == "go") {
            std::size_t time = 60000000;
            is >> std::skipws >> token;
            while (token != "endl") {
                if (token == "wtime" || token == "btime") {
                    if ((board.Turn() == WHITE && token == "wtime") ||
                        (board.Turn() == BLACK && token == "btime")) {
                        is >> std::skipws >> token;
                        time = 0.05 * std::stoi(token);
                    } else {
                        is >> std::skipws >> token;
                    }
                    is >> std::skipws >> token;
                    continue;
                } else if (token == "winc" || token == "binc") {
                    is >> std::skipws >> token;
                    is >> std::skipws >> token;
                    continue;
                }
                token = "endl";
            }
            const Move move = Search::GetBestMoveTime(board, time);
            std::cout << "bestmove " << move.Export() << std::endl;
        }
    }
}
