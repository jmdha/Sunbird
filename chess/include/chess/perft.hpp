#ifndef CHESS_PERFT
#define CHESS_PERFT

#include <memory>
#include <string>
#include <utility>

#include "board.hpp"
#include "internal/constants.hpp"
#include "internal/move.hpp"
#include "move_gen.hpp"

namespace Chess {
class Perft {
public:
    int RunFromPosition(Board &board, int depth);
    int PerftDivide(Board &board, int depth);

private:
    int Run(Board &board, int depth);
};
} // namespace Chess
#endif // PERFT
