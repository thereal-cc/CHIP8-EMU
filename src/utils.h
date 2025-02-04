#ifndef UTILS_H
#define UTILS_H

#include <stdlib.h>

#define debug_print(fmt, ...)                         \
    do {                                              \
        if (DEBUG) fprintf(stderr, fmt, __VA_ARGS__); \
    } while (0)

// Display
#define H_RES 64
#define V_RES 32
#define DEBUG 0

// Chip 8
#define STACK_SIZE 48
#define MAX_MEMORY 0x1000
#define CYCLES_PER_SEC 11

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