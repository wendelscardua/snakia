#pragma once

#include "common.hpp"
#include "queue.hpp"
#include "snake.hpp"

class Phone {
public:
  Phone();
  ~Phone();

  void loop();
private:
  Snake snake;
  u8 keypad_position;

  u8 ticks;
  u8 ticks_per_step;

  Position thumb_position;

  Queue<Direction, 4> input_buffer;

  void playing_update();

  void thumb_update();

  void render_thumb();
};
