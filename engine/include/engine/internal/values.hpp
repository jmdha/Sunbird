#ifndef CHESS_ENGINE_VALUES
#define CHESS_ENGINE_VALUES

#include <array>

namespace Chess::Engine::Values {
constexpr int PHASE_INC[6] = {0, 1, 1, 2, 4, 0};
constexpr int INF = 99999;
namespace Material {
namespace Pawn {
constexpr int MG = 82;
constexpr int EG = 94;
} // namespace Pawn
namespace Knight {
constexpr int MG = 337;
constexpr int EG = 281;
} // namespace Knight
namespace Bishop {
constexpr int MG = 365;
constexpr int EG = 297;
} // namespace Bishop
namespace Rook {
constexpr int MG = 477;
constexpr int EG = 512;
} // namespace Rook
namespace Queen {
constexpr int MG = 1025;
constexpr int EG = 936;
} // namespace Queen
constexpr std::array<int, 6> MG = {Pawn::MG, Knight::MG, Bishop::MG, Rook::MG, Queen::MG, 0};
constexpr std::array<int, 6> EG = {Pawn::EG, Knight::EG, Bishop::EG, Rook::EG, Queen::EG, 0};
} // namespace Material
namespace Position {
// clang-format off
namespace Pawn {
constexpr std::array<int, 64> MG = {
      0,  0,  0,  0,  0,  0,  0,  0,
      5, 10, 10,-20,-20, 10, 10,  5,
      5, -5, 10,  0,  0,-10, -5,  5,
      0,  0,  0, 20, 20,  0,  0,  0,
      5,  5, 10, 25, 25, 10,  5,  5,
     10, 10, 20, 30, 30, 20, 10, 10,
     50, 50, 50, 50, 50, 50, 50, 50,
      0,  0,  0,  0,  0,  0,  0,  0
};

constexpr std::array<int, 64> EG = {
      0,  0,  0,  0,  0,  0,  0,  0,
      5, 10, 10,-20,-20, 10, 10,  5,
      5, -5, 10,  0,  0,-10, -5,  5,
      0,  0,  0, 20, 20,  0,  0,  0,
      5,  5, 10, 25, 25, 10,  5,  5,
     10, 10, 20, 30, 30, 20, 10, 10,
     50, 50, 50, 50, 50, 50, 50, 50,
      0,  0,  0,  0,  0,  0,  0,  0
};
} // namespace Pawn

namespace Knight {
constexpr std::array<int, 64> MG = {
    -50,-40,-30,-30,-30,-30,-40,-50,
    -40,-20,  0,  0,  0,  0,-20,-40,
    -30,  0, 10, 15, 15, 10,  0,-30,
    -30,  5, 15, 20, 20, 15,  5,-30,
    -30,  0, 15, 20, 20, 15,  0,-30,
    -30,  5, 10, 15, 15, 10,  5,-30,
    -40,-20,  0,  5,  5,  0,-20,-40,
    -50,-40,-30,-30,-30,-30,-40,-50
};

constexpr std::array<int, 64> EG = {
    -50,-40,-30,-30,-30,-30,-40,-50,
    -40,-20,  0,  0,  0,  0,-20,-40,
    -30,  0, 10, 15, 15, 10,  0,-30,
    -30,  5, 15, 20, 20, 15,  5,-30,
    -30,  0, 15, 20, 20, 15,  0,-30,
    -30,  5, 10, 15, 15, 10,  5,-30,
    -40,-20,  0,  5,  5,  0,-20,-40,
    -50,-40,-30,-30,-30,-30,-40,-50
};
} // namespace Knight

namespace Bishop {
constexpr std::array<int, 64> MG = {
    -20,-10,-10,-10,-10,-10,-10,-20,
    -10,  5,  0,  0,  0,  0,  5,-10,
    -10, 10, 10, 10, 10, 10, 10,-10,
    -10,  0, 10, 10, 10, 10,  0,-10,
    -10,  5,  5, 10, 10,  5,  5,-10,
    -10,  0,  5, 10, 10,  5,  0,-10,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -20,-10,-10,-10,-10,-10,-10,-20
};

constexpr std::array<int, 64> EG = {
    -20,-10,-10,-10,-10,-10,-10,-20,
    -10,  5,  0,  0,  0,  0,  5,-10,
    -10, 10, 10, 10, 10, 10, 10,-10,
    -10,  0, 10, 10, 10, 10,  0,-10,
    -10,  5,  5, 10, 10,  5,  5,-10,
    -10,  0,  5, 10, 10,  5,  0,-10,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -20,-10,-10,-10,-10,-10,-10,-20
};
} // namespace Bishop

namespace Rook {
constexpr std::array<int, 64> MG = {
     0,  0,  0,  5,  5,  0,  0,  0
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
     5, 10, 10, 10, 10, 10, 10,  5,
     0,  0,  0,  0,  0,  0,  0,  0
};

constexpr std::array<int, 64> EG = {
    0,  0,  0,  5,  5,  0,  0,  0
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
    -5,  0,  0,  0,  0,  0,  0, -5,
     5, 10, 10, 10, 10, 10, 10,  5,
     0,  0,  0,  0,  0,  0,  0,  0
};
} // namespace Rook

namespace Queen {
constexpr std::array<int, 64> MG {
    -20,-10,-10, -5, -5,-10,-10,-20
    -10,  0,  5,  0,  0,  0,  0,-10,
    -10,  5,  5,  5,  5,  5,  0,-10,
      0,  0,  5,  5,  5,  5,  0, -5,
     -5,  0,  5,  5,  5,  5,  0, -5,
    -10,  0,  5,  5,  5,  5,  0,-10,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -20,-10,-10, -5, -5,-10,-10,-20
};

constexpr std::array<int, 64> EG = {
20,-10,-10, -5, -5,-10,-10,-20
    -10,  0,  5,  0,  0,  0,  0,-10,
    -10,  5,  5,  5,  5,  5,  0,-10,
      0,  0,  5,  5,  5,  5,  0, -5,
     -5,  0,  5,  5,  5,  5,  0, -5,
    -10,  0,  5,  5,  5,  5,  0,-10,
    -10,  0,  0,  0,  0,  0,  0,-10,
    -20,-10,-10, -5, -5,-10,-10,-20
};
} // namespace Queen

namespace King {
constexpr std::array<int, 64> MG = {
     20, 30, 10,  0,  0, 10, 30, 20,
     20, 20,  0,  0,  0,  0, 20, 20,
    -10,-20,-20,-20,-20,-20,-20,-10,
    -20,-30,-30,-40,-40,-30,-30,-20,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30
};

constexpr std::array<int, 64> EG = {
     20, 30, 10,  0,  0, 10, 30, 20,
     20, 20,  0,  0,  0,  0, 20, 20,
    -10,-20,-20,-20,-20,-20,-20,-10,
    -20,-30,-30,-40,-40,-30,-30,-20,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30,
    -30,-40,-40,-50,-50,-40,-40,-30
};
} // namespace King
// clang-format on
} // namespace Position
namespace {
constexpr std::array<int, 64> C(std::array<int, 64> a, int b) {
    std::array<int, 64> array;
    for (int i = 0; i < array.size(); i++)
        array[i] = a[i] + b;
    return array;
}
constexpr std::array<int, 64> I(std::array<int, 64> array) {
    std::array<int, 64> newArray;
    for (int i = 0; i < array.size(); i++)
        newArray[i] = array.at(63 - i);
    return newArray;
} // namespace
} // namespace
constexpr std::array<std::array<int, 64>, 6> MG_WHITE = {
    C(Position::Pawn::MG, Material::Pawn::MG),     C(Position::Knight::MG, Material::Knight::MG),
    C(Position::Bishop::MG, Material::Bishop::MG), C(Position::Rook::MG, Material::Rook::MG),
    C(Position::Queen::MG, Material::Queen::MG),   C(Position::King::MG, 0)};
constexpr std::array<std::array<int, 64>, 6> MG_BLACK = {
    C(I(Position::Pawn::MG), Material::Pawn::MG),
    C(I(Position::Knight::MG), Material::Knight::MG),
    C(I(Position::Bishop::MG), Material::Bishop::MG),
    C(I(Position::Rook::MG), Material::Rook::MG),
    C(I(Position::Queen::MG), Material::Queen::MG),
    C(I(Position::King::MG), 0)};
constexpr std::array<std::array<int, 64>, 6> EG_WHITE = {
    C(Position::Pawn::EG, Material::Pawn::EG),     C(Position::Knight::EG, Material::Knight::EG),
    C(Position::Bishop::EG, Material::Bishop::EG), C(Position::Rook::EG, Material::Rook::EG),
    C(Position::Queen::EG, Material::Queen::EG),   C(Position::King::EG, 0)};
constexpr std::array<std::array<int, 64>, 6> EG_BLACK = {
    C(I(Position::Pawn::EG), Material::Pawn::EG),
    C(I(Position::Knight::EG), Material::Knight::EG),
    C(I(Position::Bishop::EG), Material::Bishop::EG),
    C(I(Position::Rook::EG), Material::Rook::EG),
    C(I(Position::Queen::EG), Material::Queen::EG),
    C(I(Position::King::EG), 0)};
constexpr std::array<std::array<std::array<int, 64>, 6>, 2> MG{MG_WHITE, MG_BLACK};
constexpr std::array<std::array<std::array<int, 64>, 6>, 2> EG{EG_WHITE, EG_BLACK};
} // namespace Chess::Engine::Values
#endif
