#ifndef CHIP8_H
#define CHIP8_H

#include "utils.h"

typedef enum STATE 
{
    QUIT,
    RUNNING,
    PAUSE,
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
    u8 rpl_flags[RPL_FLAG_COUNT];
    u8 buffer[SUPER_H_RES * SUPER_V_RES]; // Set Buffer to SChip Dimensions by Default
    u8 keypad[16];
    u8 horizontal_res;
    u8 vertical_res;
    u8 delay_timer;
    u8 sound_timer;
    u8 draw_flag;
    u8 sound_flag;
    u8 quirks; // Store Quirks as Bit Flags
    bool increment_pc;
    bool super_mode;
} chip8_t;

// Chip8 Functions
void init_cpu(chip8_t *chip8);
void cpu_cycle(chip8_t *chip8);
void update_timers(chip8_t *chip8);

// Filesystem Functions
u8 load_rom(const char* rom_path, chip8_t *chip8);

// Quirk Commands
void enableQuirk(chip8_t *chip8, u8 quirk);
void disableQuirk(chip8_t *chip8, u8 quirk);
bool isQuirkEnabled(chip8_t *chip8, u8 quirk);

// Debug
void print_memory(const chip8_t *chip8);

#endif