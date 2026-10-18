#pragma once

#include "board.hpp"
#include "position.hpp"
#include "queue.hpp"

class Snake {
public:
  Queue<Position, Board::WIDTH * Board::HEIGHT> body;
  u16 max_length;
  bool is_alive;

  Snake();

  void reset();

  void update(Direction input);

  void draw_head();
  void erase_head(Direction new_direction);
  void erase_tail();
  void draw_tail();
};
