#pragma once
#include <array>
#include <cstdint>
#include <memory>

#include "lve_model.hpp"

namespace lve {
enum Color : uint8_t {
  White = 0,
  Blue = 1,
  Red = 2,
  Green = 3,
  Yellow = 4,
  Black = 5,
  Grey = 6,
  Orange = 7,
};
enum Face { Inside = 0, Front, Back, Left, Right, Top, Down };

constexpr std::array<Color, 6> faceColors = {White, Blue,   Red,
                                             Green, Yellow, Orange};
class RubiksCube {
public:
  RubiksCube();
  Model::Builder getModel() const;
  Color getColor(Face face, int x, int y, int z);

private:
  std::array<uint64_t, 6> data;
};

} // namespace lve
