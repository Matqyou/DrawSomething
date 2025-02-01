//
// Created by Matq on 10/01/2025.
//

#pragma once

#include <functional>
#include <utility>
#include <chrono>
#include <queue>
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

enum class PlaybackMode {
    DO_NOTHING,
    RECORD,
    REPLAY,
};

class Canvas : public Element {
private:
    bool instructions_intro;
    CanvasMode canvas_mode;
    Vec2i resolution;
    SDL_FRect canvas_source, canvas_rect;
    TextureData* canvas;

    TextureData* text_guess;
    TextureData* text_watch;
    TextureData* text_draw;
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

    PlaybackMode canvas_playback;
    std::chrono::steady_clock::time_point playback_start;
    // recording_data = time, segment, color, thickness
    std::vector<std::tuple<long long, std::pair<Vec2f, Vec2f>, SDL_Color, float>> recording_data;
    std::vector<std::tuple<long long, std::pair<Vec2f, Vec2f>, SDL_Color, float>> replay_data;
    std::vector<std::tuple<long long, std::pair<Vec2f, Vec2f>, SDL_Color, float>>::iterator replay_iterator;
    size_t last_recording_size;

    void EmplaceRecordingData(const Vec2f& segment_start, const Vec2f& segment_end);
    void CopyRecordingToReplay(long long max_idle_milliseconds);

    static LinkTexture sTextureGuess;
    static LinkTexture sTextureWatch;
    static LinkTexture sTextureDraw;

public:
    Canvas(const Vec2i& pos, const Vec2i& size);

    // Getting
    [[nodiscard]] TextureData* GetCanvasTexture() const { return canvas; }

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
    Canvas* SetPlaybackMode(PlaybackMode mode) {
        this->canvas_playback = mode;
        if (mode == PlaybackMode::RECORD) {
            this->playback_start = std::chrono::steady_clock::now();
            this->recording_data.clear();
        } else if (mode == PlaybackMode::REPLAY) {
            if (!this->recording_data.empty()) {
                this->playback_start = std::chrono::steady_clock::now();
                CopyRecordingToReplay(300);
                this->replay_iterator = replay_data.begin();
            } else { this->canvas_playback = PlaybackMode::DO_NOTHING; }
        }
        return this;
    }

    // Manipulation
    void LoadExample();
    void ClearCanvas();
    void SetTool(DrawTool tool);
    void SetDrawColor(SDL_Color color);
    void SetCurrentToolSize(float new_size);

    // Ticking
    void Tick() override;
    void HandleEvent(SDL_Event& event, EventContext& event_summary) override;
    void Render() override;
    void PostRefresh() override;

};
}
