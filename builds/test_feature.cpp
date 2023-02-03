#include <iostream>
#include <cassert>
#include <fstream>

#include "bit_shifts.hh"
#include "board.hh"
#include "constants.hh"
#include "evaluator.hh"
#include "minimax.hh"
#include "board_importer.hh"
#include "positions.hh"
#include "../include/indicators/indicators.hpp"

constexpr int posCount = 1;

int main(int argc, char* argv[]) {
    BitShifts::Init();
    Board board = Board();
    indicators::ProgressBar bar{
            indicators::option::BarWidth{50},
            indicators::option::Start{"["},
            indicators::option::Fill{"■"},
            indicators::option::Lead{"■"},
            indicators::option::Remainder{"-"},
            indicators::option::End{"]"},
            indicators::option::ShowPercentage{true},
            indicators::option::ShowElapsedTime{true},
            indicators::option::ShowRemainingTime{true}
    };
    printf("--Running Tests--\n");
    const double totalGames = COLORCOUNT * posCount;
    std::vector<MiniMax *> engines
            {new MiniMax(&board), new MiniMax(&board, Evaluator(true))};
    auto positions = Positions::GetPositions(posCount);
    std::vector<U64> winCount{0, 0};
    for (int i = 0; i < 2; i++)
        for (int posI = 0; posI < positions.size(); posI++) {
            double progress = (i * posCount + posI) / totalGames * 100;
            bar.set_progress((unsigned long) progress);
            auto position = positions.at(posI);
            BoardImporter::ImportFEN(&board, position);
            bool gameOver = false;
            bool draw = false;
            while (!gameOver) {
                Move move;
                if ((int) board.GetColor() == i)
                    move = engines.at(0)->GetBestMove();
                else
                    move = engines.at(1)->GetBestMove();
                if (move.GetType() == MoveType::SPECIAL_DRAW)
                    draw = true;
                if (move.GetValue() != 0 && move.GetType() != MoveType::SPECIAL_DRAW) {
                    board.DoMove(move);
                } else
                    gameOver = true;
            }
            if (!draw) {
                if ((int) board.GetOppColor() == i)
                    winCount.at(0)++;
                else
                    winCount.at(1)++;
            }
        }
    bar.set_progress(100);
    printf("\nNew feature won %f%% of games(%llu/%llu)\n", (double) winCount.at(1) / totalGames * 100, winCount.at(1), (U64) totalGames);
}
