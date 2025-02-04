#ifndef INTERFACE_H  
#define INTERFACE_H

#include "SDL3/SDL.h"

#include "utils.h"
#include "chip8.h"

#define WINDOW_SCALE_REG 8

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    u8 extended; // 0 = 64x32, 1 = 128x64
    u8 render_width;
    u8 render_height;
    u8 render_offset_x;
    u8 render_offset_y;
} interface_t;

void init_interface(interface_t *interface);
void draw(interface_t *interface, u8 *buffer);
void sdl_ehandler(chip8_t *chip8);
void stop_interface(interface_t *interface);

#endif