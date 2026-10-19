#pragma once

#include "common.hpp"
#include "queue.hpp"
#include "snake.hpp"

class Phone {
public:
  const u8 THUMB_PRESSED_TICKS = 8;
  const u16 GAME_OVER_DELAY = 60 * 8; // 8 seconds

  Phone();
  ~Phone();

  void loop();
private:
  u16 score;
  u8 level;
  u8 maze;

  Snake snake;
  u8 keypad_position;

  u8 ticks;
  u8 ticks_per_step;

  Position thumb_position;
  u8 thumb_pressed_ticks;

  u8 menu_option;

  bool cheat_enabled;

  u16 game_over_timer;

  Queue<Direction, 4> input_buffer;

  void game_reset();
  void menu_reset();

  void menu_update();
  void playing_update();
  void game_over_update();

  void thumb_update();

  void render_thumb();

  void refresh_score_hud();

  void deselect_menu_option();
  void select_menu_option();

  void refresh_level_number();
  void refresh_maze_number();
  void refresh_cheat_status();
};
