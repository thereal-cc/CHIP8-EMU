#include "interface.h"
#include "utils.h"

u8 keymappings[16] = {
    SDL_SCANCODE_1, SDL_SCANCODE_2, SDL_SCANCODE_3, SDL_SCANCODE_4,
    SDL_SCANCODE_Q, SDL_SCANCODE_W, SDL_SCANCODE_E, SDL_SCANCODE_R,
    SDL_SCANCODE_A, SDL_SCANCODE_S, SDL_SCANCODE_D, SDL_SCANCODE_F,
    SDL_SCANCODE_Z, SDL_SCANCODE_X, SDL_SCANCODE_C, SDL_SCANCODE_V
};

void init_interface(interface_t *interface) {
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
        debug_print("SDL_Init Error: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    interface->window = SDL_CreateWindow("Chip-8", H_RES * WINDOW_SCALE, V_RES * WINDOW_SCALE, 0);
    if (!interface->window) {
        debug_print("SDL_Window Error: %s\n", SDL_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    interface->renderer = SDL_CreateRenderer(interface->window, NULL);
    if (!interface->renderer) {
        debug_print("SDL_Renderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(interface->window);
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
}

u8 sdl_ehandler(chip8_t *chip8) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            return 0;
        }
    }

    const bool* state = SDL_GetKeyboardState(NULL);
    
    if (state[SDL_SCANCODE_ESCAPE])  return 0;
    for (u8 keycode = 0; keycode < 16; keycode++) {
        chip8->keypad[keycode] = state[keymappings[keycode]] ? 1 : 0;;
    }

    return 1;
}

void draw(interface_t *interface, u8 *buffer) {
    SDL_SetRenderDrawColor(interface->renderer, 0, 0, 0, 255);

    // Clear Current Rendering Target
    SDL_RenderClear(interface->renderer);
    SDL_SetRenderDrawColor(interface->renderer, 255, 255, 255, 255);

    for (u8 y = 0; y < V_RES; y++) {
        for (u8 x = 0; x < H_RES; x++) {
            if (buffer[x + (y * 64)]) {
                SDL_FRect rect;
                rect.x = x * WINDOW_SCALE;
                rect.y = y * WINDOW_SCALE;
                rect.w = WINDOW_SCALE;
                rect.h = WINDOW_SCALE;

                SDL_RenderFillRect(interface->renderer, &rect);
            }
        }
    }

    SDL_RenderPresent(interface->renderer);
}

void stop_interface(interface_t *interface) {
    SDL_DestroyWindow(interface->window);
    SDL_Quit();
}