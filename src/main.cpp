#include "global.hpp"
#include "phone.hpp"
#include <nesdoug.h>
#include <neslib.h>
#include <peekpoke.h>
#include <ines.h>

MAPPER_PRG_ROM_KB(32);
MAPPER_CHR_ROM_KB(8);
MAPPER_USE_VERTICAL_MIRRORING;


Global global_state;

int main() {
  ppu_off();

  bank_bg(0);
  bank_spr(1);

  set_vram_buffer();

  Phone phone;

  phone.loop();
}
