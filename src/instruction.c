#include "instruction.h"
#include "debug.h"
#include <string.h>

void (*opcode_table[16])(chip8_t *, u16, u8, u8) = {
    chip8_op_0,
    chip8_op_1,
    chip8_op_2,
    chip8_op_3,
    chip8_op_4,
    chip8_op_5,
    chip8_op_6,
    chip8_op_7,
    chip8_op_8,
    chip8_op_9,
    chip8_op_A,
    chip8_op_B,
    chip8_op_C,
    chip8_op_D,
    chip8_op_E,
    chip8_op_F
};

void chip8_op_0(chip8_t *chip8, u16 opcode, u8 x, u8 y) {
    switch (opcode) {
        case 0x00E0: // Clear Display
            dbg_printf("[OK] 0x%X: 00E0\n", opcode);
            memset(chip8->buffer, 0, sizeof(chip8->buffer));
            break;
        case 0x00EE: // Return from subroutine
            dbg_printf("[OK] 0x%X: 00EE\n", opcode);
            chip8->pc = chip8->stack[--chip8->sp];
            chip8->increment_pc = false;
            break;
        default:
            dbg_printf("[FAILED] Unknown opcode: 0x%X\n", opcode);
            break;
    }
}

void chip8_op_1(chip8_t *chip8, u16 opcode, u8 x, u8 y) {
    dbg_printf("[OK] 0x%X: 1nnn\n", opcode);
    chip8->pc = opcode & 0x0FFF;
    chip8->increment_pc = false;
}

void chip8_op_2(chip8_t *chip8, u16 opcode, u8 x, u8 y) {
    dbg_printf("[OK] 0x%X: 2nnn\n", opcode);
    chip8->stack[chip8->sp++] = chip8->pc + 2;
    chip8->pc = opcode & 0x0FFF;
    chip8->increment_pc = false;
}

void chip8_op_3(chip8_t *chip8, u16 opcode, u8 x, u8 y) {
    dbg_printf("[OK] 0x%X: 3xkk\n", opcode);
    if (chip8->V_register[x] == (opcode & 0x00FF)) chip8->pc += 2;
}

void chip8_op_4(chip8_t *chip8, u16 opcode, u8 x, u8 y) {
    dbg_printf("[OK] 0x%X: 4xkk\n", opcode);
    if (chip8->V_register[x] != (opcode & 0x00FF)) chip8->pc += 2;
}

void chip8_op_5(chip8_t *chip8, u16 opcode, u8 x, u8 y) {
    dbg_printf("[OK] 0x%X: 5xy0\n", opcode);
    if (chip8->V_register[x] == chip8->V_register[y]) chip8->pc += 2;
}

void chip8_op_6(chip8_t *chip8, u16 opcode, u8 x, u8 y) {
    dbg_printf("[OK] 0x%X: 6xkk\n", opcode);
    chip8->V_register[x] = (opcode & 0x00FF);
}

void chip8_op_7(chip8_t *chip8, u16 opcode, u8 x, u8 y) {
    dbg_printf("[OK] 0x%X: 7xkk\n", opcode);
    u8 value = (opcode & 0x00FF);
    chip8->V_register[x] = (chip8->V_register[x] + value) & 0xFF;
}

