//
// Created by Matq on 10/01/2025.
//

#pragma once

#include <functional>
#include <utility>
#include "element/base/Element.h"

class Canvas : public Element {
private:
    bool instructions_intro;

    Texture* text_tap_to_guess;
    Vec2i scale_texture_tap_to_guess;
//    void (* after_intro_callback)();
    using Callback = std::function<void()>;
    Callback after_intro_callback;

    static PreloadTexture sTextureTapToGuess;
    static LinkFont sFontInstructions;

    void init_intro();

public:
    Canvas(const Vec2i& pos, const Vec2i& size);
    Canvas(const Vec2i& pos, const Vec2i& size, ElementDraw draw);
    Canvas(const Vec2i& pos, const Vec2i& size, const Vec2i& visual, const Vec2i& offset);
    Canvas(const Vec2i& pos, const Vec2i& size, const Vec2i& visual, const Vec2i& offset, Texture* texture);
    Canvas(const Vec2i& pos, const Vec2i& size, Texture* texture);

    // Manipulation
    Canvas* SetCallback(Callback callback) {
        this->after_intro_callback = std::move(callback);
        return this;
    }

    // Ticking
    void HandleEvent(SDL_Event& event, EventContext& event_summary) override;
    void Render() const override;
};