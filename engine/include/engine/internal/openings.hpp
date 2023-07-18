#ifndef OPENINGS_HH
#define OPENINGS_HH

#include <unordered_map>
#include <unordered_set>
#include <optional>
#include <vector>

#include <chess/internal/constants.hpp>
#include <chess/internal/move.hpp>

namespace OpeningBook {
    static inline std::unordered_map<U64, std::vector<Move>> moves{
            // v|Start pos
            {8350766115355486690LLU,  {
                Move(MoveType::DoublePawnPush, Square::E2, Square::E4),
                Move(MoveType::Quiet, Square::D2, Square::D4)}},
            // v|Queens pawn - rnbqkbnr/pppppppp/8/8/3P4/8/PPP1PPPP/RNBQKBNR b KQkq - 0 1
            {6421916850053666185LLU,  {
                Move(MoveType::Quiet, Square::G8, Square::F6),
                Move(MoveType::DoublePawnPush, Square::D7, Square::D5),
                Move(MoveType::Quiet, Square::D7, Square::D6)}},
#pragma region KingsPawn
            // v|Kings pawn - rnbqkbnr/pppppppp/8/8/4P3/8/PPPP1PPP/RNBQKBNR b KQkq - 0 1
            {10094012945678312991LLU, {
                Move(MoveType::DoublePawnPush, Square::C7, Square::C6),
                Move(MoveType::DoublePawnPush, Square::G8, Square::F6),
                Move(MoveType::DoublePawnPush, Square::C7, Square::C5),
                Move(MoveType::DoublePawnPush, Square::D7, Square::D5),
                Move(MoveType::DoublePawnPush, Square::E7, Square::E5)}},
                // v|Sicilian Defence - rnbqkbnr/pp1ppppp/8/2p5/4P3/8/PPPP1PPP/RNBQKBNR w KQkq - 0 2
                {5211104197556890052LLU,  {
                    Move(MoveType::Quiet, Square::G1, Square::F3),
                    Move(MoveType::Quiet, Square::B1, Square::C3),
                    Move(MoveType::DoublePawnPush, Square::D2, Square::D4),
                    Move(MoveType::Quiet, Square::C2, Square::C3)}},
                // v|Open Game - rnbqkbnr/pppp1ppp/8/4p3/4P3/8/PPPP1PPP/RNBQKBNR w KQkq - 0 2
                {14242943004056214401LLU, {
                    Move(MoveType::Quiet, Square::G1, Square::F3)}},
                // v|Alekhine's Defence - rnbqkb1r/pppppppp/5n2/8/4P3/8/PPPP1PPP/RNBQKBNR w KQkq - 1 2
                {15088881769053357431LLU, {
                    Move(MoveType::Quiet, Square::E4, Square::E5)}},
                    // v|Alekhine's Defence - e4e5 - rnbqkb1r/pppppppp/5n2/4P3/8/8/PPPP1PPP/RNBQKBNR b KQkq - 0 2
                    {12534082090564268652LLU, {
                        Move(MoveType::Quiet, Square::F6, Square::D5)}},
                    // v|Alekhine's Defence - e4e5 f6d5 - rnbqkb1r/pppppppp/8/3nP3/8/8/PPPP1PPP/RNBQKBNR w KQkq - 1 3
                    {1365368362862926443LLU, {
                        Move(MoveType::DoublePawnPush, Square::D2, Square::D4)}},
                    // v|Alekhine's Defence - e4e5 f6d5 d2d4 - rnbqkb1r/pppppppp/8/3nP3/3P4/8/PPP2PPP/RNBQKBNR b KQkq - 0 3
                    {4039194068581541376LLU, {
                        Move(MoveType::Quiet, Square::D7, Square::D6)}},
                    // v|Alekhine's Defence - e4e5 f6d5 d2d4 d7d6 - rnbqkb1r/ppp1pppp/3p4/3nP3/3P4/8/PPP2PPP/RNBQKBNR w KQkq - 0 4
                    {438558983194905563LLU, {
                        Move(MoveType::Quiet, Square::G1, Square::F3)}},
                    // v|Alekhine's Defence - e4e5 f6d5 d2d4 d7d6 g1f3 - rnbqkb1r/ppp1pppp/3p4/3nP3/3P4/5N2/PPP2PPP/RNBQKB1R b KQkq - 1 4
                    {6632780381304436558LLU, {
                        Move(MoveType::Quiet, Square::G7, Square::G6)}},
                // v|French Defence - rnbqkbnr/pppp1ppp/4p3/8/4P3/8/PPPP1PPP/RNBQKBNR w KQkq - 0 2
                {8920097050941420015LLU,  {
                    Move(MoveType::DoublePawnPush, Square::D2, Square::D4),
                    Move(MoveType::Quiet, Square::G1, Square::F3),
                    Move(MoveType::Quiet, Square::D2, Square::D3)}},
                // v|Caro-Kann Defence - rnbqkbnr/pp1ppppp/2p5/8/4P3/8/PPPP1PPP/RNBQKBNR w KQkq - 0 2
                {4650654809621012036LLU,  {
                    Move(MoveType::DoublePawnPush, Square::D2, Square::D4)}},
                    // v|Caro-Kann Defence - d2d4 - rnbqkbnr/pp1ppppp/2p5/8/3PP3/8/PPP2PPP/RNBQKBNR b KQkq - 0 2
                    {7671507275110326831LLU,  {
                        Move(MoveType::DoublePawnPush, Square::D7, Square::D5)}},
                    // v|Caro-Kann Defence - d2d4 d7d5 - rnbqkbnr/pp2pppp/2p5/3p4/3PP3/8/PPP2PPP/RNBQKBNR w KQkq - 0 3
                    {13103501317940581182LLU,  {
                        Move(MoveType::Capture, Square::E4, Square::D5)}},
                    // v|Caro-Kann Defence - d2d4 d7d5 e4d5 - rnbqkbnr/pp2pppp/2p5/3P4/3P4/8/PPP2PPP/RNBQKBNR b KQkq - 0 3
                    {13264921878476250373LLU,  {
                        Move(MoveType::Capture, Square::C6, Square::D5)}},
                    // v|Caro-Kann Defence - d2d4 d7d5 e4d5 c6d5 - rnbqkbnr/pp2pppp/8/3p4/3P4/8/PPP2PPP/RNBQKBNR w KQkq - 0 4
                    {5795088147202498993LLU,  {
                        Move(MoveType::DoublePawnPush, Square::C2, Square::C4)}},
                    // v|Caro-Kann Defence - d2d4 d7d5 e4d5 c6d5 c2c4 - rnbqkbnr/pp2pppp/8/3p4/2PP4/8/PP3PPP/RNBQKBNR b KQkq - 0 4
                    {1271215216296955275LLU,  {
                        Move(MoveType::Quiet, Square::G8, Square::F6)}},
                    // v|Caro-Kann Defence - d2d4 d7d5 e4d5 c6d5 c2c4 g8f6 - rnbqkb1r/pp2pppp/5n2/3p4/2PP4/8/PP3PPP/RNBQKBNR w KQkq - 1 5
                    {15209535726999171207LLU,  {
                        Move(MoveType::Quiet, Square::B1, Square::C3)}}
#pragma endregion KingsPawn
    };

    std::optional<Move> GetMove(U64 boardHash) {
        if (moves.contains(boardHash)) {
            std::vector<Move> possibleMoves;
            std::sample(moves.at(boardHash).begin(), moves.at(boardHash).end(), std::back_inserter(possibleMoves), 1, std::mt19937{std::random_device{}()});
            return possibleMoves.at(0);
        }
        return {};
    }
}
#endif //OPENINGS_HH
