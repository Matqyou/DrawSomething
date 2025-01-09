//
// Created by Matq on 16/12/2024.
//

#pragma once

#include "base/Element.h"

class Frame : public Element {
public:
    Frame(const Vec2i& pos, const Vec2i& size);
    Frame(const Vec2i& pos, const Vec2i& size, ElementDraw draw);
    Frame(const Vec2i& pos, const Vec2i& size, const Vec2i& visual, const Vec2i& offset);
    Frame(const Vec2i& pos, const Vec2i& size, const Vec2i& visual, const Vec2i& offset, Texture* texture);
    Frame(const Vec2i& pos, const Vec2i& size, Texture* texture);

};