#pragma once

#include "common.hpp"
#include "position.hpp"
class Board {
public:
  static const u8 WIDTH = 22;
  static const u8 HEIGHT = 12;

  static const u8 FOOD_TIMER = 20;
  static const u8 PELLETS_FOR_FOOD = 5;

  Board();

  bool is_set(u8 x, u8 y);
  void set(u8 x, u8 y);
  void unset(u8 x, u8 y);
  void clear();

  bool has_pellet(u8 x, u8 y);
  bool is_pellet_valid();
  bool has_food(u8 x, u8 y);
  void pellet_eaten();
  void async_spawn_pellet();
  void async_spawn_food();
  void update_food();
  void food_eaten();
  u8 food_value();
private:

  u16 inner_board[WIDTH];
  Position pellet;
  Position food;
  bool needs_pellet;
  // counts how many game ticks until food despawns
  u8 food_ticks;
  // counts pellets eaten until we need to spawn food
  u8 food_pellet_counter;
};

extern Board board;
