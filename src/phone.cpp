#include "phone.hpp"
#include "assets.hpp"
#include "board.hpp"
#include "common.hpp"
#include "global.hpp"
#include "metasprites.hpp"
#include "position.hpp"
#include "ui-metasprites.hpp"
#include <nesdoug.h>
#include <neslib.h>

const u8 TICKS_PER_LEVEL[] = {36, 32, 28, 24, 20, 16, 12, 8, 4};

const Position KEYPAD_TO_POSITION[] = {
    {0, 0, None},       {0x40, 0xa0, None}, {0x80, 0xa0, None},
    {0xb8, 0xa0, None}, {0x40, 0xc0, None}, {0x80, 0xc0, None},
    {0xb8, 0xc0, None}, {0x40, 0xe0, None}, {0x80, 0xe0, None},
    {0xb8, 0xe0, None},
};

Phone::Phone()
    : level(4), keypad_position(5),
      thumb_position(KEYPAD_TO_POSITION[keypad_position]) {
  pal_bg(bg_pal);
  pal_spr(spr_pal);
  vram_adr(NTADR_A(0, 0));
  vram_write(screen_nam, 1024);

  ppu_on_all();
  pal_fade_to(0, 4);

  global_state.game_state = Global::GameState::Resetting;
}

Phone::~Phone() {
  // not needed for now
}

void Phone::game_reset() {
  score = 0;
  ticks = 0;
  ticks_per_step = TICKS_PER_LEVEL[level];

  board.clear();
  snake.reset();
  snake.draw_head();
  board.spawn_pellet();
  if (board.is_pellet_valid()) {
    board.draw_pellet();
  }

  refresh_score_hud();
}

void Phone::loop() {
  while (true) {
    ppu_wait_nmi();

    oam_clear();

    global_state.input.poll();

    thumb_update();

    switch (global_state.game_state) {
    case Global::GameState::Resetting: {
      const char dark_row[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                               1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
      const char light_row[] = {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
                                3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3};
      for (u8 row = 0; row < Board::HEIGHT; row++) {
        multi_vram_buffer_horz(dark_row, 22,
                               NTADR_A(Position::X_SCREEN_OFFSET,
                                       row + Position::Y_SCREEN_OFFSET));
        ppu_wait_nmi();
      }
      for (u8 row = 0; row < Board::HEIGHT; row++) {
        multi_vram_buffer_horz(light_row, 22,
                               NTADR_A(Position::X_SCREEN_OFFSET,
                                       row + Position::Y_SCREEN_OFFSET));
        ppu_wait_nmi();
      }

      game_reset();

      global_state.game_state = Global::GameState::Playing;
    }; break;
    case Global::GameState::Playing:
      playing_update();
      break;
    case Global::GameState::GameOver:
      game_over_update();
      break;
    case Global::GameState::Title:
    case Global::GameState::Menu:
      break;
    }

    render_thumb();

    oam_hide_rest();
  }
}

void Phone::playing_update() {
  ticks++;
  if (ticks < ticks_per_step) {
    // won't move the snake this frame, take the time to update pellet
    if (!board.is_pellet_valid()) {
      board.spawn_pellet();
      if (board.is_pellet_valid()) {
        board.draw_pellet();
      }
    }
    return;
  }
  ticks = 0;

  snake.update(input_buffer.is_empty() ? None : input_buffer.pop());

  if (snake.ate) {
    score += level;

    refresh_score_hud();
  }

  if (!snake.is_alive) {
    global_state.game_state = Global::GameState::GameOver;
  }
}

void Phone::game_over_update() {
  // TODO: draw game over message

  if (!input_buffer.is_empty() && input_buffer.pop() == Center) {
    global_state.game_state = Global::GameState::Resetting;
  }
}

u8 interpolate(u8 source, u8 target) {
  if (source == target) {
    return source;
  }

  if (source < target) {
    return source + (target - source + 3) / 4;
  } else {
    return source - (source - target + 3) / 4;
  }
}

void Phone::thumb_update() {
  auto pressed = global_state.input.pressed();
  const u8 BUTTONS_ABOVE[] = {0, 1, 2, 3, 1, 2, 3, 4, 5, 6};
  const u8 BUTTONS_BELOW[] = {0, 4, 5, 6, 7, 8, 9, 7, 8, 9};
  const u8 BUTTONS_LEFT[] = {0, 1, 1, 2, 4, 4, 5, 7, 7, 8};
  const u8 BUTTONS_RIGHT[] = {0, 2, 3, 3, 5, 6, 6, 8, 9, 9};

  if (pressed & PAD_UP) {
    keypad_position = BUTTONS_ABOVE[keypad_position];
  } else if (pressed & PAD_DOWN) {
    keypad_position = BUTTONS_BELOW[keypad_position];
  }

  if (pressed & PAD_LEFT) {
    keypad_position = BUTTONS_LEFT[keypad_position];
  } else if (pressed & PAD_RIGHT) {
    keypad_position = BUTTONS_RIGHT[keypad_position];
  }

  auto target = KEYPAD_TO_POSITION[keypad_position];

  thumb_position.x = interpolate(thumb_position.x, target.x);
  thumb_position.y = interpolate(thumb_position.y, target.y);

  if (pressed & PAD_A) {
    if (!input_buffer.is_full()) {
      input_buffer.push((Direction)keypad_position);
    }
    thumb_position = target;
    thumb_pressed_ticks = THUMB_PRESSED_TICKS;
  }
}

void Phone::render_thumb() {
  if (thumb_pressed_ticks) {
    thumb_pressed_ticks--;
    oam_meta_spr_horizontal(thumb_position.x, thumb_position.y,
                            UI::PressedThumb);
  } else {
    oam_meta_spr_horizontal(thumb_position.x, thumb_position.y, UI::Thumb);
  }
}

void Phone::refresh_score_hud() {
  char first_row[4];
  char second_row[4];

  u8 digit;
  u16 value = score;

  digit = 0;
  if (value >= 8000) {
    value -= 8000;
    digit += 8;
  }

  if (value >= 4000) {
    value -= 4000;
    digit += 4;
  }

  if (value >= 2000) {
    value -= 2000;
    digit += 2;
  }

  if (value >= 1000) {
    value -= 1000;
    digit++;
  }

  first_row[0] = 0x30 + digit;
  second_row[0] = 0x40 + digit;

  digit = 0;
  if (value >= 800) {
    value -= 800;
    digit += 8;
  }

  if (value >= 400) {
    value -= 400;
    digit += 4;
  }

  if (value >= 200) {
    value -= 200;
    digit += 2;
  }

  if (value >= 100) {
    value -= 100;
    digit++;
  }

  first_row[1] = 0x30 + digit;
  second_row[1] = 0x40 + digit;

  digit = 0;
  if (value >= 80) {
    value -= 80;
    digit += 8;
  }

  if (value >= 40) {
    value -= 40;
    digit += 4;
  }

  if (value >= 20) {
    value -= 20;
    digit += 2;
  }

  if (value >= 10) {
    value -= 10;
    digit++;
  }

  first_row[2] = 0x30 + digit;
  second_row[2] = 0x40 + digit;

  first_row[3] = 0x30 + (u8)value;
  second_row[3] = 0x40 + (u8)value;

  multi_vram_buffer_horz(first_row, 4, NTADR_A(5, 2));
  multi_vram_buffer_horz(second_row, 4, NTADR_A(5, 3));
}
