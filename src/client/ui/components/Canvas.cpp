//
// Created by Matq on 10/01/2025.
//

#include "Canvas.h"
#include "../../core/Application.h"

auto static sCallbackScaleNearest = [](Texture* texture) {
    SDL_SetTextureScaleMode(texture->SDLTexture(), SDL_SCALEMODE_NEAREST);
};

PreloadTexture Canvas::sTextureGuess("game.canvas.guess", sCallbackScaleNearest);
PreloadTexture Canvas::sTextureWatch("game.canvas.watch", sCallbackScaleNearest);
PreloadTexture Canvas::sTextureDraw("game.canvas.draw", sCallbackScaleNearest);
LinkFont Canvas::sFontInstructions("fredoka.biggest");

Canvas::Canvas(const Vec2i& pos, const Vec2i& size)
    : Element(ELEMENT_CUSTOM, pos, size, DONT_DRAW) {
    instructions_intro = true;
    intro_type = INTRO_GUESS;
    canvas = nullptr;

    auto assets = Assets::Get();
    auto drawing = Application::Get()->GetDrawing();

    // Instruction texts
    auto instructions_font = sFontInstructions.GetFont()->TTFFont();
    SDL_Color instructions_font_color = { 136, 136, 136, 255 };
    const char* guess_text_ = "TAP TO GUESS.";
    const char* watch_text_ = "TAP TO WATCH!";
    const char* draw_text_ = "TAP TO DRAW!";
    text_guess = assets->RenderTextBlended(instructions_font, guess_text_, instructions_font_color);
    text_watch = assets->RenderTextBlended(instructions_font, watch_text_, instructions_font_color);
    text_draw = assets->RenderTextBlended(instructions_font, draw_text_, instructions_font_color);

    // Instructions image scale
    scale_guess = Vec2i(Rectangles::ScaleToBounds(sTextureGuess.GetTexture()->GetSize(), Vec2f(400.0f, 400.0f)));
    scale_watch = Vec2i(Rectangles::ScaleToBounds(sTextureWatch.GetTexture()->GetSize(), Vec2f(300.0f, 300.0f)));
    scale_draw = Vec2i(Rectangles::ScaleToBounds(sTextureDraw.GetTexture()->GetSize(), Vec2f(400.0f, 400.0f)));

    after_intro_callback = [] { };
    resolution = Vec2i(950, 550);
    canvas_source = { 0, 0, (float)resolution.x, (float)resolution.y };
    canvas_rect = { (float)pos.x, (float)pos.y, (float)resolution.x, (float)resolution.y };
    canvas = new Texture(SDL_CreateTexture(drawing->Renderer(),
                                           SDL_PIXELFORMAT_RGBA8888,
                                           SDL_TEXTUREACCESS_TARGET,
                                           resolution.x,
                                           resolution.y));
    drawing->SetRenderTarget(canvas);
    drawing->SetColor(255, 255, 255, 255);
    drawing->Clear();
    drawing->SetRenderTarget(nullptr);

    this->dragging = false;
    this->last_drag = Vec2f(0, 0);
    this->tool = TOOL_PENCIL;
    this->draw_color = { 0, 0, 0, 255 };
}

void Canvas::ClearCanvas() {
    auto drawing = Application::Get()->GetDrawing();
    drawing->SetRenderTarget(canvas);
    drawing->SetColor(255, 255, 255, 255);
    drawing->Clear();
    drawing->SetRenderTarget(nullptr);
}

void Canvas::SetTool(DrawTool tool) {
    this->tool = tool;
}

void Canvas::SetDrawColor(SDL_FColor color) {
    draw_color = color;
}

void Canvas::Tick() {
    last_drag = drag;

    float mouse_x, mouse_y;
    SDL_GetMouseState(&mouse_x, &mouse_y);
    drag = Vec2f(mouse_x, mouse_y);
}

void Canvas::HandleEvent(SDL_Event& event, EventContext& event_summary) {
    switch (event.type) {
        case SDL_EVENT_MOUSE_BUTTON_DOWN: {
            if (event.button.button == SDL_BUTTON_LEFT
                && PointCollides(event.button.x, event.button.y)) {
                if (instructions_intro) {
                    instructions_intro = false;
                    after_intro_callback();
                } else {
                    dragging = true;
                    drag = Vec2f(event.button.x, event.button.y);
                    last_drag = drag;
                }
            }
            break;
        }
        case SDL_EVENT_MOUSE_BUTTON_UP: {
            if (event.button.button == SDL_BUTTON_LEFT)
                dragging = false;
            break;
        }
    }
}

void Canvas::Render() {
    auto drawing = Application::Get()->GetDrawing();

    drawing->RenderTexture(canvas->SDLTexture(), &canvas_source, canvas_rect);

    if (dragging && tool != TOOL_NONE) {
        drawing->SetRenderTarget(canvas);
        auto draw_offset = Vec2f(canvas_rect.x - canvas_source.x, canvas_rect.y - canvas_source.y);
        SDL_FColor draw_color = tool == TOOL_PENCIL ? this->draw_color : SDL_FColor(255, 255, 255, 255);
        drawing->DrawLine(drag - draw_offset, last_drag - draw_offset, 10.0, draw_color);
        drawing->SetRenderTarget(nullptr);
    }

    // Draw instructions
    if (!instructions_intro)
        return;

    Vec2i intro_texture_scale;
    Texture* intro_texture = nullptr;
    Texture* text_texture = nullptr;
    switch (intro_type) {
        case INTRO_GUESS: {
            intro_texture_scale = scale_guess;
            intro_texture = sTextureGuess.GetTexture();
            text_texture = text_guess;
            break;
        }
        case INTRO_WATCH: {
            intro_texture_scale = scale_watch;
            intro_texture = sTextureWatch.GetTexture();
            text_texture = text_watch;
            break;
        }
        case INTRO_DRAW: {
            intro_texture_scale = scale_draw;
            intro_texture = sTextureDraw.GetTexture();
            text_texture = text_draw;
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

void Canvas::PostRefresh() {
    auto visible_area = ClampMax(size, resolution);
    auto centered = Rectangles::CenterRelative(visible_area, size);
    canvas_source = Rectangles::CenterRelative(visible_area, resolution);

    canvas_rect = {
        (float)pos.x + centered.x,
        (float)pos.y + centered.y,
        (float)visible_area.x,
        (float)visible_area.y,
    };
};