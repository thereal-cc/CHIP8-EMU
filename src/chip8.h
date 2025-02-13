#ifndef CHIP8_H
#define CHIP8_H

#include "utils.h"
#include "debug.h"
#include "sys/timers.h"
#include "graphx.h"
#include "keypadc.h"
#include "fileioc.h"

typedef enum STATE 
{
    QUIT = -1,
    RUNNING = 0,
    PAUSE = 1,
} STATE;

typedef struct 
{
    u8 memory[MAX_MEMORY]; // Memory
    u8 V_register[16]; // Registers [V[0] -> V[15]]
    u16 I_register;
    u16 stack[16];
    u16 pc;
    u8 sp;
    u8 state;
    u8 buffer[H_RES * V_RES]; // Set Buffer to SChip Dimensions by Default
    u8 keypad[16];
    u8 delay_timer;
    u8 sound_timer;
    u8 draw_flag;
    u8 sound_flag;
    u8 quirks; // Store Quirks as Bit Flags
    bool increment_pc;
} chip8_t;

// Chip8 Functions
void init_cpu(chip8_t *chip8);
void cpu_cycle(chip8_t *chip8);
void update_timers(chip8_t *chip8);
void input_handler(chip8_t *chip8);
void draw_chip8(chip8_t *chip8);

// Filesystem Functions
u8 load_rom(chip8_t *chip8);

// Quirk Commands
void enableQuirk(chip8_t *chip8, u8 quirk);
void disableQuirk(chip8_t *chip8, u8 quirk);
bool isQuirkEnabled(chip8_t *chip8, u8 quirk);

// Debug
void print_memory(const chip8_t *chip8);

#endif