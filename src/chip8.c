#include "chip8.h"
#include "instruction.h"
#include <time.h>
#include <string.h>
#include <stdio.h>

u8 full_fontset[240] = 
{
    0xF0, 0x90, 0x90, 0x90, 0xF0,  // 0
    0x20, 0x60, 0x20, 0x20, 0x70,  // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0,  // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0,  // 3
    0x90, 0x90, 0xF0, 0x10, 0x10,  // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0,  // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0,  // 6
    0xF0, 0x10, 0x20, 0x40, 0x40,  // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0,  // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0,  // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90,  // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0,  // B
    0xF0, 0x80, 0x80, 0x80, 0xF0,  // C
    0xE0, 0x90, 0x90, 0x90, 0xE0,  // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0,  // E
    0xF0, 0x80, 0xF0, 0x80, 0x80,  // F

    // high-res mode font sprites (0-9)
    0x3C, 0x7E, 0xE7, 0xC3, 0xC3, 0xC3, 0xC3, 0xE7, 0x7E, 0x3C, 
    0x18, 0x38, 0x58, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x3C,
    0x3E, 0x7F, 0xC3, 0x06, 0x0C, 0x18, 0x30, 0x60, 0xFF, 0xFF,
    0x3C, 0x7E, 0xC3, 0x03, 0x0E, 0x0E, 0x03, 0xC3, 0x7E, 0x3C,
    0x06, 0x0E, 0x1E, 0x36, 0x66, 0xC6, 0xFF, 0xFF, 0x06, 0x06,
    0xFF, 0xFF, 0xC0, 0xC0, 0xFC, 0xFE, 0x03, 0xC3, 0x7E, 0x3C,
    0x3E, 0x7C, 0xC0, 0xC0, 0xFC, 0xFE, 0xC3, 0xC3, 0x7E, 0x3C,
    0xFF, 0xFF, 0x03, 0x06, 0x0C, 0x18, 0x30, 0x60, 0x60, 0x60,
    0x3C, 0x7E, 0xC3, 0xC3, 0x7E, 0x7E, 0xC3, 0xC3, 0x7E, 0x3C,
    0x3C, 0x7E, 0xC3, 0xC3, 0x7F, 0x3F, 0x03, 0x03, 0x3E, 0x7C
};



void init_cpu(chip8_t *chip8) 
{
    srandom((u8)time(NULL));
    memcpy(chip8->memory, full_fontset, sizeof(full_fontset)); // Copy Font into Memory

    chip8->pc = 0x200;
    chip8->state = RUNNING;
    chip8->increment_pc = true;
    chip8->horizontal_res = H_RES;
    chip8->vertical_res = V_RES;

    initialize_opcode_table();
}

void cpu_cycle(chip8_t *chip8) 
{
    // Get Opcode, Nibble, Vx, and Vy
    u16 opcode = chip8->memory[chip8->pc] << 8 | chip8->memory[chip8->pc + 1];
    u16 highest_nibble = (opcode & 0xF000) >> 12;
    u16 x = (opcode & 0x0F00) >> 8;
    u16 y = (opcode & 0x00F0) >> 4;

    // Finds Instruction in Opcode Table and Runs it
    opcode_table[highest_nibble](chip8, opcode, x, y);

    // Check whether to increment PC
    if (chip8->increment_pc) chip8->pc += 2;
    else chip8->increment_pc = true;
}

void update_timers(chip8_t *chip8) 
{
    // Decreament Delay/Sound Timers
    if (chip8->delay_timer > 0) chip8->delay_timer -= 1;
    if (chip8->sound_timer > 0) {
        printf("BEEP\n");
        chip8->sound_timer -= 1;
        if (chip8->sound_timer == 0) chip8->sound_flag += 1;
    }
}

/* Filesystem Functions */

u8 load_rom(const char* rom_path, chip8_t *chip8) 
{
    u8 status = -1;
    FILE *fptr = fopen(rom_path, "rb");
    if (fptr == NULL)
    {
        return status;
    }

    fread(chip8->memory + chip8->pc, sizeof(char), sizeof(chip8->memory) - chip8->pc, fptr);
    status = 0;
    fclose(fptr);
    return status;
}

u8 load_RPLFlags(chip8_t *chip8) 
{
    FILE *file = fopen("rpl_flags.bin", "wb");
    if (file) {
        fread(chip8->rpl_flags, sizeof(uint8_t), 8, file);
        fclose(file);
        return 0;
    } else {
        printf("ERROR: Could not load rpl_flags.bin");
    }

    return 1;
}

u8 save_RPLFlags(chip8_t *chip8) 
{
    FILE *file = fopen("rpl_flags.bin", "wb");
    if (file) {
        fwrite(chip8->rpl_flags, sizeof(uint8_t), 8, file);
        fclose(file);
        return 0;
    } else {
        printf("ERROR: Could not save rpl_flags.bin");
    }

    return 1;
}

/* Quirk Commands */
void enableQuirk(chip8_t *chip8, u8 quirk) 
{
    chip8->quirks |= (1 << quirk);
}

void disableQuirk(chip8_t *chip8, u8 quirk) 
{ 
    chip8->quirks &= ~(1 << quirk);
}

u8 isQuirkEnabled(chip8_t *chip8, u8 quirk) 
{
    return (chip8->quirks & (1 << quirk)) != 0;
}

/* Debug Commands */
void print_memory(const chip8_t *chip8) 
{
    printf("Memory Dump (4096 bytes):\n");
    for (int i = 0; i < 4096; i += 16) {
        printf("0x%03X: ", i); // Print the memory address
        for (int j = 0; j < 16; j++) {
            printf("%02X ", chip8->memory[i + j]); // Print memory content in hex
        }
        printf("\n");
    }
}