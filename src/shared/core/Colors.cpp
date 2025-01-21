//
// Created by Matq on 20/01/2025.
//

#include "Colors.h"

namespace Colors {
SDL_FColor ColorToFloat(SDL_Color sdl_color) {
    return SDL_FColor((float)sdl_color.r / 255.0f,
                      (float)sdl_color.g / 255.0f,
                      (float)sdl_color.b / 255.0f,
                      (float)sdl_color.a / 255.0f);
}

SDL_Color ColorToInt(SDL_FColor sdl_fcolor) {
    return SDL_Color((int)(sdl_fcolor.r * 255.0f),
                     (int)(sdl_fcolor.g * 255.0f),
                     (int)(sdl_fcolor.b * 255.0f),
                     (int)(sdl_fcolor.a * 255.0f));
}
}
