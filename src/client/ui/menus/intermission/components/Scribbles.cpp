//
// Created by Matq on 20/12/2024.
//

#include "Scribbles.h"
#include "../../../../core/Application.h"

namespace Intermission {
static LinkTexture sTexturePencil("pencil2");
static const Vec2f sPencilTip(222, 918);

Scribbles::Scribbles(const Vec2i& pos, const Vec2i& size, int extra)
    : Frame(pos, size, size + extra, Vec2i(-extra / 2, -extra / 2), nullptr) {
    this->name = L"Scribbles";
    this->pos = pos;
    this->size = size;
    this->thickness = 0;
    this->speed = 10;
    this->draw_color = { 0, 0, 0, 255 };

    auto drawing = Application::Get()->GetDrawing();
    this->generated = (new TextureData(SDL_CreateTexture(drawing->Renderer(),
                                                         SDL_PIXELFORMAT_RGBA8888,
                                                         SDL_TEXTUREACCESS_TARGET,
                                                         this->visual_size.x,
                                                         this->visual_size.y)))
        ->SetBlendMode(SDL_BLENDMODE_BLEND);

    this->path_length = 0.0;
    this->path_progress = 0.0;
    this->last_pen = Vec2f(0.0, 0.0);
    this->pen = Vec2f(0.0, 0.0);
    this->segment_length = 0.0;
    this->segment_progress = 0.0;
    this->playing = false;
}

Scribbles::~Scribbles() {
    delete generated;
}

Scribbles* Scribbles::GenerateZigZag(float minimum,
                                     float minimum_range,
                                     float range_coefficient,
                                     float thickness,
                                     float speed,
                                     SDL_FColor color) {
    this->thickness = thickness;
    this->speed = speed;
    this->draw_color = color;
    auto drawing = Application::Get()->GetDrawing();
    drawing->SetRenderTarget(generated);
    drawing->SetColor(0, 0, 0, 0);
    drawing->Clear();
    drawing->SetRenderTarget(nullptr);

    path.clear();
    path_length = 0.0;
    path_progress = 0.0;

    auto current_pos = Vec2f(40.0, 0.0);
    float radians = float(130 + rand() % 20) / 180.0f * (float)M_PI;

    path.push_back(current_pos - Vec2f(visual_offset));
    bool side = true;
    for (int j = 0; j < 200; j++) {
        auto direction = AngleVec2f(radians);

        Vec2f end_position;
        for (int i = 1; i < 2500.0; i++) {
            end_position = current_pos + direction * (float)i;

            if (end_position.x<0 || end_position.x>(float)size.x || end_position.y<0 || end_position.y>(float)
            size.y) {
                if (end_position.x < 0) end_position.x = 0;
                else if (end_position.x > (float)size.x) end_position.x = (float)size.x;
                if (end_position.y < 0) end_position.y = 0;
                else if (end_position.y > (float)size.y) end_position.y = (float)size.y;
                break;
            }
        }

        path.push_back(end_position - Vec2f(visual_offset));
        path_length += DistanceVec2f(current_pos, end_position);
        current_pos = end_position;

        if ((end_position - Vec2f((float)size.x, (float)size.y)).Length() <= 30.0)
            break;

        float corners = std::min(DistanceVec2f(Vec2f(size.x, 0), current_pos),
                                 DistanceVec2f(Vec2f(0, size.y), current_pos));
        auto range = (int)(minimum_range + corners / range_coefficient); // preferrably 2.5 +
        radians += M_PI + (float)(minimum + (rand() % range) / 10.0) / 180.0 * M_PI * (side ? 1.0 : -1.0);
        side = !side;
    }

    segment_iterator = path.begin();
    segment_iterator_after = segment_iterator + 1;
    segment_length = DistanceVec2f(*segment_iterator, *segment_iterator_after);
    segment_progress = 0.0;
    segment_start = *segment_iterator;
    segment_direction = (*segment_iterator_after - *segment_iterator).Normalize();
    last_pen = *segment_iterator;
    pen = *segment_iterator;
    playing = true;
    return this;
}

void Scribbles::Tick() {
    path_progress += speed;
    segment_progress += speed;

    while (playing && segment_progress > segment_length) {
        segment_progress -= segment_length;
        segment_iterator++;
        segment_iterator_after++;

        segment_start = *segment_iterator;
        if (segment_iterator_after == path.end()) {
            segment_direction = Vec2f(0.0, 0.0);
            playing = false;
            break;
        }

        segment_length = DistanceVec2f(*segment_iterator, *segment_iterator_after);
        segment_direction = (*segment_iterator_after - *segment_iterator).Normalize();
    }

    last_pen = pen;
    pen = segment_start + segment_direction * segment_progress;
}

void Scribbles::Render() {
    auto drawing = Application::Get()->GetDrawing();

    if (playing) {
        drawing->SetRenderTarget(generated);
        drawing->DrawLine(Vec2f(last_pen.x, last_pen.y), Vec2f(pen.x, pen.y), thickness, draw_color);
    }

    drawing->SetRenderTarget(nullptr);
    drawing->RenderTexture(generated->SDLTexture(), nullptr, GetVisualRect());

    if (IsPlaying()) {
        auto pen_position = GetGlobalPen() - Intermission::sPencilTip * 0.3;
        SDL_FRect pencil_rect = {
            (float)pen_position.x,
            (float)pen_position.y,
            (float)(Intermission::sTexturePencil.GetTexture()->GetWidth() * 0.3),
            (float)(Intermission::sTexturePencil.GetTexture()->GetHeight() * 0.3),
        };
        SDL_FPoint center = { (float)(Intermission::sPencilTip.x * 0.3), (float)(Intermission::sPencilTip.y * 0.3) };
        drawing->RenderTextureEx(Intermission::sTexturePencil.GetTexture()->SDLTexture(),
                                 nullptr,
                                 pencil_rect,
                                 35,
                                 &center,
                                 SDL_FLIP_NONE);
    }

    RenderChildren();
}

void Scribbles::RenderDebug() {
    auto drawing = Application::Get()->GetDrawing();
    drawing->SetColor(0, 255, 0, 255);
    drawing->DrawRect(GetVisualRect());
    drawing->SetColor(255, 0, 0, 255);
    drawing->DrawRect(GetRect());

//    Vec2f previous_node;
//    bool not_first = false;
//    for (const auto& node : path) {
//        if (not_first) {
//            drawing->DrawLine(previous_node + Vec2f(pos + visual_offset), node + Vec2f(pos + visual_offset));
//        } else { not_first = true; }
//        previous_node = node;
//    }
//
//    drawing->SetColor(0, 255, 0, 255);
//    drawing->FillRect({ (float)(pen.x - 3 + pos.x + visual_offset.x), (float)(pen.y - 3 + pos.y + visual_offset.y),
//                        6, 6 });

    RenderDebugChildren();
}
}