void chip8_op_8(chip8_t *chip8, u16 opcode, u8 x, u8 y) {
    bool carry;
    switch (opcode & 0x000F) {
        case 0x00: // Vx = Vy
            dbg_printf("[OK] 0x%X: 8xy0\n", opcode);
            chip8->V_register[x] = chip8->V_register[y];
            break;
        case 0x01: // Vx = Vx OR Vy
            dbg_printf("[OK] 0x%X: 8xy1\n", opcode);
            chip8->V_register[x] = (chip8->V_register[x] | chip8->V_register[y]);
            if (!isQuirkEnabled(chip8, QUIRK_VF_RESET)) chip8->V_register[0xF] = 0;
            break;
        case 0x02: // Vx = Vx AND Vy
            dbg_printf("[OK] 0x%X: 8xy2\n", opcode);
            chip8->V_register[x] = (chip8->V_register[x] & chip8->V_register[y]);
            if (!isQuirkEnabled(chip8, QUIRK_VF_RESET)) chip8->V_register[0xF] = 0;
            break;
        case 0x03: // Vx = Vx XOR Vy
            dbg_printf("[OK] 0x%X: 8xy3\n", opcode);
            chip8->V_register[x] ^= chip8->V_register[y];
            if (!isQuirkEnabled(chip8, QUIRK_VF_RESET)) chip8->V_register[0xF] = 0;
            break;
        case 0x04: // Vx = Vx + Vy, set VF = carry
            dbg_printf("[OK] 0x%X: 8xy4\n", opcode);
            u16 sum = chip8->V_register[x] + chip8->V_register[y];
            chip8->V_register[x] = sum & 0xFF;
            chip8->V_register[0xF] = (sum > 0x100);
            break;
        case 0x05: // Vx = Vx - Vy, set VF = NOT borrow
            dbg_printf("[OK] 0x%X: 8xy5\n", opcode);
            carry = chip8->V_register[x] >= chip8->V_register[y];
            chip8->V_register[x] = (chip8->V_register[x] - chip8->V_register[y]) & 0xFF;
            chip8->V_register[0xF] = carry;
            break;
        case 0x06: // Vx = Vx SHR 1
            dbg_printf("[OK] 0x%X: 8xy6\n", opcode);
            if (!isQuirkEnabled(chip8, QUIRK_SHIFT)) chip8->V_register[x] = chip8->V_register[y];
            const bool lsb = (chip8->V_register[x] & 0x1) == 1;
            chip8->V_register[x] >>= 1;
            chip8->V_register[0xF] = lsb;
            break;
        case 0x07: // Vx = Vy - Vx, set VF = NOT borrow
            dbg_printf("[OK] 0x%X: 8xy7\n", opcode);
            carry = chip8->V_register[y] >= chip8->V_register[x];
            chip8->V_register[x] = (chip8->V_register[y] - chip8->V_register[x]) & 0xFF;
            chip8->V_register[0xF] = carry;
            break;
        case 0x0E: // Vx = Vx SHL 1
            dbg_printf("[OK] 0x%X: 8xyE\n", opcode);
            if (!isQuirkEnabled(chip8, QUIRK_SHIFT)) chip8->V_register[x] = chip8->V_register[y];
            const bool msb = (chip8->V_register[x] >> 7) == 1;
            chip8->V_register[x] <<= 1;
            chip8->V_register[0xF] = msb;
            break;
        default:
            dbg_printf("[FAILED] Unknown opcode: 0x%X\n", opcode);
            break;
    }
}

void chip8_op_9(chip8_t *chip8, u16 opcode, u8 x, u8 y) {
    dbg_printf("[OK] 0x%X: 9xy0\n", opcode);
    if (chip8->V_register[x] != chip8->V_register[y]) chip8->pc += 2;
}

void chip8_op_A(chip8_t *chip8, u16 opcode, u8 x, u8 y) {
    dbg_printf("[OK] 0x%X: Annn\n", opcode);
    chip8->I_register = (opcode & 0x0FFF);
}

void chip8_op_B(chip8_t *chip8, u16 opcode, u8 x, u8 y) {
    dbg_printf("[OK] 0x%X: Bnnn\n", opcode);
    if (!isQuirkEnabled(chip8, QUIRK_JUMP)) chip8->pc = (opcode & 0x0FFF) + chip8->V_register[0];
    else chip8->pc = (opcode & 0x0FFF) + chip8->V_register[x];
    chip8->increment_pc = false;
}

void chip8_op_C(chip8_t *chip8, u16 opcode, u8 x, u8 y) {
    dbg_printf("[OK] 0x%X: Cxkk\n", opcode);
    chip8->V_register[x] = (random() % 256) & (opcode & 0x00FF);
}

void chip8_op_D(chip8_t *chip8, u16 opcode, u8 x, u8 y) {
    dbg_printf("[OK] 0x%X: Dxyn\n", opcode);
    chip8->draw_flag = 1;
    chip8->V_register[0xF] = 0;

    // Get Size, X/Y Coordinates, Set Collision to 0
    u8 sprite_size = opcode & 0x000F;
    u8 x_coord = chip8->V_register[x] % H_RES;
    u8 y_coord = chip8->V_register[y] % V_RES;
    u8 bit = 8;
    u8 pixel;

    for (u8 j = 0; j < sprite_size; j++) {
        if (y_coord + j >= V_RES) break;
        pixel = chip8->memory[chip8->I_register + j];
        // Each Bit in the Byte
        for (u8 i = 0; i < bit; i++) {
            if (x_coord + i >= H_RES) break;
            if ((pixel & (0x80 >> i)) != 0) {
                u16 buffer_index = (x_coord + i) + ((y_coord + j) * H_RES);
                if (chip8->buffer[buffer_index]) {
                    chip8->V_register[0xF] = 1;
                }

                chip8->buffer[buffer_index] ^= 1;
            }
        }
    }
}

