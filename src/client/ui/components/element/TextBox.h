//
// Created by Matq on 16/12/2024.
//

#pragma once

#include <functional>
#include "base/Element.h"
#include "SDL3_ttf/SDL_ttf.h"
#include "../../../core/app/Assets.h"

class TextBox : public Element {
private:
    TTF_Font* font;
    std::string text;
    using Callback = std::function<void(std::string&)>;
    Callback callback;

    std::vector<Texture*> text_lines;
    bool update_render;
    SDL_Color text_color;

    static std::vector<std::string> WrapText(const std::string& text, TTF_Font* font, int max_width);
    void UpdateRender();

public:
    TextBox(const Vec2i& pos, const Vec2i& size);
    ~TextBox() override;

    // Options
    TextBox* SetCallback(Callback callback) {
        this->callback = callback;
        return this;
    }
    TextBox* SetFont(TTF_Font* font) {
        this->font = font;
        return this;
    }
    TextBox* SetTextColor(SDL_Color sdl_color) {
        this->text_color = sdl_color;
        return this;
    }

    // Manipulating
    void AppendText(const char* input);
    void Backspace();

    // Ticking
    void HandleEvent(SDL_Event& event, EventContext& event_summary) override;
    void PostEvent() override;
    void Render() override;

};