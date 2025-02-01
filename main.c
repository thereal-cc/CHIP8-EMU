#include "src/chip8.h"
#include "src/interface.h"
#include <unistd.h>

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

    char *rompath = (argc != 2) ? (printf("No File Loaded, Using Test Rom\n"), "tests/1-chip8-logo.ch8") : argv[1];
    u8 status = load_rom(rompath, &chip8);
    if (status != 1) {
        printf("[ERROR] Unable to load rom\n");
        return EXIT_FAILURE;
    } 

    printf("[OK] Rom is Up and Running!\n");

    while (chip8.state != QUIT) {
        sdl_ehandler(&chip8);
        cpu_cycle(&chip8);

        if (chip8.draw_flag) {
            draw(&interface, chip8.buffer);
            chip8.draw_flag = 0;
        }

        SDL_Delay(16.67); // 16.67 ms (60 FPS)
    }

    stop_interface(&interface);
    return EXIT_SUCCESS;
}