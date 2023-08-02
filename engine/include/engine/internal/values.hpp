#ifndef CHESS_ENGINE_VALUES
#define CHESS_ENGINE_VALUES

#include <array>

namespace Chess::Engine {
namespace MaterialValue {
enum { Pawn = 100, Knight = 300, Bishop = 300, Rook = 500, Queen = 900, Inf = 300000 };
constexpr std::array<int, 5> ALL = {Pawn, Knight, Bishop, Rook, Queen};
} // namespace MaterialValue

// clang-format off

namespace PositionValue {
constexpr std::array<short, 64> PAWN_WHITE = {
	  0,  0,  0,  0,  0,  0,  0,  0,
      5, 10, 10,-20,-20, 10, 10,  5,
      5, -5, 10,  0,  0,-10, -5,  5,
      0,  0,  0, 20, 20,  0,  0,  0,
      5,  5, 10, 25, 25, 10,  5,  5,
     10, 10, 20, 30, 30, 20, 10, 10,
     50, 50, 50, 50, 50, 50, 50, 50,
      0,  0,  0,  0,  0,  0,  0,  0
};

constexpr std::array<short, 64> KNIGHT_WHITE = {
	-50,-40,-30,-30,-30,-30,-40,-50,
    -40,-20,  0,  0,  0,  0,-20,-40,
    -30,  0, 10, 15, 15, 10,  0,-30,
    -30,  5, 15, 20, 20, 15,  5,-30,
    -30,  0, 15, 20, 20, 15,  0,-30,
    -30,  5, 10, 15, 15, 10,  5,-30,
    -40,-20,  0,  5,  5,  0,-20,-40,
    -50,-40,-30,-30,-30,-30,-40,-50
};

constexpr std::array<short, 64> BISHOP_WHITE = {
	-20,-10,-10,-10,-10,-10,-10,-20,
    -10,  5,  0,  0,  0,  0,  5,-10,
    -10, 10, 10, 10, 10, 10, 10,-10,
    -10,  0, 10, 10, 10, 10,  0,-10,
    -10,  5,  5, 10, 10,  5,  5,-10,
    -10,  0,  5, 10, 10,  5,  0,-10,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -20,-10,-10,-10,-10,-10,-10,-20
};

constexpr std::array<short, 64> ROOK_WHITE = {
	 0,  0,  0,  5,  5,  0,  0,  0
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
     5, 10, 10, 10, 10, 10, 10,  5,
     0,  0,  0,  0,  0,  0,  0,  0
};

constexpr std::array<short, 64> QUEEN_WHITE = {
	-20,-10,-10, -5, -5,-10,-10,-20
    -10,  0,  5,  0,  0,  0,  0,-10,
    -10,  5,  5,  5,  5,  5,  0,-10,
      0,  0,  5,  5,  5,  5,  0, -5,
     -5,  0,  5,  5,  5,  5,  0, -5,
    -10,  0,  5,  5,  5,  5,  0,-10,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -20,-10,-10, -5, -5,-10,-10,-20
};

constexpr std::array<short, 64> KING_WHITE = {
	 20, 30, 10,  0,  0, 10, 30, 20,
     20, 20,  0,  0,  0,  0, 20, 20,
    -10,-20,-20,-20,-20,-20,-20,-10,
    -20,-30,-30,-40,-40,-30,-30,-20,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30
};

// clang-format on

constexpr std::array<std::array<short, 64>, 6> ALL_WHITE{PAWN_WHITE, KNIGHT_WHITE, BISHOP_WHITE,
                                                         ROOK_WHITE, QUEEN_WHITE,  KING_WHITE};
namespace {
constexpr std::array<short, 64> InverseArray(std::array<short, 64> array) {
    std::array<short, 64> newArray;
    for (int i = 0; i < array.size(); i++)
        newArray[i] = array.at(63 - i);
    return newArray;
}
} // namespace

constexpr std::array<std::array<short, 64>, 6> ALL_BLACK{
    InverseArray(PAWN_WHITE), InverseArray(KNIGHT_WHITE), InverseArray(BISHOP_WHITE),
    InverseArray(ROOK_WHITE), InverseArray(QUEEN_WHITE),  InverseArray(KING_WHITE)};
} // namespace PositionValue
} // namespace Chess::Engine
#endif