void chip8_op_E(chip8_t *chip8, u16 opcode, u8 x, u8 y) {
    switch (opcode & 0x00FF) {
        case 0x009E: // Skip if key with value in Vx is pressed
            dbg_printf("[OK] 0x%X: Ex9E\n", opcode);
            if (chip8->keypad[chip8->V_register[x]]) chip8->pc += 2;
            break;
        case 0x00A1:
            dbg_printf("[OK] 0x%X: ExA1\n", opcode);
            if (!chip8->keypad[chip8->V_register[x]]) chip8->pc += 2;
            break;
        default:
            dbg_printf("[FAILED] Unknown opcode: 0x%X\n", opcode);
            break;
    }
}

void chip8_op_F(chip8_t *chip8, u16 opcode, u8 x, u8 y) {
    switch(opcode & 0x00FF) {
        case 0x0007: // Vx = delay timer
            dbg_printf("[OK] 0x%X: Fx07\n", opcode);
            chip8->V_register[x] = chip8->delay_timer;
            break;
        case 0x000A: // Wait for key press, store key value in Vx
            dbg_printf("[OK] 0x%X: Fx0A\n", opcode);
            bool key_pressed = false;
            for (u8 i = 0; i < 16; i++) {
                if (chip8->keypad[i]) {
                    chip8->V_register[x] = chip8->keypad[i];
                    key_pressed = true;
                    break;
                }
            }
            if (!key_pressed) chip8->pc -= 2;
            break;
        case 0x0015: // Delay Timer = Vx
            dbg_printf("[OK] 0x%X: Fx15\n", opcode);
            chip8->delay_timer = chip8->V_register[x];
            break;
        case 0x0018: // Sound Timer = Vx
            dbg_printf("[OK] 0x%X: Fx18\n", opcode);
            chip8->sound_timer = chip8->V_register[x];
            break;
        case 0x001E: // I = I + Vx
            dbg_printf("[OK] 0x%X: Fx1E\n", opcode);
            chip8->I_register += chip8->V_register[x];
            break;
        case 0x0029: // I = Location of Sprite for digit V[x]
            dbg_printf("[OK] 0x%X: Fx29\n", opcode);
            // SChip Behavior
            u8 digit = chip8->V_register[x] & 0x0F;
            chip8->I_register = chip8->V_register[x] + (digit * 5);
            break;
        case 0x0033: // Store BCD representation of Vx in memory locations I, I+1, and I+2.
            dbg_printf("[OK] 0x%X: Fx33\n", opcode);
            u8 value = chip8->V_register[x];
            chip8->memory[chip8->I_register] = value / 100; // Hundreds Digit
            chip8->memory[chip8->I_register + 1] = (value / 10) % 10; // Tens Digit
            chip8->memory[chip8->I_register + 2] = value % 10; // Ones Digit
            break;
        case 0x0055: // Store registers V0 through Vx in memory starting at location I
            dbg_printf("[OK] 0x%X: Fx55\n", opcode);
            for (u16 i = 0; i <= x; i++) {
                chip8->memory[chip8->I_register + i] = chip8->V_register[i];
            }
            if (!isQuirkEnabled(chip8, QUIRK_LOADS)) chip8->I_register += (x + 1);
            break;
        case 0x0065: // Read registers V0 through Vx from memory starting at location I.
            dbg_printf("[OK] 0x%X: Fx65\n", opcode);
            for (u16 i = 0; i <= x; i++) {
                chip8->V_register[i] = chip8->memory[chip8->I_register + i];
            }
            if (!isQuirkEnabled(chip8, QUIRK_LOADS)) chip8->I_register += (x + 1);
            break;
        default:
            dbg_printf("[FAILED] Unknown opcode: 0x%X\n", opcode);
            break;
    }
}