#include "phone.hpp"
#include "assets.hpp"
#include "board.hpp"
#include "common.hpp"
#include "global.hpp"
#include <nesdoug.h>
#include <neslib.h>

Phone::Phone() : keypad_position(5), ticks(0), ticks_per_step(24) {
  pal_bg(bg_pal);
  pal_spr(spr_pal);
  vram_adr(NTADR_A(0, 0));
  vram_write(screen_nam, 1024);

  board.clear();
  snake.reset();
  snake.draw_head();
  board.spawn_pellet();
  if (board.is_pellet_valid()) {
    board.draw_pellet();
  }
  flush_vram_update2();

  ppu_on_all();
  pal_fade_to(0, 4);

  global_state.game_state = Global::GameState::Playing;
}

Phone::~Phone() {
  // not needed for now
}

void Phone::loop() {
  while (true) {
    ppu_wait_nmi();

    oam_clear();

    global_state.input.poll();

    thumb_update();

    switch (global_state.game_state) {
    case Global::GameState::Playing:
      playing_update();
      break;
    }

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

  if (pressed & PAD_A) {
    if (keypad_position != 5 && !input_buffer.is_full()) {
      input_buffer.push((Direction)keypad_position);
    }
  }
}
