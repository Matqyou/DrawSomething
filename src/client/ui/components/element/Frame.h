//
// Created by Matq on 16/12/2024.
//

#pragma once

#include "base/Element.h"

class Frame : public Element {
public:
    Frame();
    Frame(const Vec2i& pos, const Vec2i& size, ElementDraw draw);
    Frame(const Vec2i& pos, const Vec2i& size, SDL_Texture* sdl_texture);
    Frame(const Vec2i& pos, const Vec2i& size, const Vec2i& visual, const Vec2i& offset, SDL_Texture* sdl_texture);
    Frame(const Vec2i& pos, const Vec2i& size, const VisualTexture& visual_texture);

};