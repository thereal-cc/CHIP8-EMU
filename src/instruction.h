#ifndef INSTRUCTION_H
#define INSTRUCTION_H

#include "utils.h"
#include "chip8.h"

// Opcode Function (Only needs Chip8 Object & Vx and Vy)
extern void (*opcode_table[16])(chip8_t *, u16, u8, u8);

// Opcodes (Sorted By Greatest Bit)
void chip8_op_0(chip8_t *chip8, u16 opcode, u8 x, u8 y); // 0NNN (SYS), 00E0 (CLS), 00EE (RET)
void chip8_op_1(chip8_t *chip8, u16 opcode, u8 x, u8 y); // 1NNN (JP addr)
void chip8_op_2(chip8_t *chip8, u16 opcode, u8 x, u8 y); // 2NNN (CALL addr)
void chip8_op_3(chip8_t *chip8, u16 opcode, u8 x, u8 y); // 3XNN (SE Vx, byte)
void chip8_op_4(chip8_t *chip8, u16 opcode, u8 x, u8 y); // 4XNN (SNE Vx, byte)
void chip8_op_5(chip8_t *chip8, u16 opcode, u8 x, u8 y); // 5XY0 (SE Vx, Vy)
void chip8_op_6(chip8_t *chip8, u16 opcode, u8 x, u8 y); // 6XNN (LD Vx, byte)
void chip8_op_7(chip8_t *chip8, u16 opcode, u8 x, u8 y); // 7XNN (ADD Vx, byte)
void chip8_op_8(chip8_t *chip8, u16 opcode, u8 x, u8 y); // 8XYN (Arithmetic / Bitwise Ops)
void chip8_op_9(chip8_t *chip8, u16 opcode, u8 x, u8 y); // 9XY0 (SNE Vx, Vy)
void chip8_op_A(chip8_t *chip8, u16 opcode, u8 x, u8 y); // ANNN (LD I, addr)
void chip8_op_B(chip8_t *chip8, u16 opcode, u8 x, u8 y); // BNNN (JP V0, addr)
void chip8_op_C(chip8_t *chip8, u16 opcode, u8 x, u8 y); // CXNN (RND Vx, byte)
void chip8_op_D(chip8_t *chip8, u16 opcode, u8 x, u8 y); // DXYN (DRW Vx, Vy, nibble)
void chip8_op_E(chip8_t *chip8, u16 opcode, u8 x, u8 y); // EX9E (SKP Vx), EXA1 (SKNP Vx)
void chip8_op_F(chip8_t *chip8, u16 opcode, u8 x, u8 y); // FXNN (Various opcodes for timers, memory, and input)

#endif