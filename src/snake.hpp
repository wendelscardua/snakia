#pragma once

#include "board.hpp"
#include "position.hpp"
#include "queue.hpp"

class Snake {
public:
  static const size_t MAX_LENGTH = Board::WIDTH * Board::HEIGHT;
  Queue<Position, MAX_LENGTH + 1> body;
  u16 max_length;
  bool is_alive;
  bool ate;

  Snake();

  void reset();

  void update(Direction input);

  void draw_head();
  void erase_head(Direction new_direction);
  void erase_tail();
  void draw_tail();
};
