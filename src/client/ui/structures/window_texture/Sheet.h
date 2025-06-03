//
// Created by Matq on 20/01/2025.
//

#pragma once

#include "../../../core/app/Assets.h"

class Sheet {
private:
    Texture* sheet;

    Texture* top_left;
    Texture* top;
    Texture* top_right;
    Texture* left;
    Texture* middle;
    Texture* right;
    Texture* bottom_left;
    Texture* bottom;
    Texture* bottom_right;

public:
    SDL_Texture* top_left_sdl;
    SDL_Texture* top_sdl;
    SDL_Texture* top_right_sdl;
    SDL_Texture* left_sdl;
    SDL_Texture* middle_sdl;
    SDL_Texture* right_sdl;
    SDL_Texture* bottom_left_sdl;
    SDL_Texture* bottom_sdl;
    SDL_Texture* bottom_right_sdl;

    Sheet();
    ~Sheet();

    // Manipulation
    void Generate(AssetsClass* assets, Texture* sheet);

};
