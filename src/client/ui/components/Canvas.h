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

    Texture* text_guess;
    Texture* text_watch;
    Texture* text_draw;
    Vec2i scale_guess;
    Vec2i scale_watch;
    Vec2i scale_draw;
    using Callback = std::function<void()>;
    Callback after_intro_callback;

    static PreloadTexture sTextureGuess;
    static PreloadTexture sTextureWatch;
    static PreloadTexture sTextureDraw;
    static LinkFont sFontInstructions;

public:
    Canvas(const Vec2i& pos, const Vec2i& size);

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