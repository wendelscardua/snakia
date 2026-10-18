#pragma once

#include "common.hpp"

struct Position {
  static const u8 X_SCREEN_OFFSET = 5;
  static const u8 Y_SCREEN_OFFSET = 5;

  u8 x;
  u8 y;
  Direction direction;

public:
  Position operator+(Direction) const;
  int screen_coordinates() const;
};
