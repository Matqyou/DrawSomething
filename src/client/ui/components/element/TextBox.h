//
// Created by Matq on 16/12/2024.
//

#pragma once

#include <functional>
#include "base/Element.h"
#include "SDL_ttf.h"
#include "../../../core/app/Assets.h"

class Screen;
class TextBox : public Element {
private:
    std::string text;
    void (* enter_callback)(const std::string&);

    std::vector<Texture*> text_lines;
    bool update_render;
    SDL_Color text_color;

    static LoadedFont sDefaultFont;

    static std::vector<std::string> WrapText(const std::string& text, TTF_Font* font, int max_width);
    void UpdateRender();

public:
    TextBox(const Vec2i& pos, const Vec2i& size, void (* enter_callback)(const std::string&));
    ~TextBox();

    // Manipulating
    void AppendText(const char* input);
    void Backspace();

    // Ticking
    void HandleEvent(SDL_Event& event, EventContext& event_summary) override;
    void PostEvent() override;
    void Render() const override;

};