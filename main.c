#include "src/chip8.h"
#include "src/interface.h"
#include "src/tinyfiledialogs.h"

int main() {
    // Initialize CPU and Display
    chip8_t chip8;
    interface_t interface;

    memset(&chip8, 0, sizeof(chip8_t));
    memset(&interface, 0, sizeof(interface_t));  

    init_cpu(&chip8);
    SDL_Log("[OK] Chip8 is Up and Running!\n");

    init_interface(&interface);
    SDL_Log("[OK] Interface is Up and Running!\n");

    // Load Rom
    const char *filters[] = { "*.ch8" };
    const char *rompath = tinyfd_openFileDialog("Open Rom", "", 1, filters, "Chip8 Files", 0);
    if (rompath == NULL) {
        SDL_Log("[ERROR] No file selected or an error occured");
        stop_interface(&interface);
        return EXIT_FAILURE;
    }

    u8 status = load_rom(rompath, &chip8);
    if (status != 0) {
        SDL_Log("[ERROR] Unable to load rom\n");
        stop_interface(&interface);
        return EXIT_FAILURE;
    } 

    SDL_Log("[OK] Rom is Up and Running!\n");

    u32 last_time = SDL_GetTicks();
    u32 timer_interval = 1000 / 60;
    while (chip8.state != QUIT) {
         // Fetch Inputs
        input_handler(&chip8);

        // Update Timers
        u32 current_time = SDL_GetTicks();
        if (current_time - last_time >= timer_interval) {
            update_timers(&chip8);
            last_time = current_time;
        }

        // Execute CPU Cycle
        for (u8 i = 0; i < CYCLES_PER_SEC; i++) {
            if (chip8.draw_flag == 1) break;
            cpu_cycle(&chip8);
        }   

        // Display Buffer
        draw_chip8(&interface, chip8.buffer);
        chip8.draw_flag = 0;

        SDL_Delay(16); // 16 ms (Roughly 60 FPS)
    }

    stop_interface(&interface);
    return EXIT_SUCCESS;
}