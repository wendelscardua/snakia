#include "snake.hpp"
#include "board.hpp"
#include "ggsound.hpp"
#include "position.hpp"
#include "soundtrack.hpp"
#include <nesdoug.h>
#include <neslib.h>

Snake::Snake() {}

void Snake::reset(Position position) {
  body.clear();
  body.push(position);
  max_length = 4;
  is_alive = true;
  ate_pellet = false;
  ate_food = false;
}

void Snake::update(Direction input) {
  if (!is_alive) {
    return;
  }

  auto& current_head = body.peek_head();

  // normalize direction:
  // - diagonals are converted to the appropriate direction
  // - invalid directions default to the head one
  const Direction NEXT_DIRECTION_FROM_UP[] = {
      Up, Left, Up, Right, Left, Up, Right, Left, Up, Right,
  };

  const Direction NEXT_DIRECTION_FROM_DOWN[] = {
      Down, Left, Down, Right, Left, Down, Right, Left, Down, Right,
  };

  const Direction NEXT_DIRECTION_FROM_LEFT[] = {
      Left, Up, Up, Up, Left, Left, Left, Down, Down, Down,
  };

  const Direction NEXT_DIRECTION_FROM_RIGHT[] = {
      Right, Up, Up, Up, Right, Right, Right, Down, Down, Down,
  };

  Direction new_direction;

  switch (current_head.direction) {
  case Up:
    new_direction = NEXT_DIRECTION_FROM_UP[input];
    break;
  case Left:
    new_direction = NEXT_DIRECTION_FROM_LEFT[input];
    break;
  case Right:
    new_direction = NEXT_DIRECTION_FROM_RIGHT[input];
    break;
  case Down:
    new_direction = NEXT_DIRECTION_FROM_DOWN[input];
    break;
  default:
    new_direction = current_head.direction;
    break;
  }

  erase_head(new_direction);

  Position new_head = current_head + new_direction;

  current_head.direction = new_direction;

  body.push(new_head);

  if (board.is_set(new_head.x, new_head.y)) {
    is_alive = false;
    GGSound::play_sfx(SFX::Bleargh, GGSound::SFXPriority::Two);
  } else if (board.has_pellet(new_head.x, new_head.y)) {
    ate_pellet = true;
    if (max_length < MAX_LENGTH) {
      max_length++;
    }
    GGSound::play_sfx(SFX::Bloop, GGSound::SFXPriority::Two);
  } else if (board.has_food(new_head.x, new_head.y)) {
    ate_food = true;
    GGSound::play_sfx(SFX::Bleep, GGSound::SFXPriority::Two);
  }

  if (body.size() > max_length) {
    erase_tail();

    body.pop();

    draw_tail();
  }

  // NOTE: drawing this later to prevent a "headless ouroboros" game over state
  draw_head();
}

void Snake::draw_head() {
  auto head = body.peek_head();
  auto coordinate = head.screen_coordinates();

  u8 tile;
  switch (head.direction) {
  case Right:
    tile = 0x1c;
    break;
  case Left:
    tile = 0x1d;
    break;
  case Down:
    tile = 0x1e;
    break;
  case Up:
    tile = 0x1f;
    break;
  default:
    tile = 0;
    break;
  }


  auto next_position = head + head.direction;
  if (board.has_pellet(next_position.x, next_position.y) || board.has_food(next_position.x, next_position.y)) {
    // NOTE: "open mouth" tiles are offset from "closed mouth" ones by $10
    tile += 0x10;
  }

  one_vram_buffer(tile, coordinate);
  board.set(head.x, head.y);
}

void Snake::erase_head(Direction new_direction) {
  auto& head = body.peek_head();
  auto coordinate = head.screen_coordinates();
  u8 tile;
  switch (head.direction) {
  case Right:
    switch (new_direction) {
    case Down:
      tile = 0x18;
      break;
    case Up:
      tile = 0x19;
      break;
    default:
      tile = 0x14;
      break;
    }
    break;
  case Left:
    switch (new_direction) {
    case Down:
      tile = 0x1a;
      break;
    case Up:
      tile = 0x1b;
      break;
    default:
      tile = 0x15;
      break;
    }
    break;
  case Down:
    switch (new_direction) {
    case Left:
      tile = 0x19;
      break;
    case Right:
      tile = 0x1b;
      break;
    default:
      tile = 0x16;
      break;
    }
    break;
  case Up:
    switch (new_direction) {
    case Left:
      tile = 0x18;
      break;
    case Right:
      tile = 0x1a;
      break;
    default:
      tile = 0x17;
      break;
    }
    break;
  default:
    tile = 0;
    break;
  }

  if (ate_pellet) {
    // NOTE: body variants with a pellet "belly" are offset by $10
    ate_pellet = false;
    tile += 0x10;
  } else if (ate_food) {
    ate_food = false;
    tile += 0x10;
  }

  one_vram_buffer(tile, coordinate);
}

void Snake::erase_tail() {
  auto& tail = body.peek_tail();
  one_vram_buffer(0x03, tail.screen_coordinates());
  board.unset(tail.x, tail.y);
};

void Snake::draw_tail() {
  auto& tail = body.peek_tail();
  auto coordinate = tail.screen_coordinates();
  u8 tile;
  switch (tail.direction) {
  case Right:
    tile = 0x10;
    break;
  case Left:
    tile = 0x11;
    break;
  case Down:
    tile = 0x12;
    break;
  case Up:
    tile = 0x13;
    break;
  default:
    tile = 0;
    break;
  }

  one_vram_buffer(tile, coordinate);
}
