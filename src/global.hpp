#pragma once

#include "common.hpp"
#include "input.hpp"

/* represents global state:
   - settings
   - high scores
   - etc
*/
class Global {
public:
  enum class GameState : u8 { PrepareMenu, Menu, PrepareGame, Playing, GameOver };

  GameState game_state;

  Input input;

  Global();
};

extern Global global_state;
