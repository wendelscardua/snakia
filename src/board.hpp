#pragma once

#include "common.hpp"
class Board {
public:
  static const u8 WIDTH = 22;
  static const u8 HEIGHT = 13;

  Board();

  bool is_set(u8 x, u8 y);
  void set(u8 x, u8 y);
  void unset(u8 x, u8 y);
  void clear();
private:

  u16 inner_board[WIDTH];
};

extern Board board;
