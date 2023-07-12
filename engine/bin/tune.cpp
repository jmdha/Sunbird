#include <iostream>
#include <cassert>
#include <fstream>
#include <memory>

#include <chess/internal/bit_shifts.hpp>
#include <chess/internal/constants.hpp>
#include <chess/board.hpp>
#include <chess/board_importer.hpp>
#include <engine/internal/positions.hpp>
#include <engine/minimax.hpp>
#include <engine/evaluator.hpp>

// How much each parameter is changed for a given iteration
// e.g. pawn value(0.2): 100 -> 120
constexpr double delta = 0.2;

constexpr double applyFactor = 0.1;

constexpr int iterationCount = 100;
constexpr int posCount = 10;

std::array<int, PIECECOUNT> staPieceValues {pieceValues};
std::array<int, PIECECOUNT> posPieceValues {pieceValues};
std::array<int, PIECECOUNT> negPieceValues {pieceValues};

void ModPieceValues(int count) {
    std::random_device dev;
    std::mt19937 rng(dev());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0,PIECECOUNT - 1); // All pieces except king
    posPieceValues = staPieceValues;
    negPieceValues = staPieceValues;
    for (int i = 0; i < count; i++) {
        int index = (int) dist(rng);
        posPieceValues.at(index) += (int)((double) posPieceValues.at(index) * delta);
        negPieceValues.at(index) -= (int)((double) negPieceValues.at(index) * delta);
    }

}

int main() {
    BitShifts::Init();
    std::shared_ptr<Board> board = std::make_shared<Board>();
    printf("Creating log file at \"./tuning_log.csv\"\n");
    std::ofstream log;
    log.open("tuning_log.csv");
    log << "pawn_val,knight_val,bishop_val,rook_val,queen_val,king_val\n";
    log.close();
    printf("--Running Tests--\n");
    const double totalGames = iterationCount * COLORCOUNT * posCount;
    for (int iteration = 0; iteration < iterationCount; iteration++) {
        ModPieceValues(3);
        std::vector<MiniMax*> engines
                { new MiniMax(board, Evaluator(posPieceValues)),
                  new MiniMax(board, Evaluator(negPieceValues)) };
        auto positions = Positions::GetPositions(posCount);
        std::vector<U64> winCount{0, 0};
        for (int i = 0; i < 2; i++)
            for (int posI = 0; posI < positions.size(); posI++) {
                double progress = (iteration * COLORCOUNT * posCount + i * posCount + posI) / totalGames * 100;
                //printf("%f\n", progress);
                auto position = positions.at(posI);
                *board = BoardImporter::ImportFEN(position);
                bool gameOver = false;
                bool draw = false;
                while (!gameOver) {
                    Move move;
                    if ((int) board->GetColor() == i)
                        move = engines.at(0)->GetBestMove();
                    else
                        move = engines.at(1)->GetBestMove();
                    if (move.GetType() == MoveType::SPECIAL_DRAW)
                        draw = true;
                    if (move.GetValue() != 0 && move.GetType() != MoveType::SPECIAL_DRAW) {
                        board->DoMove(move);
                    } else
                        gameOver = true;
                }
                if (!draw) {
                    if ((int) board->GetOppColor() == i)
                        winCount.at(0)++;
                    else
                        winCount.at(1)++;
                }
            }
        log.open("tuning_log.csv", std::ios_base::app);
        for (int i = 0; i < PIECECOUNT; i++) {
            if (winCount[0] > winCount[1])
                staPieceValues[i] += (int) ((double) (posPieceValues[i] - staPieceValues[i]) * applyFactor);
            else if (winCount[0] < winCount[1])
                staPieceValues[i] += (int) ((double) (negPieceValues[i] - staPieceValues[i]) * applyFactor);
            log << staPieceValues[i];
            if (i != PIECECOUNT - 1)
                log << ",";
        }
        log << "\n";
        log.close();
    }
    printf("\nFinal values:\n");
    for (int i = 0; i < PIECECOUNT; i++)
        printf("%d - %d\n", i, staPieceValues.at(i));
}
