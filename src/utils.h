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
#define DEBUG 1

// Chip 8
#define STACK_SIZE 48
#define MAX_MEMORY 0x1000

typedef uint8_t u8;
typedef uint16_t u16;

#endif