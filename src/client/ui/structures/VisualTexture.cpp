//
// Created by Matq on 24/01/2025.
//

#include "VisualTexture.h"

VisualTexture::VisualTexture(SDL_Texture* sdl_texture, const Vec2d& visual_ratio, const Vec2d& visual_offset) {
    this->sdl_texture = sdl_texture;

    this->visual_rect = { 0, 0, 0, 0 };
    this->visual_ratio = visual_ratio;
    this->visual_offset = visual_offset;
}

VisualTexture::~VisualTexture() {

}

Vec2f VisualTexture::GetOriginalTextureSize() {
    Vec2f texture_size;
    SDL_GetTextureSize(sdl_texture, &texture_size.x, &texture_size.y);
    return texture_size;
}
