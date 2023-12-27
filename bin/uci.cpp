#include <climits>
#include <iostream>
#include <optional>
#include <string>

#include <chess/board.hpp>
#include <chess/import.hpp>
#include <engine/internal/tt.hpp>
#include <engine/search.hpp>
#include <unordered_map>
#include <unordered_set>

using namespace Chess;

enum class Command { uci, isready, setoption, quit, position, ucinewgame, go, stop };

const std::unordered_map<std::string, Command> Commands{
    {"uci", Command::uci},   {"isready", Command::isready},   {"setoption", Command::setoption},
    {"quit", Command::quit}, {"position", Command::position}, {"ucinewgame", Command::ucinewgame},
    {"go", Command::go},     {"stop", Command::stop}};

enum class Option { hash };
const std::unordered_map<std::string, Option> Options{{"hash", Option::hash}};

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

int main() {
    Engine::TT::Init(32);
    Board board = Import::MoveSequence("");
    while (true) {
        std::string input;
        std::getline(std::cin, input);

        auto tokens = Tokenize(input);
        if (!Commands.contains(tokens[0])) {
            std::cout << "unknown command: " << tokens[0] << '\n';
            continue;
        }

        switch (Commands.at(tokens[0])) {
        case Command::uci:
            std::cout << "id name " << _PROJECT_NAME << " v" << _PROJECT_VERSION << '\n';
            std::cout << "id author " << _PROJECT_AUTHOR << '\n';
            for (const auto option : Options) {
                std::cout << "option " << option.first << '\n';
            }
            std::cout << "uciok" << '\n';
            break;
        case Command::setoption:
            if (!Options.contains(tokens[1])) {
                std::cout << "unknown option: " << tokens[1] << '\n';
                continue;
            }
            switch (Options.at(tokens[1])) {
            case Option::hash:
                if (tokens.size() < 3)
                    continue;
                auto hash_size = std::atoi(tokens[2].c_str());
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
            std::cout << "readyok" << '\n';
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
