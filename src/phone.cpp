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
    : level(4), maze(0), keypad_position(5),
      thumb_position(KEYPAD_TO_POSITION[keypad_position]),
      cheat_enabled(false) {
  pal_bg(bg_pal);
  pal_spr(spr_pal);
  vram_adr(NTADR_A(0, 0));
  vram_write(screen_nam, 1024);

  pal_bright(0);
  ppu_on_all();
  pal_fade_to(0, 4);

  global_state.game_state = Global::GameState::PrepareMenu;
}

Phone::~Phone() {
  // not needed for now
}

const unsigned char mazes[6][22 * 12] = {
    {// Maze 0 - empty
     0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
     0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
     0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
     0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
     0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
     0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
     0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
     0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
     0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
     0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
     0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
     0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
     0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
     0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
     0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
     0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
     0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
     0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
     0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
     0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
     0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
     0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03},
    {0x6b, 0x6a, 0x6a, 0x6a, 0x6a, 0x6a, 0x6a, 0x6a, 0x6a, 0x6a, 0x6a, 0x6a,
     0x6a, 0x6a, 0x6a, 0x6a, 0x6a, 0x6a, 0x6a, 0x6a, 0x6a, 0x6c, 0x69, 0x03,
     0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
     0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x69, 0x69, 0x03, 0x03, 0x03,
     0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
     0x03, 0x03, 0x03, 0x03, 0x03, 0x69, 0x69, 0x03, 0x03, 0x03, 0x03, 0x03,
     0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
     0x03, 0x03, 0x03, 0x69, 0x69, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
     0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
     0x03, 0x69, 0x69, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
     0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x69,
     0x69, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
     0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x69, 0x69, 0x03,
     0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
     0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x69, 0x69, 0x03, 0x03, 0x03,
     0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
     0x03, 0x03, 0x03, 0x03, 0x03, 0x69, 0x69, 0x03, 0x03, 0x03, 0x03, 0x03,
     0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
     0x03, 0x03, 0x03, 0x69, 0x69, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
     0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03,
     0x03, 0x69, 0x6e, 0x6a, 0x6a, 0x6a, 0x6a, 0x6a, 0x6a, 0x6a, 0x6a, 0x6a,
     0x6a, 0x6a, 0x6a, 0x6a, 0x6a, 0x6a, 0x6a, 0x6a, 0x6a, 0x6a, 0x6a, 0x6d},
};

