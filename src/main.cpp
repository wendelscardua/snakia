#include "global.hpp"
#include <nesdoug.h>
#include <neslib.h>
#include <peekpoke.h>
#include <ines.h>

MAPPER_PRG_ROM_KB(32);
MAPPER_CHR_ROM_KB(8);
MAPPER_USE_VERTICAL_MIRRORING;

int main() {
  ppu_off();

  bank_bg(0);
  bank_spr(1);

  set_vram_buffer();

  Global global_state;

  // global_state.game_state = Global::GameState::TitleScreen;

  while (true) {
    switch (global_state.game_state) {
    }
  }
}
