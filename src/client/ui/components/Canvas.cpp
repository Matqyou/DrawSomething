//
// Created by Matq on 10/01/2025.
//

#include "Canvas.h"
#include "../../core/Application.h"

PreloadTexture Canvas::sTextureTapToGuess("game.finger");
LinkFont Canvas::sFontInstructions("fredoka.biggest");

void Canvas::init_intro() {
    // Finger text
    {
        SDL_Surface* sdl_surface =
            TTF_RenderText_Blended(sFontInstructions.GetFont()->TTFFont(),
                                   "TAP TO GUESS.",
                                   strlen("TAP TO GUESS."),
                                   { 136, 136, 136, 255 });
        this->text_tap_to_guess = Assets::Get()->TextureFromSurface(sdl_surface);
        SDL_DestroySurface(sdl_surface);
    }

    // Instructions image scale
    scale_texture_tap_to_guess =
        Vec2i(Rectangles::ScaleToBounds(sTextureTapToGuess.GetTexture()->GetSize(), Vec2f(400.0f, 400.0f)));
}

Canvas::Canvas(const Vec2i& pos, const Vec2i& size)
    : Element(ELEMENT_CUSTOM, pos, size, DRAW_RECT) {
    instructions_intro = true;
    text_tap_to_guess = nullptr;
    init_intro();
}

Canvas::Canvas(const Vec2i& pos, const Vec2i& size, ElementDraw draw)
    : Element(ELEMENT_CUSTOM, pos, size, draw) {
    instructions_intro = true;
    text_tap_to_guess = nullptr;
    init_intro();
}

Canvas::Canvas(const Vec2i& pos, const Vec2i& size, const Vec2i& visual, const Vec2i& offset)
    : Element(ELEMENT_CUSTOM, pos, size, visual, offset) {
    instructions_intro = true;
    text_tap_to_guess = nullptr;
    init_intro();
}

Canvas::Canvas(const Vec2i& pos, const Vec2i& size, const Vec2i& visual, const Vec2i& offset, Texture* texture)
    : Element(ELEMENT_CUSTOM, pos, size, visual, offset, texture) {
    instructions_intro = true;
    text_tap_to_guess = nullptr;
    init_intro();
}

Canvas::Canvas(const Vec2i& pos, const Vec2i& size, Texture* texture)
    : Element(ELEMENT_CUSTOM, pos, size, size, Vec2i(0, 0), texture) {
    instructions_intro = true;
    text_tap_to_guess = nullptr;
    init_intro();
}

void Canvas::HandleEvent(SDL_Event& event, EventContext& event_summary) {
    switch (event.type) {
        case SDL_EVENT_MOUSE_BUTTON_DOWN: {
            if (instructions_intro && event.button.button == SDL_BUTTON_LEFT && PointCollides(event.button.x, event.button.y)) {
                instructions_intro = false;
                after_intro_callback();
            }
        }
    }
}

void Canvas::Render() const {
    auto drawing = Application::Get()->GetDrawing();

    // Draw canvas background
    drawing->SetColor(color);
    drawing->FillRect(GetRect());

    // Draw instructions
    if (!instructions_intro)
        return;

    const float gap = 5.0f;

    float canvas_midpoint = (float)pos.x + (float)size.x / 2.0f;
    float centered_text_x = canvas_midpoint - text_tap_to_guess->GetWidth() / 2.0f;
    float centered_instructions_x = canvas_midpoint - (float)scale_texture_tap_to_guess.x / 2.0f;

    float total_height = text_tap_to_guess->GetHeight() + gap + (float)scale_texture_tap_to_guess.y;
    float text_y = (float)pos.y + (float)size.y / 2.0f - total_height / 2.0f;
    float instructions_y = text_y + text_tap_to_guess->GetHeight() + gap;

    SDL_FRect text_rect = {
        centered_text_x,
        text_y,
        text_tap_to_guess->GetWidth(),
        text_tap_to_guess->GetHeight(),
    };
    drawing->RenderTexture(text_tap_to_guess->SDLTexture(), nullptr, text_rect);

    SDL_FRect instructions_rect = {
        centered_instructions_x,
        instructions_y,
        (float)scale_texture_tap_to_guess.x,
        (float)scale_texture_tap_to_guess.y,
    };
    drawing->RenderTexture(sTextureTapToGuess.GetTexture()->SDLTexture(), nullptr, instructions_rect);
}
