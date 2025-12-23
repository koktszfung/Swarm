#pragma once

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define STR(x)  #x
#define XSTR(x) STR(x)

#define UNUSED(x)        (void)(x)
#define ARRAY_COUNT(arr) (sizeof(arr) / sizeof((arr)[0]))

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  ((byte) & 0x80 ? '1' : '0'), \
  ((byte) & 0x40 ? '1' : '0'), \
  ((byte) & 0x20 ? '1' : '0'), \
  ((byte) & 0x10 ? '1' : '0'), \
  ((byte) & 0x08 ? '1' : '0'), \
  ((byte) & 0x04 ? '1' : '0'), \
  ((byte) & 0x02 ? '1' : '0'), \
  ((byte) & 0x01 ? '1' : '0')

#define PHYSICS_DT 0.01f
#define PHYSICS_SPEEDUP 2.0f
#define WIN_WIDTH 800
#define WIN_HEIGHT 600
#define CELL_DIM 20.0f
#define CAMERA_SPEED 200.0f
#define ZOOM_SCALE 2.0f
#define MIN_SCALE 0.125f
#define MAX_SCALE 8.0f

#define FONT_SIZE 18
#define DEBUG 1
#define DEBUG_DELTATIME 1
#define DEBUG_CAMERA_POS 1
#define DEBUG_TEXTURE 0
#define DEBUG_HASHCELL_RECT 0
#define DEBUG_HASHCELL_COORD 0
#define DEBUG_SWARM_COORD 0
#define DEBUG_SWARM_NEIGHBOUR 0
