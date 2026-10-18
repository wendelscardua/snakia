#include "position.hpp"
#include "board.hpp"
#include "common.hpp"
#include <neslib.h>

u8 wrap_increment(u8 value, u8 max) {
  if (value == max - 1) {
    return 0;
  }
  return value + 1;
}

u8 wrap_decrement(u8 value, u8 max) {
  if (value == 0) {
    return max - 1;
  }

  return value - 1;
}

Position Position::operator+(Direction new_direction) const {
  switch (new_direction) {
  case Up:
    return {x, wrap_decrement(y, Board::HEIGHT), new_direction};
  case Left:
    return {wrap_decrement(x, Board::WIDTH), y, new_direction};
  case Right:
    return {wrap_increment(x, Board::WIDTH), y, new_direction};
  case Down:
    return {x, wrap_increment(y, Board::HEIGHT), new_direction};
  default:
    break;
  }
  return *this;
}

int Position::screen_coordinates() const {
  return NTADR_A(x + X_SCREEN_OFFSET, y + Y_SCREEN_OFFSET);
}
