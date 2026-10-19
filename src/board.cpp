#include "board.hpp"
#include "coroutine.hpp"
#include <nesdoug.h>
#include <neslib.h>

Board board;

Board::Board() {};

static const u16 MASKS[Board::WIDTH] = {
    0b0000000000000001, 0b0000000000000010, 0b0000000000000100,
    0b0000000000001000, 0b0000000000010000, 0b0000000000100000,
    0b0000000001000000, 0b0000000010000000, 0b0000000100000000,
    0b0000001000000000, 0b0000010000000000, 0b0000100000000000,
    0b0001000000000000, 0b0010000000000000, 0b0100000000000000,
    0b1000000000000000,
};

bool Board::is_set(u8 x, u8 y) { return inner_board[x] & MASKS[y]; }

bool Board::has_pellet(u8 x, u8 y) { return !needs_pellet && pellet.x == x && pellet.y == y; }

bool Board::has_food(u8 x, u8 y) { return food_ticks > 0 && food.y == y && (food.x == x || food.x + 1 == x); }

void Board::set(u8 x, u8 y) { inner_board[x] |= MASKS[y]; }

void Board::unset(u8 x, u8 y) { inner_board[x] &= ~MASKS[y]; }

void Board::clear() {
  for (u8 i = 0; i < WIDTH; i++) {
    inner_board[i] = 0;
  }

  food_ticks = 0;
  food_pellet_counter = 0;
  needs_pellet = true;
}

void Board::pellet_eaten() {
  needs_pellet = true;
  food_pellet_counter++;
}

void Board::async_spawn_pellet() {
  CORO_INIT;

  if (needs_pellet) {
    while (true) {
      pellet.x = rand16() % WIDTH;
      pellet.y = rand16() % HEIGHT;
      if (!is_set(pellet.x, pellet.y) &&
          (food.y != pellet.y ||
           (food.x != pellet.x && food.x + 1 != pellet.x))) {
        break;
      }
      CORO_YIELD();
    }
    one_vram_buffer(0x20, pellet.screen_coordinates());
    needs_pellet = false;
  }
  CORO_FINISH();
}

void Board::async_spawn_food() {
  CORO_INIT;

  if (food_ticks == 0 && food_pellet_counter >= PELLETS_FOR_FOOD) {
    while (true) {
      food.x = rand16() % (WIDTH - 1);
      food.y = rand16() % HEIGHT;
      if (!is_set(food.x, food.y) && !is_set(food.x + 1, food.y) &&
          (food.y != pellet.y ||
           (food.x != pellet.x && food.x + 1 != pellet.x))) {
        break;
      }
      CORO_YIELD();
    }
    u8 random_food_tile = 2 * (rand8() % 6) + 0x90;
    one_vram_buffer(random_food_tile, food.screen_coordinates());
    one_vram_buffer(random_food_tile + 1, food.screen_coordinates() + 1);
    one_vram_buffer(random_food_tile, NTADR_A(23, 2));
    one_vram_buffer(random_food_tile + 1, NTADR_A(24, 2));
    food_pellet_counter -= PELLETS_FOR_FOOD;
    food_ticks = FOOD_TIMER;
  }
  CORO_FINISH();
}

void Board::update_food() {
  if (food_ticks > 0) {
    food_ticks--;
    if (food_ticks == 0) {
      if (!is_set(food.x, food.y)) {
        one_vram_buffer(3, food.screen_coordinates());
      }
      if (!is_set(food.x + 1, food.y)) {
        one_vram_buffer(3, food.screen_coordinates() + 1);
      }
      goto blank;
    }
    char first_row[2];
    char second_row[2];
    u8 value = food_ticks;
    // NOTE: food_ticks is always less than 20 (citation needed)
    if (value >= 10) {
      value -= 10;
      first_row[0] = 0x31;
      second_row[0] = 0x41;
    } else {
      first_row[0] = 0x30;
      second_row[0] = 0x40;
    }
    first_row[1] = 0x30 + value;
    second_row[1] = 0x40 + value;
    multi_vram_buffer_horz(first_row, 2, NTADR_A(25, 2));
    multi_vram_buffer_horz(second_row, 2, NTADR_A(25, 3));
  } else {
  blank:
    const char first_blank[] = {3, 3, 3, 3};
    const char second_blank[] = {0x4a, 0x4a, 0x4a, 0x4a};
    multi_vram_buffer_horz(first_blank, 4, NTADR_A(23, 2));
    multi_vram_buffer_horz(second_blank, 4, NTADR_A(23, 3));
  }
}

void Board::food_eaten() {
  if (!is_set(food.x, food.y)) {
    one_vram_buffer(3, food.screen_coordinates());
  }
  if (!is_set(food.x + 1, food.y)) {
    one_vram_buffer(3, food.screen_coordinates() + 1);
  }
  food_ticks = 0;
  update_food(); // clear food hud
}

u8 Board::food_value() { return food_ticks; }
