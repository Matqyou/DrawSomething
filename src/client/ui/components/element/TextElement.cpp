//
// Created by Matq on 23/01/2025.
//

#include "TextElement.h"

TextElement::TextElement(const Vec2i& relative)
    : Element(relative, Vec2i(60, 40), ElementDraw::DRAW_TEXTURE) {
    name = L"TextElement";
    generated_text = nullptr;
}

TextElement::~TextElement() {
    delete generated_text;
}

TextElement* TextElement::UpdateText(TTF_Font* ttf_font, const char* new_text, SDL_Color sdl_color) {
    auto assets = Assets::Get();
    delete generated_text;
    generated_text = assets->RenderTextBlended(ttf_font, new_text, sdl_color);
    sdl_texture = generated_text->SDLTexture();
    size = Vec2i(generated_text->GetSize());
    visual_size = Vec2i(generated_text->GetSize());
    return this;
}
