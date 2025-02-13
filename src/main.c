#include <string.h>
#include "sys/timers.h"
#include "ti/screen.h"
#include "time.h"

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

    u32 last_time = clock();
    u32 timer_interval = 1000 / 60;

    kb_key_t key;
    while (chip8->state != (u8)QUIT) {
        // Fetch Inputs
        kb_Scan();
        input_handler(chip8);

        // Update Timers
        u32 current_time = clock();
        if (current_time - last_time >= timer_interval) {
            update_timers(chip8);
            last_time = current_time;
        }

        // Execute CPU Cycle
        for (u8 i = 0; i < CYCLES_PER_SEC; i++) {
            if (chip8->draw_flag == 1) break;
            cpu_cycle(chip8);
        }   

        // Display Buffer
        draw_chip8(chip8);
        chip8->draw_flag = 0;

        //delay(6); // 16 ms (Roughly 60 FPS)
    }

    free(chip8);
    gfx_End();
    os_ClrHomeFull();
    ti_Close(0);
    
    return EXIT_SUCCESS;
}