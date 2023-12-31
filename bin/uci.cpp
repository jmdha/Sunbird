#include "engine/internal/tt.hpp"
#include <climits>
#include <iostream>
#include <string>

#include <chess/board.hpp>
#include <chess/import.hpp>
#include <engine/search.hpp>
#include <unordered_map>

using namespace Chess;

const std::vector<std::string> POSITIONS{
    "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1",                 // Init,
    "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ",        // Kiwipete
    "8/2p5/3p4/KP5r/1R3p1k/8/4P1P1/8 w - - ",                                   // NoCastle
    "r3k2r/Pppp1ppp/1b3nbN/nP6/BBP1P3/q4N2/Pp1P2PP/R2Q1RK1 w kq - 0 1",         // Position 4
    "rnbq1k1r/pp1Pbppp/2p5/8/2B5/8/PPP1NnPP/RNBQK2R w KQ - 1 8  ",              // Position 5
    "r4rk1/1pp1qppp/p1np1n2/2b1p1B1/2B1P1b1/P1NP1N2/1PP1QPPP/R4RK1 w - - 0 10 " // Position 6
};

enum class Command { uci, isready, setoption, quit, position, ucinewgame, go, stop };

const std::unordered_map<std::string, Command> Commands{
    {"uci", Command::uci},   {"isready", Command::isready},   {"setoption", Command::setoption},
    {"quit", Command::quit}, {"position", Command::position}, {"ucinewgame", Command::ucinewgame},
    {"go", Command::go},     {"stop", Command::stop}};

enum class Option { hash };
const std::unordered_map<std::string, Option> Options{{"Hash", Option::hash}};

// There must a better way to tokenize in cpp (excluding boost)
std::vector<std::string> Tokenize(std::string s) {
    std::vector<std::string> tokens;
NEXT_TOKEN:
    bool inApro = (s[0] == '"');
    if (auto p = s.find_first_of((inApro) ? '"' : ' ', (inApro) ? 1 : 0); p != std::string::npos) {
        tokens.push_back(s.substr((inApro) ? 1 : 0, inApro ? p - 1 : p));
        s = s.substr(p + 1, s.size() - p);
        if (!s.empty())
            goto NEXT_TOKEN;
    }
    if (!s.empty())
        tokens.push_back(s);
    return tokens;
}

int main(int argc, char **argv) {
    Engine::TT::Init();
    if (argc > 1 && std::string(argv[1]) == "bench") {
        size_t nodes = 0;
        size_t time = 0;
        for (const auto &pos : POSITIONS) {
            Board board = Import::FEN(pos);

            std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
            Engine::Search::GetBestMoveDepth(board, 5);
            std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
            nodes += board.MoveCount();
            size_t t = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
            time += t;
        }
        std::cout << nodes << " nodes " << nodes * 1000 / time << " nps" << '\n';
        exit(0);
    }
    Board board = Import::MoveSequence("");
    while (true) {
        std::string input;
        std::getline(std::cin, input);

        auto tokens = Tokenize(input);
        if (!Commands.contains(tokens[0])) {
            printf("unknown command: %s\n", tokens[0].c_str());
            continue;
        }

        switch (Commands.at(tokens[0])) {
        case Command::uci:
            std::cout << "id name " << _PROJECT_NAME << " v" << _PROJECT_VERSION << '\n';
            std::cout << "id author " << _PROJECT_AUTHOR << "\n\n";
            std::cout << "option name Hash type spin default 32 min 1 max 512" << '\n';
            std::cout << "option name Threads type spin default 1 min 1 max 1" << '\n';
            std::cout << "uciok" << '\n';
            std::flush(std::cout);
            break;
        case Command::setoption:
            if (!Options.contains(tokens[2])) {
                std::cout << "unknown option: " << tokens[1] << '\n';
                continue;
            }
            switch (Options.at(tokens[2])) {
            case Option::hash:
                if (tokens.size() < 3)
                    continue;
                auto hash_size = std::atoi(tokens[4].c_str());
                if (hash_size == 0) {
                    std::cout << "hash size cannot be 0" << '\n';
                    continue;
                }
                Engine::TT::Clean();
                Engine::TT::Init(hash_size);
                break;
            }
            break;
        case Command::ucinewgame:
            Engine::TT::Clear();
            break;
        case Command::isready:
            printf("readyok\n");
            break;
        case Command::position:
            if (tokens[1] == "startpos") {
                board = Import::MoveSequence("");
            } else if (tokens[1] == "fen") {
                auto fenIndex = input.find("fen") + 4;
                auto movesIndex = input.find("moves");
                std::string fen = input.substr(fenIndex, (movesIndex == std::string::npos)
                                                             ? input.size() - fenIndex
                                                             : movesIndex - fenIndex - 1);
                board = Import::FEN(fen);
            } else
                throw std::logic_error("Unexpected token in position: \"" + tokens[1] + '\"');

            if (auto p = input.find("moves"); p != std::string::npos)
                Import::MoveSequence(board, input.substr(p + 6, input.size() - p - 6));

            break;
        case Command::go: {
            int searchTime = INT_MAX;

            for (size_t i = 0; i < tokens.size(); i++) {
                auto token = tokens[i];
                if ((board.Pos().GetTurn() == Color::White && token == "wtime") ||
                    (board.Pos().GetTurn() == Color::Black && token == "btime")) {
                    searchTime = std::stoi(tokens[i + 1]);
                    searchTime = static_cast<int>(searchTime * 0.05);
                } else if (token == "movetime")
                    searchTime = std::stoi(tokens[i + 1]);
            }

            Move move = std::get<Move>(Engine::Search::GetBestMoveTime(board, searchTime));
            std::cout << "bestmove " << move.ToString() << '\n';

            break;
        }
        case Command::quit:
            Engine::TT::Clean();
            return 0;
        default:
            throw std::logic_error("The impossible has happended: " + input);
        }
    }
}
