#include <string.h>
#include <time.h>
#include "fileioc.h"
#include "chip8.h"
#include "instruction.h"
#include "ti/screen.h"
#include "tice.h"

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
}

void cpu_cycle(chip8_t *chip8) 
{
    // Get Opcode, Nibble, Vx, and Vy
    u16 opcode = chip8->memory[chip8->pc] << 8 | chip8->memory[chip8->pc + 1];
    u16 highest_nibble = (opcode & 0xF000) >> 12;
    u8 x = (opcode & 0x0F00) >> 8;
    u8 y = (opcode & 0x00F0) >> 4;

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
        dbg_printf("BEEP\n");
        chip8->sound_timer -= 1;
        if (chip8->sound_timer == 0) chip8->sound_flag += 1;
    }
}

/* Filesystem Functions */
u8 load_rom(chip8_t *chip8) 
{
    char rom_name[ROM_NAME_SIZE];
    ti_var_t rom_slot;
    os_GetStringInput("Enter Rom Name: ", rom_name, ROM_NAME_SIZE);
    asm_NewLine();

    rom_slot = ti_Open(rom_name, "r");
    if (rom_slot == 0) {
        os_PutStrFull("Error: Rom Not Found\n");
        while (!os_GetCSC());
        return -1;
    }

    u16 rom_size = ti_GetSize(rom_slot);
    if (rom_size > MAX_MEMORY - 0x200) {
        os_PutStrFull("Error: Rom Size Too Large\n");
        while (!os_GetCSC());
        return -1;
    }

    size_t chunks_read = ti_Read(&chip8->memory[0x200], rom_size, 1, rom_slot);
    if (chunks_read != 1) {
        os_PutStrFull("Error: Rom Read Failed\n");
        while (!os_GetCSC());
        return -1;
    }

    return 0;
}

void input_handler(chip8_t *chip8) 
{
    if (kb_IsDown(kb_KeyDel)) {
        chip8->state = QUIT;
        return;
    }

    // Reset all keypad states before updating
    memset(chip8->keypad, 0, sizeof(chip8->keypad));

    chip8->keypad[0x1] = kb_IsDown(kb_Key7);
    chip8->keypad[0x2] = kb_IsDown(kb_Key8);
    chip8->keypad[0x3] = kb_IsDown(kb_Key9);
    chip8->keypad[0xC] = kb_IsDown(kb_KeyMul);

    chip8->keypad[0x4] = kb_IsDown(kb_Key4);
    chip8->keypad[0x5] = kb_IsDown(kb_Key5);
    chip8->keypad[0x6] = kb_IsDown(kb_Key6);
    chip8->keypad[0xD] = kb_IsDown(kb_KeySub);

    chip8->keypad[0x7] = kb_IsDown(kb_Key1);
    chip8->keypad[0x8] = kb_IsDown(kb_Key2);
    chip8->keypad[0x9] = kb_IsDown(kb_Key3);
    chip8->keypad[0xE] = kb_IsDown(kb_KeyAdd);

    chip8->keypad[0xA] = kb_IsDown(kb_Key0);  
    chip8->keypad[0x0] = kb_IsDown(kb_KeyDecPnt);
    chip8->keypad[0xB] = kb_IsDown(kb_KeyDiv);  
    chip8->keypad[0xF] = kb_IsDown(kb_KeyEnter);
                    
    // Turn Quirks On/Off
    if (kb_IsDown(kb_KeyMath)) {
        if (!isQuirkEnabled(chip8, QUIRK_VF_RESET)) {
            enableQuirk(chip8, QUIRK_VF_RESET);
        } else {
            disableQuirk(chip8, QUIRK_VF_RESET);
        }
    }

    else if (kb_IsDown(kb_KeyApps)) {
        if (!isQuirkEnabled(chip8, QUIRK_LOADS)) {
            enableQuirk(chip8, QUIRK_LOADS);
        } else {
            disableQuirk(chip8, QUIRK_LOADS);
        }
    }

    else if (kb_IsDown(kb_KeyPrgm)) {
        if (!isQuirkEnabled(chip8, QUIRK_DISPWAIT)) {
            enableQuirk(chip8, QUIRK_DISPWAIT);
        } else {
            disableQuirk(chip8, QUIRK_DISPWAIT);
        }
    }

    else if (kb_IsDown(kb_KeyVars)) {
        if (!isQuirkEnabled(chip8, QUIRK_CLIP)) {
            enableQuirk(chip8, QUIRK_CLIP);
        } else {
            disableQuirk(chip8, QUIRK_CLIP);
        }
    }
    
    else if (kb_IsDown(kb_KeyClear)) {
        if (!isQuirkEnabled(chip8, QUIRK_SHIFT)) {
            enableQuirk(chip8, QUIRK_SHIFT);
        } else {
            disableQuirk(chip8, QUIRK_SHIFT);
        }
    }

    else if (kb_IsDown(kb_KeyPower)) {
        if (!isQuirkEnabled(chip8, QUIRK_JUMP)) {
            enableQuirk(chip8, QUIRK_JUMP);
        } else {
            disableQuirk(chip8, QUIRK_JUMP);
        }
    }
}

void draw_chip8(chip8_t *chip8) 
{
    gfx_ZeroScreen();
    // Draw Quriks
    for (u8 i = 0; i < 6; i++) {
        gfx_SetColor(0xFF);
        if (isQuirkEnabled(chip8, i)) {
            gfx_SetColor(0x06);
        }

        gfx_FillRectangle(40 * i + 55, 10, 10, 10);
    }

    
    gfx_SetColor(0xFF);
    u8 y_offset = V_RES + 8;
    
    for (u8 y = 0; y < V_RES; y++) {
        for (u8 x = 0; x < H_RES; x++) {
            if (chip8->buffer[x + (y * H_RES)]) {
                gfx_FillRectangle(x * SCALE, (y * SCALE) + y_offset, SCALE, SCALE);
            }
        }
    }

    gfx_SwapDraw();
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

bool isQuirkEnabled(chip8_t *chip8, u8 quirk) 
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