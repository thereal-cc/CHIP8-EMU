#include "src/chip8.h"
#include "src/interface.h"

int main(int argc, char *argv[]) {
    // Initialize CPU and Display
    chip8_t chip8;
    interface_t interface;
    struct timespec req = { 0, 16670000L }; // 0 seconds, 16.67 miliseconds

    memset(&chip8, 0, sizeof(chip8_t));
    memset(&interface, 0, sizeof(interface_t));  

    init_cpu(&chip8);
    puts("[OK] Chip8 is Up and Running!");

    init_interface(&interface);
    puts("[OK] Interface is Up and Running!");

    u8 status = load_rom("./tests/1-chip8-logo.ch8", &chip8);
    if (status != 1) {
        puts("[ERROR] Unable to load rom");
        chip8.state = QUIT;
    } else {
        puts("[OK] Rom is Up and Running");
    }

    print_memory(&chip8);

    while (chip8.state != QUIT) {
        cpu_cycle(&chip8);
        chip8.state = sdl_ehandler(&chip8);

        if (chip8.draw_flag) {
            draw(&interface, chip8.buffer);
        }

        nanosleep(&req.tv_sec, &req.tv_nsec);
    }

    stop_interface(&interface);
    return EXIT_SUCCESS;
}