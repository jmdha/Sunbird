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

constexpr int posCount = 20;

std::random_device dev;
std::mt19937 rng(dev());
std::uniform_int_distribution<std::mt19937::result_type> mat_dist(0, 5); // All pieces except king
std::uniform_int_distribution<std::mt19937::result_type>
    square_dist(0, PIECECOUNT); // All pieces except king

enum class GameResult { WhiteWin, BlackWin, Draw };

using namespace Chess;
using namespace Chess::Engine;

std::array<std::array<int, 5>, 2> ModMaterialValues() {
    std::array<std::array<int, 5>, 2> values{MATERIAL_VALUE, MATERIAL_VALUE};

    for (int i = 0; i < 1; i++) {
        int pIndex = mat_dist(rng);
        double change = (int)((double)MATERIAL_VALUE[pIndex] * delta);
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
        const auto priorValues = MATERIAL_VALUE;
        const auto materialValues =
            std::array<std::array<int, 5>, 2>{0, 0, 0, 0, 0, 100, 300, 300, 500, 900};
        std::vector<std::string> positions = Positions::GetPositions(posCount);
        int winCount[2]{0};
        for (int i = 0; i < 2; i++) {
            bool t = i;
            for (const auto pos : positions) {
                Board board = Import::FEN(pos);
                bool draw = false;
                do {
                    MATERIAL_VALUE = materialValues[t = !t];
                    Move move = Negamax::GetBestMove(board, 2);
                    if (move.GetType() == MoveType::SPECIAL_DRAW) {
                        draw = true;
                        break;
                    } else if (move.GetValue() == 0)
                        board.DoMove(move);
                    else
                        break;
                } while (true);
                if (!draw) {
                    if ((int)board.GetOppColor() == i)
                        winCount[0]++;
                    else
                        winCount[1]++;
                }
            }
        }
        MATERIAL_VALUE = priorValues;
        printf("Finished round: %d - %d\n", winCount[0], winCount[1]);
        if (winCount[0] != winCount[1]) {
            MATERIAL_VALUE = materialValues[winCount[1] > winCount[0]];
            const std::string roundPath = iterationDir + std::to_string(roundIndex);
            printf("Writing result to file %s\n", roundPath.c_str());
            std::ofstream roundFile(roundPath);
            roundFile << "-----Material-----\n";
            roundFile << "pawn,knight,bishop,rook,queen,\n";
            for (const auto value : MATERIAL_VALUE)
                roundFile << value << ',';
            roundFile << '\n';
            roundFile.close();
        }
    }
}
