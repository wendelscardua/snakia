#pragma once

#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;

enum Direction {
  None = 0,
  UpLeft = 1,
  Up = 2,
  UpRight = 3,
  Left = 4,
  Center = 5,
  Right = 6,
  DownLeft = 7,
  Down = 8,
  DownRight = 9,
};

// neslib/nesdoug internal stuff

extern "C" char OAM_BUF[256];
extern "C" char SPRID;

extern u8 VRAM_INDEX;
extern char VRAM_BUF[256];

typedef union {
  struct {
    s8 x;
    s8 y;
    u8 tile;
    u8 attribute;
  } spr;
  u8 terminator;
} Sprite;
