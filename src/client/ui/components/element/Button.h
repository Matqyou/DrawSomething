//
// Created by Matq on 16/12/2024.
//

#pragma once

#include <utility>

#include "base/Element.h"

class Button : public Element {
protected:
    using Callback = std::function<void()>;
    Callback callback;
    bool clickable;

    SDL_Texture* sdl_pressed_texture;
    VisualTexture pressed_visual_texture;
    bool pressed_down;

public:
    Button(const Vec2i& pos, const Vec2i& size, ElementDraw draw);
    Button(const Vec2i& pos, const Vec2i& size, const VisualTexture& texture, const VisualTexture& pressed_texture);

    // Options
    Button* SetCallback(Callback callback) {
        this->callback = std::move(callback);
        return this;
    }
    Button* SetClickable(bool new_clickable) {
        this->clickable = new_clickable;
        return this;
    }
    Button* SetPressedVisualTexture(const VisualTexture& visual_texture) {
        this->pressed_visual_texture = visual_texture;
        return this;
    }

    // Manipulating
    void RunCallback() const;

    // Ticking
    void PostRefresh() override;
    void HandleEvent(SDL_Event& event, EventContext& event_summary) override;
    void Render() override;
    void UpdatePressedVisualTexture();

};