#include "chip8.h"
#include <time.h>
#include <string.h>
#include <stdio.h>

u8 fontset[80] = 
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
    0xF0, 0x80, 0xF0, 0x80, 0x80   // F
};

void init_cpu(chip8_t *chip8) 
{
    srand((unsigned int)time(NULL));
    memcpy(chip8->memory, fontset, sizeof(fontset)); // Copy Font into Memory

    chip8->pc = 0x200;
    chip8->state = RUNNING;
}

u8 load_rom(const char* rom_path, chip8_t *chip8) 
{
    u8 status = -1;
    FILE *fptr = fopen(rom_path, "rb");
    if (fptr == NULL)
    {
        return status;
    }

    fread(chip8->memory + chip8->pc, sizeof(char), sizeof(chip8->memory) - chip8->pc, fptr);
    status = 1;
    fclose(fptr);
    return status;
}

void cpu_cycle(chip8_t *chip8) 
{
    // Get Opcode
    u16 opcode = chip8->memory[chip8->pc] << 8 | chip8->memory[chip8->pc + 1];
    
    // Vx and Vy
    unsigned short x = (opcode & 0x0F00) >> 8;
    unsigned short y = (opcode & 0x00F0) >> 4;

    switch (opcode & 0xF000) {
        case 0x0000:
            switch (opcode & 0x00FF) {
                case 0x00E0: // Clear Display
                    debug_print("[OK] 0x%X: 00E0\n", opcode);
                    memset(chip8->buffer, 0, sizeof(chip8->buffer));
                    chip8->pc += 2;
                    break;
                case 0x00EE: // Return from subroutine
                    debug_print("[OK] 0x%X: 00EE\n", opcode);
                    chip8->pc = chip8->stack[chip8->sp];
                    chip8->sp -= 1;
                    chip8->pc += 2;
                    break;
                default:
                    debug_print("[FAILED] Unknown opcode: 0x%X\n", opcode);
                    break;
            }
            break;
        case 0x1000: // Jump to address NNN
            debug_print("[OK] 0x%X: 1nnn\n", opcode);
            chip8->pc = opcode & 0x0FFF;
            break;
        case 0x2000: // Calls subroutine at NNN
            debug_print("[OK] 0x%X: 2nnn\n", opcode);
            chip8->sp += 1;
            chip8->stack[chip8->sp] = chip8->pc;
            chip8->pc = opcode & 0x0FFF;
            break;
        case 0x3000: // Skip if Vx == kk
            debug_print("[OK] 0x%X: 3xkk\n", opcode);
            if (chip8->V_register[x] == (opcode & 0x00FF)) {
                chip8->pc += 2;
            }
            chip8->pc += 2;
            break;
        case 0x4000: // Skip if Vx != kk
            debug_print("[OK] 0x%X: 4xkk\n", opcode);
            if (chip8->V_register[x] != (opcode & 0x00FF)) {
                chip8->pc += 2;
            }
            chip8->pc += 2;
            break;
        case 0x5000: // Skip if Vx == Vy
            debug_print("[OK] 0x%X: 5xy0\n", opcode);
            if (chip8->V_register[x] == chip8->V_register[y]) {
                chip8->pc += 2;
            }
            chip8->pc += 2;
            break;
        case 0x6000: // Set Vx == kk
            debug_print("[OK] 0x%X: 6xkk\n", opcode);
            chip8->V_register[x] = (opcode & 0x00FF);
            //debug_print("V[%d] = 0x%X\n", x, chip8->V_register[x]);
            chip8->pc += 2;
            break;
        case 0x7000: // Set Vx = Vx + kk 
            debug_print("[OK] 0x%X: 7xkk\n", opcode);
            chip8->V_register[x] += (opcode & 0x00FF);
            chip8->pc += 2;
            break;
        case 0x8000:
            switch (opcode & 0x000F) {
                case 0x0000: // Vx = Vy
                    debug_print("[OK] 0x%X: 8xy0\n", opcode);
                    chip8->V_register[x] = chip8->V_register[y];
                    chip8->pc += 2;
                    break;
                case 0x0001: // Vx = Vx OR Vy
                    debug_print("[OK] 0x%X: 8xy1\n", opcode);
                    chip8->V_register[x] = (chip8->V_register[x] | chip8->V_register[y]);
                    chip8->pc += 2;
                    break;
                case 0x0002: // Vx = Vx AND Vy
                    debug_print("[OK] 0x%X: 8xy2\n", opcode);
                    chip8->V_register[x] = (chip8->V_register[x] & chip8->V_register[y]);
                    chip8->pc += 2;
                    break;
                case 0x0003: // Vx = Vx XOR Vy
                    debug_print("[OK] 0x%X: 8xy3\n", opcode);
                    chip8->V_register[x] = (chip8->V_register[x] ^ chip8->V_register[y]);
                    chip8->pc += 2;
                    break;
                case 0x0004: // Vx = Vx + Vy, VF = Carry
                    debug_print("[OK] 0x%X: 8xy4\n", opcode);
                    chip8->V_register[x] = chip8->V_register[x] + chip8->V_register[y];
                    if (chip8->V_register[x] > 0xFF) chip8->V_register[0xF] = 1;
                    chip8->pc += 2; 
                    break;
                case 0x0005: // Vx = Vx - Vy, VF = Not Borrow
                    debug_print("[OK] 0x%X: 8xy5\n", opcode);
                    if (chip8->V_register[x] > 0xFF) chip8->V_register[0xF] = 1;
                    chip8->V_register[x] = chip8->V_register[x] - chip8->V_register[y];
                    chip8->pc += 2; 
                    break;
                case 0x0006: // Vx = Vx SHR 1
                    if ((chip8->V_register[x] & 1) == 1) chip8->V_register[0xF] = 1;
                    chip8->V_register[x] = chip8->V_register[x] / 2;
                    chip8->pc += 2;
                    break;
                case 0x0007: // Vx = Vy - Vx, VF = Not Borrow
                    debug_print("[OK] 0x%X: 8xy7\n", opcode);
                    if (chip8->V_register[y] > chip8->V_register[x]) chip8->V_register[0xF] = 1;
                    chip8->V_register[x] = chip8->V_register[y] - chip8->V_register[x];
                    chip8->pc += 2; 
                    break;
                case 0x000E: // Vx = Vx SHL 1
                    debug_print("[OK] 0x%X: 8xyE\n", opcode);
                    // Get Most Significant Bit
                    u8 msb = chip8->V_register[x] >> (sizeof(chip8->V_register[x] ) * 8 - 1);
                    if (msb == 1) chip8->V_register[0xF] = 1;
                    chip8->V_register[x] *= 2;
                    chip8->pc += 2;
                    break;
            }
            break;
        case 0x9000:
            debug_print("[OK] 0x%X: 9xy0\n", opcode);
            if (chip8->V_register[x] != chip8->V_register[y]) {
                chip8->pc += 2;
            }
            chip8->pc += 2;
            break;
        case 0xA000: // Set I = nnn
            debug_print("[OK] 0x%X: Annn\n", opcode);
            chip8->I_register = (opcode & 0x0FFF);
            chip8->pc += 2;
            break;
        case 0xB000: // Jmp to location nnn + V0
            debug_print("[OK] 0x%X: Bnnn\n", opcode);
            chip8->pc = (opcode & 0x0FFF) + chip8->V_register[0];
            break;
        case 0xC000: // Vx = random byte && kk
            debug_print("[OK] 0x%X: Cxkk\n", opcode);
            chip8->V_register[x] = ((rand() % 256) & 0x00FF);
            chip8->pc += 2;
            break;
        case 0xD000:
            debug_print("[OK] 0x%X: Dxyn\n", opcode);
            chip8->draw_flag = 1;

            // Get Size, X/Y Coordinates, Set Collision to 0
            u16 sprite_size = opcode & 0x000F;
            u16 pixel;
            chip8->V_register[0xF] = 0;

            u8 i, j;
            for (j = 0; j < sprite_size; j++) {
                pixel = chip8->memory[chip8->I_register + j];

                // Each Bit in the Byte
                for (i = 0; i < 8; i++) {
                    if ((pixel & (0x80 >> i)) != 0) {
                        if (chip8->buffer[(chip8->V_register[x] + i + ((chip8->V_register[y] + j) * H_RES))] == 1) {
                            chip8->V_register[0xF] = 1;
                        }

                        chip8->buffer[(chip8->V_register[x] + i + ((chip8->V_register[y] + j) * H_RES))] ^= 1;
                    }
                }
            }

            chip8->pc += 2;
            break;
        case 0xE000:
            switch (opcode & 0x00FF) {
                case 0x009E: // Skip if key with value in Vx is pressed
                    debug_print("[OK] 0x%X: Ex9E\n", opcode);
                    if (chip8->keypad[chip8->V_register[x]]) {
                        chip8->pc += 2;
                    }
                    chip8->pc += 2;
                    break;
                case 0x00A1:
                    debug_print("[OK] 0x%X: ExA1\n", opcode);
                    if (!chip8->keypad[chip8->V_register[x]]) {
                        chip8->pc += 2;
                    }
                    chip8->pc += 2;
                    break;
            }
            break;
        case 0xF000:
            switch(opcode & 0x00FF) {
                case 0x0007: // Vx = delay timer
                    debug_print("[OK] 0x%X: Fx07\n", opcode);
                    chip8->V_register[x] = chip8->delay_timer;
                    chip8->pc += 2;
                    break;
                case 0x000A: // Wait for key press, store key value in Vx
                    debug_print("[OK] 0x%X: Fx0A\n", opcode);
                    for (u8 i = 0; i < 16; i++) {
                        if (chip8->keypad[i]) {
                            chip8->V_register[x] = i;
                            chip8->pc += 2;
                            break;
                        }
                    }
                    break;
                case 0x0015: // Delay Timer = Vx
                    debug_print("[OK] 0x%X: Fx15\n", opcode);
                    chip8->delay_timer = chip8->V_register[x];
                    chip8->pc += 2;
                    break;
                case 0x0018: // Sound Timer = Vx
                    debug_print("[OK] 0x%X: Fx18\n", opcode);
                    chip8->sound_timer = chip8->V_register[x];
                    chip8->pc += 2;
                    break;
                case 0x001E: // I = I + Vx
                    debug_print("[OK] 0x%X: Fx1E\n", opcode);
                    chip8->I_register += chip8->V_register[x];
                    chip8->pc += 2;
                    break;
                case 0x0029: // I = Location of Sprite for digit V[x]
                    debug_print("[OK] 0x%X: Fx29\n", opcode);
                    chip8->I_register = chip8->V_register[x] * 5;
                    chip8->pc += 2;
                    break;
                case 0x0033: // Store BCD representation of Vx in memory locations I, I+1, and I+2.
                    debug_print("[OK] 0x%X: Fx33\n", opcode);
                    u8 value = chip8->V_register[x];
                    u8 I = chip8->I_register;
                    
                    chip8->memory[I] = (value % 1000) / 100; // Hundreds Digit
                    chip8->memory[I + 1] = (value % 100) / 10; // Tens Digit
                    chip8->memory[I + 2] = value % 10; // Ones Digit

                    chip8->pc += 2;
                    break;
                case 0x0055: // Store registers V0 through Vx in memory starting at location I
                    debug_print("[OK] 0x%X: Fx55\n", opcode);
                    for (u8 byte = 0; byte < x + 1; byte++) {
                        chip8->memory[chip8->I_register + byte] = chip8->V_register[byte];
                    }
                    chip8->pc += 2;
                    break;
                case 0x0065: // Read registers V0 through Vx from memory starting at location I.
                    debug_print("[OK] 0x%X: Fx65\n", opcode);
                    for (u8 byte = 0; byte < x + 1; byte++) {
                        chip8->V_register[byte] = chip8->memory[chip8->I_register + byte];
                    }
                    chip8->pc += 2;
                    break;
            }
            break;
        default:
            debug_print("[FAILED] Unknown opcode: 0x%X\n", opcode);
            break;
    }

    // Decreament Delay/Sound Timers
    if (chip8->delay_timer > 0) chip8->delay_timer -= 1;
    if (chip8->sound_timer > 0) {
        chip8->sound_flag = 1;
        puts("BEEP");
        chip8->sound_timer -= 1;
    }
}

// Function to print memory
void print_memory(const chip8_t *chip8) {
    printf("Memory Dump (4096 bytes):\n");
    for (int i = 0; i < 4096; i += 16) {
        printf("0x%03X: ", i); // Print the memory address
        for (int j = 0; j < 16; j++) {
            printf("%02X ", chip8->memory[i + j]); // Print memory content in hex
        }
        printf("\n");
    }
}