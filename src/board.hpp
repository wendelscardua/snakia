#pragma once

#include "common.hpp"
#include "position.hpp"
class Board {
public:
  static const u8 WIDTH = 22;
  static const u8 HEIGHT = 12;

  Board();

  bool is_set(u8 x, u8 y);
  void set(u8 x, u8 y);
  void unset(u8 x, u8 y);
  void clear();

  bool has_pellet(u8 x, u8 y);
  bool is_pellet_valid();
  void spawn_pellet();
  void draw_pellet();
private:

  u16 inner_board[WIDTH];
  Position pellet;
};

extern Board board;
