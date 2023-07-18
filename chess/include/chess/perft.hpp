#ifndef PERFT
#define PERFT

#include <memory>
#include <string>
#include <utility>

#include "board.hpp"
#include "internal/constants.hpp"
#include "internal/move.hpp"
#include "move_gen.hpp"

class Perft {
  public:
    int RunFromPosition(Board &board, int depth);
    int PerftDivide(Board &board, int depth);

  private:
    int Run(Board &board, int depth);
};
#endif // PERFT
