//
// Created by Matq on 10/01/2025.
//

#include "Canvas.h"
#include "../../core/Application.h"
#include "../cursors/Cursors.h"

auto static sCallbackScaleNearest = [](Texture* texture) {
    SDL_SetTextureScaleMode(texture->SDLTexture(), SDL_SCALEMODE_NEAREST);
};

PreloadTexture Canvas::sTextureGuess("game.canvas.guess", sCallbackScaleNearest);
PreloadTexture Canvas::sTextureWatch("game.canvas.watch", sCallbackScaleNearest);
PreloadTexture Canvas::sTextureDraw("game.canvas.draw", sCallbackScaleNearest);
PreloadTexture sTextureDrawing("drawings.example", sCallbackScaleNearest);

LinkFont Canvas::sFontInstructions("fredoka.biggest");

Canvas::Canvas(const Vec2i& pos, const Vec2i& size)
    : Element(ELEMENT_CUSTOM, pos, size, DONT_DRAW) {
    instructions_intro = true;
    canvas_mode = CANVAS_GUESS;
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
    canvas->SetScaleMode(SDL_SCALEMODE_NEAREST);
    drawing->SetRenderTarget(canvas);
    drawing->SetColor(255, 255, 255, 255);
    drawing->Clear();
    drawing->SetRenderTarget(nullptr);

    this->painting = false;
    this->last_paint_position = Vec2f(0, 0);
    this->tool = TOOL_PENCIL;
    this->draw_color = { 0, 0, 0, 255 };
    this->brush_size = 5.0f;
    this->eraser_size = 5.0f;
    this->custom_cursor = nullptr;
}

void Canvas::LoadExample() {
    auto drawing = Application::Get()->GetDrawing();
    drawing->SetRenderTarget(canvas);
    drawing->RenderTextureFullscreen(sTextureDrawing.GetTexture()->SDLTexture(), nullptr);
    drawing->SetRenderTarget(nullptr);
}

void Canvas::ClearCanvas() {
    auto drawing = Application::Get()->GetDrawing();
    drawing->SetRenderTarget(canvas);
    drawing->SetColor(255, 255, 255, 255);
    drawing->Clear();
    drawing->SetRenderTarget(nullptr);

    paint_segments.clear();
}

void Canvas::SetTool(DrawTool tool) {
    this->tool = tool;
}

void Canvas::SetDrawColor(SDL_FColor color) {
    this->draw_color = color;
}

void Canvas::SetBrushSize(float brush_size) {
    this->brush_size = brush_size;
}

void Canvas::SetEraserSize(float eraser_size) {
    this->eraser_size = eraser_size;
}

void Canvas::Tick() {
    if (canvas_mode != CANVAS_DRAW)
        return;

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
                    painting = true;
                    last_paint_position = Vec2f(event.button.x, event.button.y);
                    paint_segments.emplace_back(last_paint_position, last_paint_position);
                }
            }
            break;
        }
        case SDL_EVENT_MOUSE_MOTION: {
            if (custom_cursor != nullptr && event_summary.cursor_changed == CursorChange::NO_CHANGE &&
                Rectangles::PointCollides(event.motion.x, event.motion.y,
                                          canvas_rect.x, canvas_rect.y,
                                          canvas_rect.x + canvas_rect.w,
                                          canvas_rect.y + canvas_rect.h)) {
                if (canvas_mode == CANVAS_DRAW && !instructions_intro) {
                    event_summary.cursor_changed = CursorChange::TO_CUSTOM;
                    SDL_SetCursor(custom_cursor);
                }
            }

            if (painting) {
                auto mouse_position = Vec2f(event.motion.x, event.motion.y);
                paint_segments.emplace_back(last_paint_position, mouse_position);
                last_paint_position = mouse_position;
            }
            break;
        }
        case SDL_EVENT_MOUSE_BUTTON_UP: {
            if (event.button.button == SDL_BUTTON_LEFT)
                painting = false;
            break;
        }
    }
}

void Canvas::Render() {
    auto drawing = Application::Get()->GetDrawing();

    drawing->RenderTexture(canvas->SDLTexture(), &canvas_source, canvas_rect);

    if (canvas_mode == CANVAS_DRAW && tool != TOOL_NONE) {
        drawing->SetRenderTarget(canvas);
        auto draw_offset = Vec2f(canvas_rect.x - canvas_source.x, canvas_rect.y - canvas_source.y);
        SDL_FColor paint_color = tool == TOOL_PENCIL ? this->draw_color : SDL_FColor(255, 255, 255, 255);

        for (const auto& segment : paint_segments) {
            auto [start, end] = segment;
            drawing->DrawLine(start - draw_offset,
                              end - draw_offset,
                              tool == TOOL_PENCIL ? brush_size : eraser_size,
                              paint_color);
        }
        paint_segments.clear();
        drawing->SetRenderTarget(nullptr);
    }

    // Draw instructions
    if (!instructions_intro)
        return;

    Vec2i intro_texture_scale;
    Texture* intro_texture = nullptr;
    Texture* text_texture = nullptr;
    switch (canvas_mode) {
        case CANVAS_GUESS: {
            intro_texture_scale = scale_guess;
            intro_texture = sTextureGuess.GetTexture();
            text_texture = text_guess;
            break;
        }
        case CANVAS_WATCH: {
            intro_texture_scale = scale_watch;
            intro_texture = sTextureWatch.GetTexture();
            text_texture = text_watch;
            break;
        }
        case CANVAS_DRAW: {
            intro_texture_scale = scale_draw;
            intro_texture = sTextureDraw.GetTexture();
            text_texture = text_draw;
            break;
        }
        default: throw std::runtime_error("Canvas.cpp Render()");
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
}
