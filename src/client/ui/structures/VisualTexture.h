//
// Created by Matq on 24/01/2025.
//

#pragma once

#include "../../core/app/Assets.h"

class VisualTexture {
private:
    SDL_Texture* sdl_texture;

    SDL_FRect visual_rect;
    Vec2d visual_ratio;
    Vec2d visual_offset;

public:
    explicit VisualTexture(SDL_Texture* sdl_texture = nullptr,
                           const Vec2d& visual_ratio = Vec2d(1.0, 1.0),
                           const Vec2d& visual_offset = Vec2d(0.0, 0.0));
    ~VisualTexture();

    // Getting
    [[nodiscard]] SDL_FRect GetVisualRect() const { return visual_rect; }
    [[nodiscard]] SDL_Texture* SDLTexture() const { return sdl_texture; }

    // Manipulation
    void UpdateRect(const SDL_FRect& new_rect) {
        this->visual_rect.w = (float)((double)(new_rect.w) * visual_ratio.x);
        this->visual_rect.h = (float)((double)(new_rect.h) * visual_ratio.y);
        this->visual_rect.x = (float)((double)(new_rect.x) + (double)(visual_rect.w) * visual_offset.x);
        this->visual_rect.y = (float)((double)(new_rect.y) + (double)(visual_rect.h) * visual_offset.y);
    }
    void SetSDLTexture(SDL_Texture* sdl_texture) {
        this->sdl_texture = sdl_texture;
    }

};
