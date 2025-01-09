//
// Created by Matq on 20/12/2024.
//

#pragma once

#include "../../core/app/Drawing.h"
#include "../../../shared/core/Vec2.h"

class Scribbles {
private:
    Texture* texture;
    Vec2d pos;
    Vec2i full_size, size;
    double extra, offset;

    std::vector<Vec2d> path;
    double path_length, path_progress;
    Vec2d last_pen, pen;

    std::vector<Vec2d>::iterator segment_iterator, segment_iterator_after;
    double segment_length, segment_progress;
    Vec2d segment_start, segment_direction;

    bool playing;

public:
    Scribbles(const Vec2d& pos, const Vec2i& size, int extra);
    ~Scribbles();

    // Getting
    [[nodiscard]] Vec2d GetGlobalPen() const { return pos + pen; }
    [[nodiscard]] bool IsPlaying() const { return playing; }

    // Manipulating
    void GenerateZigZag();

    // Ticking
    void Tick();
    void Draw();
    void DrawDebug();

};
