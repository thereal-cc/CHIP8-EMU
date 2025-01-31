#ifndef INTERFACE_H  
#define INTERFACE_H

#include <SDL3/SDL.h>

#include "utils.h"
#include "chip8.h"

#define WINDOW_SCALE 8

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
} interface_t;

void init_interface(interface_t *interface);
void draw(interface_t *interface, u8 *buffer);
u8 sdl_ehandler(chip8_t *chip8);
void stop_interface(interface_t *interface);

#endif