#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

// AppVar Length
#define ROM_NAME_SIZE 20

// Display
#define H_RES 64
#define V_RES 32

// Chip 8
#define STACK_SIZE 48
#define MAX_MEMORY 4096
#define CYCLES_PER_SEC 30
#define SCALE 5

// Quirks
#define QUIRK_VF_RESET 0
#define QUIRK_LOADS 1
#define QUIRK_DISPWAIT 2
#define QUIRK_CLIP 3
#define QUIRK_SHIFT  4
#define QUIRK_JUMP 5

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

#endif