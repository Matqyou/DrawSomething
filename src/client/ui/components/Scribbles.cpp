//
// Created by Matq on 20/12/2024.
//

#include "Scribbles.h"
#include "../../core/Application.h"

Scribbles::Scribbles(const Vec2d& pos, const Vec2i& size, int extra) {
    this->pos = pos;
    this->size = size;
    this->full_size = size + extra;
    this->extra = extra;
    this->offset = extra / 2.0;

    auto drawing = Application::Get()->GetDrawing();
    auto new_texture = new Texture(SDL_CreateTexture(drawing->Renderer(),
                                                     SDL_PIXELFORMAT_RGBA8888,
                                                     SDL_TEXTUREACCESS_TARGET,
                                                     this->full_size.x,
                                                     this->full_size.y));
    new_texture->SetBlendMode(SDL_BLENDMODE_BLEND);
    new_texture->SetAlphaMod(150);

    this->texture = new_texture;
    this->path_length = 0.0;
    this->path_progress = 0.0;
    this->last_pen = Vec2d(0.0, 0.0);
    this->pen = Vec2d(0.0, 0.0);
    this->segment_length = 0.0;
    this->segment_progress = 0.0;
    this->playing = false;
}

Scribbles::~Scribbles() {
    delete texture;
}

void Scribbles::GenerateZigZag() {
    auto drawing = Application::Get()->GetDrawing();
    drawing->SetRenderTarget(texture);
    drawing->SetColor(0, 0, 0, 0);
    drawing->Clear();
    drawing->SetRenderTarget(nullptr);

    path.clear();
    path_length = 0.0;
    path_progress = 0.0;

    auto current_pos = Vec2d(40.0, 0.0);
    double radians = (130.0 + rand() % 20) / 180.0 * M_PI;

    path.push_back(current_pos + offset);
    bool side = true;
    for (int j = 0; j < 200; j++) {
        auto direction = AngleVec2d(radians);

        Vec2d end_position;
        for (int i = 1; i < 2500.0; i++) {
            end_position = current_pos + direction * i;

            if (end_position.x < 0 || end_position.x > size.x ||
                end_position.y < 0 || end_position.y > size.y) {
                if (end_position.x < 0) end_position.x = 0;
                else if (end_position.x > size.x) end_position.x = size.x;
                if (end_position.y < 0) end_position.y = 0;
                else if (end_position.y > size.y) end_position.y = size.y;
                break;
            }
        }

        path.push_back(end_position + offset);
        path_length += DistanceVec2(current_pos, end_position);
        current_pos = end_position;

        if ((end_position - Vec2d(size.x, size.y)).Length() <= 30.0)
            break;

        double corners = std::min(DistanceVec2(Vec2d(size.x, 0), current_pos),
                                  DistanceVec2(Vec2d(0, size.y), current_pos));
        auto range = 25 + (int)(corners / 200.0 * 80.0); // preferrably 2.5 +
        radians += M_PI + (double)(3 + (rand() % range) / 10.0) / 180.0 * M_PI * (side ? 1.0 : -1.0);
        side = !side;
    }

    segment_iterator = path.begin();
    segment_iterator_after = segment_iterator + 1;
    segment_length = DistanceVec2(*segment_iterator, *segment_iterator_after);
    segment_progress = 0.0;
    segment_start = *segment_iterator;
    segment_direction = (*segment_iterator_after - *segment_iterator).Normalize();
    last_pen = *segment_iterator;
    pen = *segment_iterator;
    playing = true;
}

void Scribbles::Tick() {
    path_progress += 30.0;
    segment_progress += 30.0;

    while (playing && segment_progress > segment_length) {
        segment_progress -= segment_length;
        segment_iterator++;
        segment_iterator_after++;

        segment_start = *segment_iterator;
        if (segment_iterator_after == path.end()) {
            segment_direction = Vec2d(0.0, 0.0);
            playing = false;
            break;
        }

        segment_length = DistanceVec2(*segment_iterator, *segment_iterator_after);
        segment_direction = (*segment_iterator_after - *segment_iterator).Normalize();
    }

    last_pen = pen;
    pen = segment_start + segment_direction * segment_progress;
}

void Scribbles::Draw() {
    auto drawing = Application::Get()->GetDrawing();

    if (playing) {
        drawing->SetRenderTarget(texture);
        drawing->DrawLine(Vec2f(last_pen.x, last_pen.y), Vec2f(pen.x, pen.y), 20, { 0, 0, 0, 255 });
        drawing->SetRenderTarget(nullptr);
    }

    drawing->RenderTexture(texture->SDLTexture(), nullptr, { (float)pos.x, (float)pos.y, (float)full_size.x, (float)full_size.y });
}

void Scribbles::DrawDebug() {
    auto drawing = Application::Get()->GetDrawing();
    drawing->SetColor(255, 0, 0, 255);
    drawing->DrawRect({ (float)pos.x, (float)pos.y, (float)full_size.x, (float)full_size.y });

    Vec2d previous_node;
    bool not_first = false;
    for (const auto& node : path) {
        if (not_first) {
            drawing->DrawLine(Vec2f(previous_node + pos), Vec2f(node + pos));
        } else { not_first = true; }
        previous_node = node;
    }

    drawing->SetColor(0, 255, 0, 255);
    drawing->FillRect({ (float)(pen.x - 3 + pos.x), (float)(pen.y - 3 + pos.y), 6, 6 });
}