void Phone::game_reset() {
  const char dark_row[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                           1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  const char light_row[] = {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
                            3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3};
  for (u8 row = 0; row < Board::HEIGHT + 4; row++) {
    multi_vram_buffer_horz(dark_row, 24,
                           NTADR_A(Position::X_SCREEN_OFFSET - 1,
                                   row + Position::Y_SCREEN_OFFSET - 3));
    ppu_wait_nmi();
  }
  for (u8 row = 0; row < Board::HEIGHT + 4; row++) {
    multi_vram_buffer_horz(light_row, 24,
                           NTADR_A(Position::X_SCREEN_OFFSET - 1,
                                   row + Position::Y_SCREEN_OFFSET - 3));
    ppu_wait_nmi();
  }
  for (u8 row = 0; row < Board::HEIGHT + 4; row++) {
    auto position = NTADR_A(Position::X_SCREEN_OFFSET - 1,
                            row + Position::Y_SCREEN_OFFSET - 3);
    multi_vram_buffer_horz(screen_nam + position - NTADR_A(0, 0), 24, position);
    ppu_wait_nmi();
  }

  // initialize maze

  board.clear();

  for (u8 row = 0; row < Board::HEIGHT; row++) {
    auto position = NTADR_A(Position::X_SCREEN_OFFSET, row + Position::Y_SCREEN_OFFSET);
    multi_vram_buffer_horz(mazes[maze] + 22 * row, 22, position);
    for(u8 column = 0; column < Board::WIDTH; column++) {
      if (mazes[maze][22 * row + column] != 0x03) {
        board.set(column, row);
      }
    }
    ppu_wait_nmi();
  }

  // initialize game itself

  score = 0;
  ticks = 0;
  ticks_per_step = TICKS_PER_LEVEL[level - 1];

  snake.reset();
  snake.draw_head();
  refresh_score_hud();
  board.update_food();
}

void Phone::menu_reset() {
  const char dark_row[] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                           1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  const char light_row[] = {3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,
                            3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3};
  for (u8 row = 0; row < Board::HEIGHT + 4; row++) {
    multi_vram_buffer_horz(dark_row, 24,
                           NTADR_A(Position::X_SCREEN_OFFSET - 1,
                                   row + Position::Y_SCREEN_OFFSET - 3));
    ppu_wait_nmi();
  }
  for (u8 row = 0; row < Board::HEIGHT + 4; row++) {
    multi_vram_buffer_horz(light_row, 24,
                           NTADR_A(Position::X_SCREEN_OFFSET - 1,
                                   row + Position::Y_SCREEN_OFFSET - 3));
    ppu_wait_nmi();
  }
  for (u8 row = 0; row < Board::HEIGHT + 4; row++) {
    auto position = NTADR_A(Position::X_SCREEN_OFFSET - 1,
                            row + Position::Y_SCREEN_OFFSET - 3);
    multi_vram_buffer_horz(menu_nam + position - NTADR_A(0, 0), 24, position);
    ppu_wait_nmi();
  }

  menu_option = 0;
  select_menu_option();
  refresh_level_number();
  refresh_maze_number();
  refresh_cheat_status();
}

void Phone::loop() {
  while (true) {
    ppu_wait_nmi();

    oam_clear();

    global_state.input.poll();

    thumb_update();

    switch (global_state.game_state) {
    case Global::GameState::PrepareMenu:
      menu_reset();
      global_state.game_state = Global::GameState::Menu;
      break;
    case Global::GameState::PrepareGame:
      game_reset();
      global_state.game_state = Global::GameState::Playing;
      break;
    case Global::GameState::Playing:
      playing_update();
      break;
    case Global::GameState::GameOver:
      game_over_update();
      break;
    case Global::GameState::Menu:
      menu_update();
      break;
    }

    render_thumb();

    oam_hide_rest();
  }
}

void Phone::menu_update() {
  if (input_buffer.is_empty()) {
    return;
  }
  auto direction = input_buffer.pop();

  switch (direction) {
  case Up:
    deselect_menu_option();
    if (menu_option == 0) {
      menu_option = 3;
    } else {
      menu_option--;
    }
    select_menu_option();
    break;
  case Down:
    deselect_menu_option();
    if (menu_option == 3) {
      menu_option = 0;
    } else {
      menu_option++;
    }
    select_menu_option();
    break;
  case Center:
    switch (menu_option) {
    case 0:
      global_state.game_state = Global::GameState::PrepareGame;
      break;
    case 1:
      if (level < 9) {
        level++;
        refresh_level_number();
      }
      break;
    case 2:
      if (maze < 5) {
        maze++;
        refresh_maze_number();
      }
      break;
    case 3:
      cheat_enabled = !cheat_enabled;
      refresh_cheat_status();
      break;
    }
    break;
  case Left:
    switch (menu_option) {
    case 0:
      break;
    case 1:
      if (level > 1) {
        level--;
        refresh_level_number();
      }
      break;
    case 2:
      if (maze > 0) {
        maze--;
        refresh_maze_number();
      }
      break;
      break;
    case 3:
      cheat_enabled = !cheat_enabled;
      refresh_cheat_status();
      break;
    }
    break;
  case Right:
    switch (menu_option) {
    case 0:
      break;
    case 1:
      if (level < 9) {
        level++;
        refresh_level_number();
      }
      break;
    case 2:
      if (maze < 5) {
        maze++;
        refresh_maze_number();
      }
      break;
    case 3:
      cheat_enabled = !cheat_enabled;
      refresh_cheat_status();
      break;
    }
    break;
  case None:
  case UpLeft:
  case UpRight:
  case DownLeft:
  case DownRight:
    break;
  }
}

void Phone::playing_update() {
  ticks++;
  if (ticks < ticks_per_step) {
    // won't move the snake this frame, take the time to update other stuff
    board.async_spawn_pellet();
    board.async_spawn_food();
    return;
  }
  ticks = 0;

  board.update_food();
  snake.update(input_buffer.is_empty() ? None : input_buffer.pop());

  if (snake.ate_pellet) {
    score += level;
    refresh_score_hud();
    board.pellet_eaten();
  }

  if (snake.ate_food) {
    score += 3 * level + board.food_value();
    refresh_score_hud();
    board.food_eaten();
  }

  if (!snake.is_alive) {
    global_state.game_state = Global::GameState::GameOver;
  }
}

void Phone::game_over_update() {
  // TODO: draw game over message

  if (!input_buffer.is_empty() && input_buffer.pop() == Center) {
    global_state.game_state = Global::GameState::PrepareMenu;
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

  if (cheat_enabled) {
    bool press = false;
    if (pressed & PAD_UP) {
      if (!input_buffer.is_full()) {
        keypad_position = Up;
        press = true;
      }
    } else if (pressed & PAD_DOWN) {
      if (!input_buffer.is_full()) {
        keypad_position = Down;
        press = true;
      }
    }
    if (pressed & PAD_LEFT) {
      if (!input_buffer.is_full()) {
        keypad_position = Left;
        press = true;
      }
    } else if (pressed & PAD_RIGHT) {
      if (!input_buffer.is_full()) {
        keypad_position = Right;
        press = true;
      }
    }
    if (pressed & PAD_A) {
      if (!input_buffer.is_full()) {
        keypad_position = Center;
        press = true;
      }
    }

    if (press) {
      if (!input_buffer.is_full()) {
        input_buffer.push((Direction)keypad_position);
      }
      thumb_position = KEYPAD_TO_POSITION[keypad_position];
      thumb_pressed_ticks = THUMB_PRESSED_TICKS;
    }
    return;
  }

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

// start, level, cheat
const unsigned char deselected_text[4][12 * 3] = {
    {
        0xc0, 0xd0, 0xf0, 0xd0, 0xf0, 0xd0, 0xf0, 0xd0, 0xf0, 0xd0, 0xf0, 0xc0,
        0xc0, 0xd1, 0xf0, 0xc0, 0xe2, 0xd1, 0xf1, 0xd1, 0xf1, 0xc0, 0xe2, 0xc0,
        0xc0, 0xd0, 0xf1, 0xc0, 0xe2, 0xd1, 0xd1, 0xd1, 0xd2, 0xc0, 0xe2, 0xc0,
    },
    {
        0xc0, 0xd0, 0xc0, 0xd0, 0xf0, 0xd0, 0xd0, 0xd0, 0xf0, 0xd0, 0xc0, 0xc0,
        0xc0, 0xd1, 0xc0, 0xd1, 0xf0, 0xd1, 0xd1, 0xd1, 0xf0, 0xd1, 0xc0, 0xc0,
        0xc0, 0xd1, 0xf0, 0xd1, 0xf0, 0xd1, 0xe2, 0xd1, 0xf0, 0xd1, 0xf0, 0xc0,
    },
    {
        0xc0, 0xd0, 0xf0, 0xe0, 0xd0, 0xf0, 0xd0, 0xf0, 0xd0, 0xf0, 0xc0, 0xc0,
        0xc0, 0xd1, 0xd1, 0xd1, 0xd1, 0xf1, 0xd0, 0xf1, 0xd1, 0xf0, 0xc0, 0xc0,
        0xc0, 0xd1, 0xd1, 0xd1, 0xd1, 0xd1, 0xd1, 0xf0, 0xd1, 0xf0, 0xc0, 0xc0,
    },
    {
        0xc0, 0xd0, 0xf0, 0xd0, 0xd0, 0xd0, 0xf0, 0xd0, 0xf0, 0xd0, 0xf0, 0xc0,
        0xc0, 0xd1, 0xc0, 0xd1, 0xf1, 0xd1, 0xf0, 0xd1, 0xf1, 0xc0, 0xe2, 0xc0,
        0xc0, 0xd1, 0xf0, 0xd1, 0xd1, 0xd1, 0xf0, 0xd1, 0xd1, 0xc0, 0xe2, 0xc0,
    }};

const unsigned char selected_text[4][12 * 3] = {
    {
        0xc4, 0xd4, 0xf4, 0xd4, 0xf4, 0xd4, 0xf4, 0xd4, 0xf4, 0xd4, 0xf4, 0xc4,
        0xc4, 0xd5, 0xf4, 0xc4, 0xe6, 0xd5, 0xf5, 0xd5, 0xf5, 0xc4, 0xe6, 0xc4,
        0xc4, 0xd4, 0xf5, 0xc4, 0xe6, 0xd5, 0xd5, 0xd5, 0xd6, 0xc4, 0xe6, 0xc4,
    },
    {
        0xc4, 0xd4, 0xc4, 0xd4, 0xf4, 0xd4, 0xd4, 0xd4, 0xf4, 0xd4, 0xc4, 0xc4,
        0xc4, 0xd5, 0xc4, 0xd5, 0xf4, 0xd5, 0xd5, 0xd5, 0xf4, 0xd5, 0xc4, 0xc4,
        0xc4, 0xd5, 0xf4, 0xd5, 0xf4, 0xd5, 0xe6, 0xd5, 0xf4, 0xd5, 0xf4, 0xc4,
    },
    {
        0xc4, 0xd4, 0xf4, 0xe4, 0xd4, 0xf4, 0xd4, 0xf4, 0xd4, 0xf4, 0xc4, 0xc4,
        0xc4, 0xd5, 0xd5, 0xd5, 0xd5, 0xf5, 0xd4, 0xf5, 0xd5, 0xf4, 0xc4, 0xc4,
        0xc4, 0xd5, 0xd5, 0xd5, 0xd5, 0xd5, 0xd5, 0xf4, 0xd5, 0xf4, 0xc4, 0xc4,
    },
    {
        0xc4, 0xd4, 0xf4, 0xd4, 0xd4, 0xd4, 0xf4, 0xd4, 0xf4, 0xd4, 0xf4, 0xc4,
        0xc4, 0xd5, 0xc4, 0xd5, 0xf5, 0xd5, 0xf4, 0xd5, 0xf5, 0xc4, 0xe6, 0xc4,
        0xc4, 0xd5, 0xf4, 0xd5, 0xd5, 0xd5, 0xf4, 0xd5, 0xd5, 0xc4, 0xe6, 0xc4,
    }};

void Phone::deselect_menu_option() {
  auto position = NTADR_A(5, 5 + 3 * menu_option);
  multi_vram_buffer_horz(deselected_text[menu_option], 12, position);
  ppu_wait_nmi();
  multi_vram_buffer_horz(deselected_text[menu_option] + 12, 12,
                         position + 0x20);
  ppu_wait_nmi();
  multi_vram_buffer_horz(deselected_text[menu_option] + 24, 12,
                         position + 0x40);
  ppu_wait_nmi();
}

void Phone::select_menu_option() {
  auto position = NTADR_A(5, 5 + 3 * menu_option);
  multi_vram_buffer_horz(selected_text[menu_option], 12, position);
  ppu_wait_nmi();
  multi_vram_buffer_horz(selected_text[menu_option] + 12, 12, position + 0x20);
  ppu_wait_nmi();
  multi_vram_buffer_horz(selected_text[menu_option] + 24, 12, position + 0x40);
  ppu_wait_nmi();
}

const unsigned char sixel_digit[10][2 * 3] = {
    {0xd0, 0xf0, 0xd1, 0xd1, 0xd1, 0xf1}, {0xc0, 0xe0, 0xc1, 0xe2, 0xd0, 0xf2},
    {0xd0, 0xf0, 0xd0, 0xf1, 0xd1, 0xf0}, {0xd0, 0xf0, 0xd0, 0xf1, 0xd0, 0xf1},
    {0xd0, 0xd0, 0xd1, 0xf1, 0xc0, 0xd1}, {0xd0, 0xf0, 0xd1, 0xf0, 0xd0, 0xf1},
    {0xd0, 0xf0, 0xd1, 0xf0, 0xd1, 0xf1}, {0xd0, 0xf0, 0xc0, 0xe1, 0xc0, 0xe2},
    {0xd0, 0xf0, 0xd1, 0xf1, 0xd1, 0xf1}, {0xd0, 0xf0, 0xd1, 0xf1, 0xd0, 0xf1}};

void Phone::refresh_level_number() {
  multi_vram_buffer_horz(sixel_digit[level], 2, NTADR_A(18, 8));
  multi_vram_buffer_horz(sixel_digit[level] + 2, 2, NTADR_A(18, 8) + 0x20);
  multi_vram_buffer_horz(sixel_digit[level] + 4, 2, NTADR_A(18, 8) + 0x40);
}

void Phone::refresh_maze_number() {
  multi_vram_buffer_horz(sixel_digit[maze], 2, NTADR_A(18, 11));
  multi_vram_buffer_horz(sixel_digit[maze] + 2, 2, NTADR_A(18, 11) + 0x20);
  multi_vram_buffer_horz(sixel_digit[maze] + 4, 2, NTADR_A(18, 11) + 0x40);
}

const unsigned char label_off_on[2][4 * 3] = {
    {0xd0, 0xf0, 0xd0, 0xf0, 0xd1, 0xf0, 0xd1, 0xf0, 0xd1, 0xc0, 0xd1, 0xc0},
    {0xd0, 0xe0, 0xc0, 0xc0, 0xd1, 0xd1, 0xc0, 0xc0, 0xd1, 0xd1, 0xc0, 0xc0}};

void Phone::refresh_cheat_status() {
  multi_vram_buffer_horz(label_off_on[cheat_enabled], 4, NTADR_A(20, 14));
  multi_vram_buffer_horz(label_off_on[cheat_enabled] + 4, 4,
                         NTADR_A(20, 14) + 0x20);
  multi_vram_buffer_horz(label_off_on[cheat_enabled] + 8, 4,
                         NTADR_A(20, 14) + 0x40);
}
