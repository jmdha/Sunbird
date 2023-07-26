#include "chess/internal/utilities.hpp"
#include "engine/evaluation.hpp"
#include <chess/import.hpp>
#include <engine/internal/positions.hpp>
#include <engine/internal/values.hpp>
#include <engine/negamax.hpp>
#include <filesystem>
#include <fstream>
#include <optional>
#include <string>

// How much each parameter is changed for a given iteration
// e.g. pawn value(0.2): 100 -> 120
constexpr double delta = 1.0;

constexpr double applyFactor = 0.1;

constexpr int posCount = 10;

std::random_device dev;
std::mt19937 rng(dev());
std::uniform_int_distribution<std::mt19937::result_type> mat_dist(0, 5); // All pieces except king
std::uniform_int_distribution<std::mt19937::result_type>
    square_dist(0, PIECECOUNT); // All pieces except king

enum class GameResult { WhiteWin, BlackWin, Draw };

using namespace Chess;
using namespace Chess::Engine;

std::array<std::array<int, 5>, 2> ModMaterialValues() {
    std::array<std::array<int, 5>, 2> values{MaterialValue::ALL, MaterialValue::ALL};

    for (int i = 0; i < 1; i++) {
        int pIndex = mat_dist(rng);
        double change = (int)((double)MaterialValue::ALL[pIndex] * delta);
        values[0][pIndex] += change;
        values[1][pIndex] -= change;
    }

    return values;
}

// TODO: Refactor this shit
int main() {
    printf("Running tuning\n");
    const std::string resultDir = std::filesystem::current_path().string() + "/tuning_results/";
    printf("Result dir \"%s\"\n", resultDir.c_str());
    std::filesystem::create_directory(resultDir);
    int iteration = 0;
    for (auto i : std::filesystem::directory_iterator(resultDir))
        iteration++;
    const std::string iterationDir = resultDir + std::to_string(iteration);
    printf("Iteration dir \"%s\"\n", iterationDir.c_str());
    std::filesystem::create_directory(iterationDir);
    size_t roundIndex = 1;
    while (true) {
        printf("Beginning round %zu\n", roundIndex);
        std::vector<std::string> positions = Positions::GetPositions(posCount);
        size_t winCount[2]{0};
        for (const auto pos : positions) {
            for (int i = 0; i < 2; i++) {
                Board board = Import::FEN(pos);
                do {
                    std::pair<std::optional<Move>, int> move;
                    if (board.GetColor() == (Color)i) {
                        Negamax::EVAL_FUNCTION = Evaluation::Eval;
                        move = Negamax::GetBestMove(board, 3);
                    } else {
                        Negamax::EVAL_FUNCTION = [](const Board &board) { return 0; };
                        move = Negamax::GetBestMove(board, 2);
                    }
                    if (move.first.has_value()) {
                        board.DoMove(move.first.value());
                    } else if (move.second == 0) {
                        break;
                    } else {
                        if (board.GetColor() != (Color)i)
                            winCount[0]++;
                        else
                            winCount[1]++;
                        break;
                    }
                } while (true);
            }
        }
        printf("Finished round: %zu - %zu\n", winCount[0], winCount[1]);
        if (winCount[0] != winCount[1]) {
            const std::string roundPath = iterationDir + std::to_string(roundIndex);
            printf("Writing result to file %s\n", roundPath.c_str());
            std::ofstream roundFile(roundPath);
            roundFile << "-----Material-----\n";
            roundFile << "pawn,knight,bishop,rook,queen,\n";
            roundFile << '\n';
            roundFile.close();
        }
    }
}
