//
// Created by Matq on 23/01/2025.
//

#pragma once

#include "base/Element.h"

class TextElement : public Element {
private:
    TextureData* generated_text;

public:
    explicit TextElement(const Vec2i& relative);
    ~TextElement() override;

    // Manipulation
    TextElement* UpdateText(TTF_Font* ttf_font, const char* new_text, SDL_Color sdl_color);

};
