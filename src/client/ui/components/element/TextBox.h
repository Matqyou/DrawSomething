//
// Created by Matq on 16/12/2024.
//

#pragma once

#include <SDL3_ttf/SDL_ttf.h>
#include <functional>
#include <utility>
#include "base/Element.h"
#include "../../../core/app/Assets.h"
#include "../../structures/WrappedText.h"

class TextBox : public Element {
private:
    using Callback = std::function<void(std::string&)>;

    // Core
    WrappedText text;
    WrappedText placeholder;
    Callback callback;

    // Visual
    Vec2i text_pos, placeholder_pos;
    bool update_text;
    bool update_placeholder;
    SimpleAlign text_align_horizontally, text_align_vertically;

    void UpdateTextPosition(WrappedText& text, Vec2i* out_pos);

public:
    TextBox(const Vec2i& pos, const Vec2i& size);
    TextBox(const Vec2i& pos, const Vec2i& size, const Vec2i& visual, const Vec2i& offset, Texture* texture);
    ~TextBox() override;

    // Getting
    [[nodiscard]] WrappedText& GetText() { return text; }
    [[nodiscard]] WrappedText& GetPlaceholder() { return text; }

    // Options
    TextBox* SetFont(TTF_Font* new_font) {
        this->text.SetFont(new_font);
        this->placeholder.SetFont(new_font);
        return this;
    }
    TextBox* SetTextColor(SDL_Color new_color) {
        this->text.SetTextColor(new_color);
        return this;
    }
    TextBox* SetPlaceholderColor(SDL_Color new_color) {
        this->placeholder.SetTextColor(new_color);
        return this;
    }
    TextBox* SetText(std::string new_text) {
        this->text.SetText(std::move(new_text));
        this->update_text = true;
        return this;
    }
    TextBox* SetPlaceholder(std::string new_text) {
        this->placeholder.SetText(std::move(new_text));
        this->update_placeholder = true;
        return this;
    }
    TextBox* SetCallback(Callback callback) {
        this->callback = callback;
        return this;
    }
    TextBox* SetTextAlign(SimpleAlign horizontal, SimpleAlign vertical) {
        this->text_align_horizontally = horizontal;
        this->text_align_vertically = vertical;
        return this;
    }

    // Ticking
    void HandleEvent(SDL_Event& event, EventContext& event_summary) override;
    void PostEvent() override;
    void Render() override;
    void PostRefresh() override;

};