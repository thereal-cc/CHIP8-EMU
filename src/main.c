#include "chip8.h"

int main() {
    os_ClrHomeFull();
    ti_Close(0);
    
    // Initialize Chip8
    chip8_t *chip8 = malloc(sizeof(chip8_t));
    init_cpu(chip8);
    dbg_printf("[OK] Chip8 is Up and Running!\n");

    chip8->state = load_rom(chip8);
    if (chip8->state == (u8)QUIT) {
        free(chip8);
        os_ClrHomeFull();
        ti_Close(0);
        return EXIT_FAILURE;
    }
    
    dbg_printf("[OK] Rom is Up and Running!\n");

    gfx_Begin();
    gfx_SetDrawBuffer();

    kb_key_t key;
    clock_t start = clock();
    
    while (chip8->state != (u8)QUIT) {
        // Fetch Inputs
        kb_Scan();
        input_handler(chip8);

        clock_t now = clock();
        float elapsed = (float)(now - start) / CLOCKS_PER_SEC;
        if (elapsed >= 1.0f / TIMER_FREQUENCY) {
            start = now;
            update_timers(chip8);
        }

        // Execute CPU Cycle
        for (u8 i = 0; i < CYCLES_PER_SEC; i++) {
            if (chip8->draw_flag) break;
            cpu_cycle(chip8);
        }   

        // Display Buffer
        draw_chip8(chip8);
        chip8->draw_flag = 0;
    }

    free(chip8);
    gfx_End();
    os_ClrHomeFull();
    ti_Close(0);
    
    return EXIT_SUCCESS;
}