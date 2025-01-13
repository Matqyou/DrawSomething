//
// Created by Matq on 10/01/2025.
//

#pragma once

#include <functional>
#include <utility>
#include "element/base/Element.h"

enum CanvasIntro {
    INTRO_GUESS,
    INTRO_WATCH,
    INTRO_DRAW,
};

class Canvas : public Element {
private:
    bool instructions_intro;
    CanvasIntro intro_type;
    Vec2i resolution;
    Texture* canvas;

    Texture* text_tap_to_guess;
    Texture* text_tap_to_watch;
    Texture* text_tap_to_draw;
    Vec2i scale_texture_tap_to_guess;
    Vec2i scale_texture_tap_to_watch;
    Vec2i scale_texture_tap_to_draw;
    using Callback = std::function<void()>;
    Callback after_intro_callback;

    static PreloadTexture sTextureTapToGuess;
    static PreloadTexture sTextureTapToWatch;
    static PreloadTexture sTextureTapToDraw;
    static LinkFont sFontInstructions;

    void init_intro();

public:
    Canvas(const Vec2i& pos, const Vec2i& size);
    Canvas(const Vec2i& pos, const Vec2i& size, ElementDraw draw);
    Canvas(const Vec2i& pos, const Vec2i& size, const Vec2i& visual, const Vec2i& offset);
    Canvas(const Vec2i& pos, const Vec2i& size, const Vec2i& visual, const Vec2i& offset, Texture* texture);
    Canvas(const Vec2i& pos, const Vec2i& size, Texture* texture);

    // Options
    Canvas* SetIntro(CanvasIntro intro_type) {
        this->intro_type = intro_type;
        this->instructions_intro = true;
        return this;
    }
    Canvas* SetCallback(Callback callback) {
        this->after_intro_callback = std::move(callback);
        return this;
    }

    // Ticking
    void HandleEvent(SDL_Event& event, EventContext& event_summary) override;
    void Render() const override;
};