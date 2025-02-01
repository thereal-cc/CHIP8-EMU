#include "interface.h"
#include "utils.h"

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

void sdl_ehandler(chip8_t *chip8) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_EVENT_QUIT:
                chip8->state = QUIT;
                break;
            case SDL_EVENT_KEY_DOWN:
                switch(event.key.key){
                    case SDLK_1:chip8->keypad[0x1] = true; break;
                    case SDLK_2:chip8->keypad[0x2] = true; break;
                    case SDLK_3:chip8->keypad[0x3] = true; break;
                    case SDLK_4:chip8->keypad[0xc] = true; break;

                    case SDLK_Q:chip8->keypad[0x4] = true; break;
                    case SDLK_W:chip8->keypad[0x5] = true; break;
                    case SDLK_E:chip8->keypad[0x6] = true; break;
                    case SDLK_R:chip8->keypad[0xd] = true; break;

                    case SDLK_A:chip8->keypad[0x7] = true; break;
                    case SDLK_S:chip8->keypad[0x8] = true; break;
                    case SDLK_D:chip8->keypad[0x9] = true; break;
                    case SDLK_F:chip8->keypad[0xe] = true; break;
                                
                    case SDLK_Z:chip8->keypad[0xa] = true; break;
                    case SDLK_X:chip8->keypad[0x0] = true; break;
                    case SDLK_C:chip8->keypad[0xb] = true; break;
                    case SDLK_V:chip8->keypad[0xf] = true; break;
                    default: break;
                }
                break;
            case SDL_EVENT_KEY_UP:
                switch(event.key.key){
                    case SDLK_1:chip8->keypad[0x1] = false; break;
                    case SDLK_2:chip8->keypad[0x2] = false; break;
                    case SDLK_3:chip8->keypad[0x3] = false; break;
                    case SDLK_4:chip8->keypad[0xc] = false; break;

                    case SDLK_Q:chip8->keypad[0x4] = false; break;
                    case SDLK_W:chip8->keypad[0x5] = false; break;
                    case SDLK_E:chip8->keypad[0x6] = false; break;
                    case SDLK_R:chip8->keypad[0xd] = false; break;

                    case SDLK_A:chip8->keypad[0x7] = false; break;
                    case SDLK_S:chip8->keypad[0x8] = false; break;
                    case SDLK_D:chip8->keypad[0x9] = false; break;
                    case SDLK_F:chip8->keypad[0xe] = false; break;

                    case SDLK_Z:chip8->keypad[0xa] = false; break;
                    case SDLK_X:chip8->keypad[0x0] = false; break;
                    case SDLK_C:chip8->keypad[0xb] = false; break;
                    case SDLK_V:chip8->keypad[0xf] = false; break;
                    default:break;
                }
            break;
        }
    }
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
    SDL_DestroyRenderer(interface->renderer);
    SDL_DestroyWindow(interface->window);
    SDL_Quit();
}