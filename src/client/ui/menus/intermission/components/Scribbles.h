//
// Created by Matq on 20/12/2024.
//

#pragma once

#include "../../../../../shared/core/Vec2.h"
#include "../../../../core/app/Drawing.h"
#include "../../../components/element/Frame.h"

namespace Intermission {
class Scribbles : public Frame {
private:
    TextureData* generated;

    std::vector<Vec2f> path;
    float path_length, path_progress;
    Vec2f last_pen, pen;
    float thickness;
    float speed;
    SDL_FColor draw_color;

    std::vector<Vec2f>::iterator segment_iterator, segment_iterator_after;
    float segment_length, segment_progress;
    Vec2f segment_start, segment_direction;

    bool playing;

public:
    Scribbles(const Vec2i& pos, const Vec2i& size, int extra);
    ~Scribbles() override;

    // Getting
    [[nodiscard]] Vec2f GetGlobalPen() const { return Vec2f(pos + visual_offset) + pen; }
    [[nodiscard]] bool IsPlaying() const { return playing; }

    // Options
    Scribbles* SetAlpha(int new_alpha) {
        generated->SetAlphaMod(new_alpha);
        return this;
    }

    // Manipulating
    Scribbles* GenerateZigZag(float minimum,
                              float minimum_range,
                              float range_coefficient,
                              float thickness,
                              float speed,
                              SDL_FColor color);

    // Ticking
    void Tick() override;
    void Render() override;
    void RenderDebug() override;
//    void Draw();
//    void DrawDebug();

};
}
