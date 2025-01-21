//
// Created by Matq on 10/01/2025.
//

#pragma once

#include <functional>
#include <utility>
#include "../../../../../shared/core/Colors.h"
#include "../../../components/element/base/Element.h"

namespace Ingame {
enum CanvasMode {
    CANVAS_GUESS,
    CANVAS_WATCH,
    CANVAS_DRAW,
};

enum DrawTool {
    TOOL_NONE,
    TOOL_PENCIL,
    TOOL_ERASER,
};

class Canvas : public Element {
private:
    bool instructions_intro;
    CanvasMode canvas_mode;
    Vec2i resolution;
    SDL_FRect canvas_source, canvas_rect;
    Texture* canvas;

    Texture* text_guess;
    Texture* text_watch;
    Texture* text_draw;
    Vec2i scale_guess;
    Vec2i scale_watch;
    Vec2i scale_draw;
    using Callback = std::function<void()>;
    Callback after_intro_callback;

    std::vector<std::pair<Vec2f, Vec2f>> paint_segments;
    Vec2f last_paint_position;
    bool painting;
    DrawTool tool;
    Colors::IFColor draw_color, eraser_color;
    float brush_size, eraser_size;
    SDL_Cursor* custom_cursor;

    static PreloadTexture sTextureGuess;
    static PreloadTexture sTextureWatch;
    static PreloadTexture sTextureDraw;

public:
    Canvas(const Vec2i& pos, const Vec2i& size);

    // Getting
    [[nodiscard]] Texture* GetCanvasTexture() const { return canvas; }

    // Options
    Canvas* SetCustomCursor(SDL_Cursor* custom_cursor) {
        this->custom_cursor = custom_cursor;
        return this;
    }
    Canvas* SetMode(CanvasMode canvas_mode) {
        this->canvas_mode = canvas_mode;
        this->instructions_intro = true;
        return this;
    }
    Canvas* SetCallback(Callback callback) {
        this->after_intro_callback = std::move(callback);
        return this;
    }

    // Manipulation
    void LoadExample();
    void ClearCanvas();
    void SetTool(DrawTool tool);
    void SetDrawColor(SDL_Color color);
    void SetBrushSize(float brush_size);
    void SetEraserSize(float eraser_size);

    // Ticking
    void Tick() override;
    void HandleEvent(SDL_Event& event, EventContext& event_summary) override;
    void Render() override;
    void PostRefresh() override;

};
}
