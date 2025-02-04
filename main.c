#include "src/chip8.h"
#include "src/interface.h"

int main(int argc, char *argv[]) {
    // Initialize CPU and Display
    chip8_t chip8;
    interface_t interface;

    memset(&chip8, 0, sizeof(chip8_t));
    memset(&interface, 0, sizeof(interface_t));  

    init_cpu(&chip8);
    printf("[OK] Chip8 is Up and Running!\n");

    init_interface(&interface);
    printf("[OK] Interface is Up and Running!\n");

    // Load Rom
    char *rompath = (argc != 2) ? (printf("No File Loaded, Using Test Rom\n"), "tests/1-chip8-logo.ch8") : argv[1];
    u8 status = load_rom(rompath, &chip8);
    if (status != 1) {
        printf("[ERROR] Unable to load rom\n");
        return EXIT_FAILURE;
    } 
    printf("[OK] Rom is Up and Running!\n");

    /* Uncomment to Enable Quirks */
    //enableQuirk(&chip8, QUIRK_VF_RESET);
    //enableQuirk(&chip8, QUIRK_LOADS);
    //enableQuirk(&chip8, QUIRK_DISPWAIT);
    //enableQuirk(&chip8, QUIRK_CLIP);
    //enableQuirk(&chip8, QUIRK_SHIFT);
    //enableQuirk(&chip8, QUIRK_JUMP);

    u32 last_time = SDL_GetTicks();
    u32 timer_interval = 1000 / 60;
    while (chip8.state != QUIT) {
         // Fetch Inputs
        sdl_ehandler(&chip8);

        // Update Timers
        u32 current_time = SDL_GetTicks();
        if (current_time - last_time >= timer_interval) {
            last_time = current_time;
            update_timers(&chip8);
            last_time = current_time;
        }

        // Execute CPU Cycle
        for (u8 i = 0; i < CYCLES_PER_SEC; i++) {
            if (chip8.draw_flag == 1) break;
            cpu_cycle(&chip8);
        }   

        draw(&interface, chip8.buffer);
        chip8.draw_flag = 0;

        SDL_Delay(16); // 16.67 ms (60 FPS)
    }

    stop_interface(&interface);
    return EXIT_SUCCESS;
}