//
// Created by Matq on 17/02/2025.
//

#include "ViewportItem.h"
#include "Viewport.h"

namespace Editor {
ViewportItem::ViewportItem(Viewport* viewport)
    : Frame() {
    this->SetName("ViewportItem");

    this->viewport = viewport;
    this->populated = false;
    this->dragging_from = Vec2i(0, 0);
    this->dragging_to = Vec2i(0, 0);
    this->dragging = false;
}

ViewportItem::~ViewportItem() {

}

Rect4i ViewportItem::GetSelection() const {
    auto selected_rect = Rectangles::PointsToRect(dragging_from, dragging_to);

    return selected_rect;
}

Rect4i ViewportItem::GetVisualSelection() const {
    auto selected_rect = Rectangles::PointsToRect(dragging_from, dragging_to);
    selected_rect.w++;
    selected_rect.h++;

    return selected_rect;
}

void ViewportItem::SetSelection(const Rect4i& selection) {
    this->dragging_from = Vec2i(selection.x, selection.y);
    this->dragging_to = dragging_from + Vec2i(selection.w, selection.h);
}

void ViewportItem::SetViewportTexture(Texture* texture) {
    populated = texture != nullptr;

    SetTexture(texture);
    ResizeToTexture();
}

void ViewportItem::Tick(double elapsed_seconds) {
    if (dragging) {
        Vec2f mouse;
        SDL_GetMouseState(&mouse.x, &mouse.y);

        auto relative_mouse = Vec2f(mouse.x - (float)pos.x, mouse.y - (float)pos.y);
        auto original = Vec2i(texture_instance.GetTexture()->GetOriginalSize());
        auto visual_rect = GetRect();
        auto downscale_by = Vec2f((float)original.x / visual_rect.w, (float)original.y / visual_rect.h);

        dragging_to = Vec2i((int)(relative_mouse.x * downscale_by.x), (int)(relative_mouse.y * downscale_by.y));
        if (dragging_to.x < 0) dragging_to.x = 0;
        else if (dragging_to.x >= original.x) dragging_to.x = original.x - 1;
        if (dragging_to.y < 0) dragging_to.y = 0;
        else if (dragging_to.y >= original.y) dragging_to.y = original.y - 1;

        viewport->UpdateSelectionSizeText();
    }
}

void ViewportItem::HandleEvent(const SDL_Event& sdl_event, EventContext& event_summary) {
    switch (sdl_event.type) {
        case SDL_EVENT_MOUSE_MOTION: {
            if (PointCollides((int)sdl_event.motion.x, (int)sdl_event.motion.y))
                event_summary.rapid_context.event_captured = true;
            break;
        }
        case SDL_EVENT_MOUSE_BUTTON_DOWN: {
            if (!viewport->VisualEditing())
                break;

            if (sdl_event.button.button == SDL_BUTTON_LEFT &&
                !event_summary.rapid_context.event_captured &&
                PointCollides((int)sdl_event.button.x, (int)sdl_event.button.y)) {
                event_summary.rapid_context.event_captured = true;

                auto relative_mouse = Vec2f(sdl_event.button.x - (float)pos.x, sdl_event.button.y - (float)pos.y);
                auto original = Vec2i(texture_instance.GetTexture()->GetOriginalSize());
                auto visual_rect = GetRect();
                auto downscale_by = Vec2f((float)original.x / visual_rect.w, (float)original.y / visual_rect.h);

                dragging_from =
                    Vec2i((int)(relative_mouse.x * downscale_by.x), (int)(relative_mouse.y * downscale_by.y));

                dragging_to = dragging_from;
                dragging = true;
            }
            break;
        }
        case SDL_EVENT_MOUSE_BUTTON_UP: {
            if (sdl_event.button.button == SDL_BUTTON_LEFT)
                dragging = false;
            break;
        }
    }
}

void ViewportItem::Render() {
    if (!populated) return;
    auto drawing = Assets::Get()->GetDrawing();

    drawing->RenderTexture(texture_instance.GetTexture()->SDLTexture(),
                           nullptr, GetRect());

    drawing->SetColor(150, 150, 150, 128);
    auto original = Vec2i(texture_instance.GetTexture()->GetOriginalSize());
    auto visual_rect = GetRect();
    auto ratio = Vec2f(visual_rect.w / (float)original.x, visual_rect.h / (float)original.y);

    if ((int)visual_rect.w > original.x && (int)visual_rect.h > original.y) {
        for (int y = 0; y < original.y; y++) {
            auto translate_y = (int)((float)y * ratio.y);
            auto start = Vec2f(pos + Vec2i(0, translate_y));
            auto end = Vec2f(pos + Vec2i((int)visual_rect.w, translate_y));
            drawing->DrawLine(start, end);
        }
        for (int x = 0; x < original.x; x++) {
            auto translate_x = (int)((float)x * ratio.x);
            auto start = Vec2f(pos + Vec2i(translate_x, 0));
            auto end = Vec2f(pos + Vec2i(translate_x, (int)visual_rect.h));
            drawing->DrawLine(start, end);
        }
    }

    drawing->SetColor(255, 0, 0, 255);
    drawing->DrawRect(GetRect());

    if (!viewport->VisualEditing())
        return;

    auto selected_rect = Rectangles::PointsToRect(dragging_from, dragging_to);
    selected_rect.w++;
    selected_rect.h++;
    SDL_FRect dragging_rect = {
        (float)pos.x + (float)selected_rect.x * ratio.x,
        (float)pos.y + (float)selected_rect.y * ratio.y,
        (float)(selected_rect.w) * ratio.x,
        (float)(selected_rect.h) * ratio.y,
    };
    drawing->SetColor(rand() % 255, rand() % 255, rand() % 255, 255);
    drawing->DrawRect(dragging_rect);
}
}
