//
// Created by Matq on 20/01/2025.
//

#pragma once

#include "../../../core/app/Assets.h"

class Sheet {
private:
    TextureData* sheet;

    TextureData* top_left;
    TextureData* top;
    TextureData* top_right;
    TextureData* left;
    TextureData* middle;
    TextureData* right;
    TextureData* bottom_left;
    TextureData* bottom;
    TextureData* bottom_right;

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
    void Generate(AssetsClass* assets, TextureData* sheet);

};
