//
// Created by Matq on 10/01/2025.
//

#include "Canvas.h"
#include "../../core/Application.h"

PreloadTexture Canvas::sTextureTapToGuess("game.canvas.guess");
PreloadTexture Canvas::sTextureTapToWatch("game.canvas.watch");
PreloadTexture Canvas::sTextureTapToDraw("game.canvas.draw");
LinkFont Canvas::sFontInstructions("fredoka.biggest");

void Canvas::init_intro() {
    auto drawing = Application::Get()->GetDrawing();

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

    // Watch text
    {
        SDL_Surface* sdl_surface =
            TTF_RenderText_Blended(sFontInstructions.GetFont()->TTFFont(),
                                   "TAP TO WATCH!",
                                   strlen("TAP TO WATCH!"),
                                   { 136, 136, 136, 255 });
        this->text_tap_to_watch = Assets::Get()->TextureFromSurface(sdl_surface);
        SDL_DestroySurface(sdl_surface);
    }

    // Draw text
    {
        SDL_Surface* sdl_surface =
            TTF_RenderText_Blended(sFontInstructions.GetFont()->TTFFont(),
                                   "TAP TO DRAW!",
                                   strlen("TAP TO DRAW!"),
                                   { 136, 136, 136, 255 });
        this->text_tap_to_draw = Assets::Get()->TextureFromSurface(sdl_surface);
        SDL_DestroySurface(sdl_surface);
    }

    // Instructions image scale
    scale_texture_tap_to_guess =
        Vec2i(Rectangles::ScaleToBounds(sTextureTapToGuess.GetTexture()->GetSize(), Vec2f(400.0f, 400.0f)));
    scale_texture_tap_to_watch =
        Vec2i(Rectangles::ScaleToBounds(sTextureTapToWatch.GetTexture()->GetSize(), Vec2f(300.0f, 300.0f)));
    scale_texture_tap_to_draw =
        Vec2i(Rectangles::ScaleToBounds(sTextureTapToDraw.GetTexture()->GetSize(), Vec2f(400.0f, 400.0f)));
    after_intro_callback = [] { };
    resolution = Vec2i(950, 550);
    canvas = new Texture(SDL_CreateTexture(drawing->Renderer(),
                                           SDL_PIXELFORMAT_RGBA8888,
                                           SDL_TEXTUREACCESS_TARGET,
                                           resolution.x,
                                           resolution.y));
    drawing->SetRenderTarget(canvas);
    drawing->SetColor(255, 255, 255, 255);
    drawing->Clear();
    drawing->SetRenderTarget(nullptr);
}

Canvas::Canvas(const Vec2i& pos, const Vec2i& size)
    : Element(ELEMENT_CUSTOM, pos, size, DRAW_RECT) {
    instructions_intro = true;
    intro_type = INTRO_GUESS;
    canvas = nullptr;
    init_intro();
}

Canvas::Canvas(const Vec2i& pos, const Vec2i& size, ElementDraw draw)
    : Element(ELEMENT_CUSTOM, pos, size, draw) {
    instructions_intro = true;
    intro_type = INTRO_GUESS;
    canvas = nullptr;
    init_intro();
}

Canvas::Canvas(const Vec2i& pos, const Vec2i& size, const Vec2i& visual, const Vec2i& offset)
    : Element(ELEMENT_CUSTOM, pos, size, visual, offset) {
    instructions_intro = true;
    intro_type = INTRO_GUESS;
    canvas = nullptr;
    init_intro();
}

Canvas::Canvas(const Vec2i& pos, const Vec2i& size, const Vec2i& visual, const Vec2i& offset, Texture* texture)
    : Element(ELEMENT_CUSTOM, pos, size, visual, offset, texture) {
    instructions_intro = true;
    intro_type = INTRO_GUESS;
    canvas = nullptr;
    init_intro();
}

Canvas::Canvas(const Vec2i& pos, const Vec2i& size, Texture* texture)
    : Element(ELEMENT_CUSTOM, pos, size, size, Vec2i(0, 0), texture) {
    instructions_intro = true;
    intro_type = INTRO_GUESS;
    canvas = nullptr;
    init_intro();
}

void Canvas::HandleEvent(SDL_Event& event, EventContext& event_summary) {
    switch (event.type) {
        case SDL_EVENT_MOUSE_BUTTON_DOWN: {
            if (instructions_intro && event.button.button == SDL_BUTTON_LEFT
                && PointCollides(event.button.x, event.button.y)) {
                instructions_intro = false;
                after_intro_callback();
            }
        }
    }
}

void Canvas::Render() const {
    auto drawing = Application::Get()->GetDrawing();

    // Draw canvas background
    auto visible_area = ClampMax(size, resolution);
    auto centered = Rectangles::CenterRelative(visible_area, size);
    auto source = Rectangles::CenterRelative(visible_area, resolution);

    SDL_FRect draw_rect = {
        (float)pos.x + centered.x,
        (float)pos.y + centered.y,
        (float)visible_area.x,
        (float)visible_area.y,
    };
    drawing->RenderTexture(canvas->SDLTexture(), &source, draw_rect);

    // Draw instructions
    if (!instructions_intro)
        return;

    Vec2i intro_texture_scale;
    Texture* intro_texture = nullptr;
    Texture* text_texture = nullptr;
    switch (intro_type) {
        case INTRO_GUESS: {
            intro_texture_scale = scale_texture_tap_to_guess;
            intro_texture = sTextureTapToGuess.GetTexture();
            text_texture = text_tap_to_guess;
            break;
        }
        case INTRO_WATCH: {
            intro_texture_scale = scale_texture_tap_to_watch;
            intro_texture = sTextureTapToWatch.GetTexture();
            text_texture = text_tap_to_watch;
            break;
        }
        case INTRO_DRAW: {
            intro_texture_scale = scale_texture_tap_to_draw;
            intro_texture = sTextureTapToDraw.GetTexture();
            text_texture = text_tap_to_draw;
            break;
        }
        default:throw std::runtime_error("Canvas.cpp Render()");
    }

    const float gap = 5.0f;

    float canvas_midpoint = (float)pos.x + (float)size.x / 2.0f;
    float centered_text_x = canvas_midpoint - text_texture->GetWidth() / 2.0f;
    float centered_instructions_x = canvas_midpoint - (float)intro_texture_scale.x / 2.0f;

    float total_height = text_texture->GetHeight() + gap + (float)intro_texture_scale.y;
    float text_y = (float)pos.y + (float)size.y / 2.0f - total_height / 2.0f;
    float instructions_y = text_y + text_texture->GetHeight() + gap;

    SDL_FRect text_rect = {
        centered_text_x,
        text_y,
        text_texture->GetWidth(),
        text_texture->GetHeight(),
    };
    drawing->RenderTexture(text_texture->SDLTexture(), nullptr, text_rect);

    SDL_FRect instructions_rect = {
        centered_instructions_x,
        instructions_y,
        (float)intro_texture_scale.x,
        (float)intro_texture_scale.y,
    };
    drawing->RenderTexture(intro_texture->SDLTexture(), nullptr, instructions_rect);
}
