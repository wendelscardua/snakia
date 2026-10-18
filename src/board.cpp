#include "board.hpp"
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

bool Board::has_pellet(u8 x, u8 y) { return pellet.x == x && pellet.y == y; }

void Board::set(u8 x, u8 y) { inner_board[x] |= MASKS[y]; }

void Board::unset(u8 x, u8 y) { inner_board[x] &= ~MASKS[y]; }

void Board::clear() {
  for (u8 i = 0; i < WIDTH; i++) {
    inner_board[i] = 0;
  }
}

void Board::spawn_pellet() {
  pellet.x = rand16() % WIDTH;
  pellet.y = rand16() % HEIGHT;
}

void Board::draw_pellet() {
  one_vram_buffer(0x20, pellet.screen_coordinates());
}

bool Board::is_pellet_valid() {
  return !is_set(pellet.x, pellet.y);
}
