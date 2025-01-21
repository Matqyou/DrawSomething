//
// Created by Matq on 20/01/2025.
//

#pragma once

#include <SDL3/SDL_pixels.h>

namespace Colors {

SDL_FColor ColorToFloat(SDL_Color sdl_color);
SDL_Color ColorToInt(SDL_FColor sdl_fcolor);

class IFColor {
private:
    SDL_Color sdl_color;
    SDL_FColor sdl_fcolor;

public:
    explicit IFColor(SDL_Color sdl_color) { SetColor(sdl_color); }
    explicit IFColor(SDL_FColor sdl_fcolor) { SetColor(sdl_fcolor); }

    // Getting
    [[nodiscard]] SDL_Color GetIColor() const { return sdl_color; }
    [[nodiscard]] SDL_FColor GetFColor() const { return sdl_fcolor; }

    // Manipulation
    void SetColor(SDL_Color sdl_color) {
        this->sdl_color = sdl_color;
        this->sdl_fcolor = ColorToFloat(sdl_color);
    }
    void SetColor(SDL_FColor sdl_fcolor) {
        this->sdl_color = ColorToInt(sdl_fcolor);
        this->sdl_fcolor = sdl_fcolor;
    }

};

}