#pragma once

#include "position.hpp"
#include "common.hpp"
#include "queue.hpp"

class Snake {
public:
  Queue<Position, 300> body;
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
