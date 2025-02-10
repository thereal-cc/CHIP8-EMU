#include "interface.h"
#include "utils.h"

void init_interface(interface_t *interface) 
{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL_Init Error: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    interface->window = SDL_CreateWindow("Chip-8", H_RES * SCALE, V_RES * SCALE, 0);
    if (!interface->window) {
        SDL_Log("SDL_Window Error: %s\n", SDL_GetError());
        SDL_Quit();
        exit(EXIT_FAILURE);
    }

    interface->renderer = SDL_CreateRenderer(interface->window, NULL);
    if (!interface->renderer) {
        SDL_Log("SDL_Renderer Error: %s\n", SDL_GetError());
        SDL_DestroyWindow(interface->window);
        SDL_Quit();
        exit(EXIT_FAILURE);
    }
}

void input_handler(chip8_t *chip8) 
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_EVENT_QUIT:
                chip8->state = QUIT;
                break;
            case SDL_EVENT_KEY_DOWN:
                switch(event.key.key){
                    // Chip8 Keypad
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

                    // Turn Quirks On/Off
                    case SDLK_F1:
                    if (!isQuirkEnabled(chip8, QUIRK_VF_RESET)) {
                        enableQuirk(chip8, QUIRK_VF_RESET);
                        break;
                    }
                    disableQuirk(chip8, QUIRK_VF_RESET);
                    break;

                    case SDLK_F2:
                    if (!isQuirkEnabled(chip8, QUIRK_LOADS)) {
                        enableQuirk(chip8, QUIRK_LOADS);
                        break;
                    }
                    disableQuirk(chip8, QUIRK_LOADS);
                    break;

                    case SDLK_F3:
                    if (!isQuirkEnabled(chip8, QUIRK_DISPWAIT)) {
                        enableQuirk(chip8, QUIRK_VF_RESET);
                        break;
                    }
                    disableQuirk(chip8, QUIRK_VF_RESET);
                    break;

                    case SDLK_F4:
                    if (!isQuirkEnabled(chip8, QUIRK_CLIP)) {
                        enableQuirk(chip8, QUIRK_CLIP);
                        break;
                    }
                    disableQuirk(chip8, QUIRK_CLIP);
                    break;

                    case SDLK_F5:
                    if (!isQuirkEnabled(chip8, QUIRK_SHIFT)) {
                        enableQuirk(chip8, QUIRK_SHIFT);
                        break;
                    }
                    disableQuirk(chip8, QUIRK_SHIFT);
                    break;

                    case SDLK_F6:
                    if (!isQuirkEnabled(chip8, QUIRK_JUMP)) {
                        enableQuirk(chip8, QUIRK_JUMP);
                        break;
                    }
                    disableQuirk(chip8, QUIRK_JUMP);
                    break;

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

void draw_chip8(interface_t *interface, u8 *buffer) 
{
    SDL_SetRenderDrawColor(interface->renderer, 0, 0, 0, 255);

    // Clear Current Rendering Target
    SDL_RenderClear(interface->renderer);
    SDL_SetRenderDrawColor(interface->renderer, 255, 255, 255, 255);

    for (u8 y = 0; y < V_RES; y++) {
        for (u8 x = 0; x < H_RES; x++) {
            if (buffer[x + (y * H_RES)]) {
                SDL_FRect rect;
                rect.x = x * SCALE;
                rect.y = y * SCALE;
                rect.w = SCALE;
                rect.h = SCALE;

                SDL_RenderFillRect(interface->renderer, &rect);
            }
        }
    }

    SDL_RenderPresent(interface->renderer);
}

void stop_interface(interface_t *interface) 
{
    SDL_DestroyRenderer(interface->renderer);
    SDL_DestroyWindow(interface->window);
    SDL_Quit();
}