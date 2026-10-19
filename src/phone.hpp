#pragma once

#include "common.hpp"
#include "queue.hpp"
#include "snake.hpp"

class Phone {
public:
  const u8 THUMB_PRESSED_TICKS = 8;

  Phone();
  ~Phone();

  void loop();
private:
  u16 score;
  u8 level;

  Snake snake;
  u8 keypad_position;

  u8 ticks;
  u8 ticks_per_step;

  Position thumb_position;
  u8 thumb_pressed_ticks;

  Queue<Direction, 4> input_buffer;

  void game_reset();

  void playing_update();
  void game_over_update();

  void thumb_update();

  void render_thumb();

  void refresh_score_hud();
};
