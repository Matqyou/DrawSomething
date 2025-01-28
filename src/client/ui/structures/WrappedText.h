//
// Created by Matq on 19/01/2025.
//

#pragma once

#include <utility>

#include "../../core/app/Assets.h"

class WrappedText {
private:
    std::string text;
    TTF_Font* font;
    std::vector<TextureData*> text_lines;
    Vec2i text_lines_size;
    SDL_Color text_color;

public:
    WrappedText(TTF_Font* font, SDL_Color text_color);
    ~WrappedText();

    // Getting
    [[nodiscard]] std::string& GetText() { return text; }
    [[nodiscard]] std::vector<TextureData*>& GetRenderLines() { return text_lines; }
    [[nodiscard]] Vec2i GetSize() const { return text_lines_size; }
    [[nodiscard]] int GetWidth() const { return text_lines_size.x; }
    [[nodiscard]] int GetHeight() const { return text_lines_size.y; }
    [[nodiscard]] int GetLineHeight() const { return TTF_GetFontHeight(font); }

    // Options
    WrappedText* SetText(std::string new_text) {
        this->text = std::move(new_text);
        return this;
    }
    WrappedText* SetFont(TTF_Font* new_font) {
        this->font = new_font;
        return this;
    }
    WrappedText* SetTextColor(SDL_Color new_color) {
        this->text_color = new_color;
        return this;
    }

    // Manipulating
    static std::vector<std::string> WrapText(const std::string& text,
                                             TTF_Font* font,
                                             int max_width,
                                             Vec2i* out_size);
    void UpdateRender();
    void AppendText(const char* input);
    void Backspace();

};
