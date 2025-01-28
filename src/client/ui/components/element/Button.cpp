//
// Created by Matq on 16/12/2024.
//

#include "Button.h"
#include "../../cursors/Cursors.h"

Button::Button(const Vec2i& pos, const Vec2i& size, ElementDraw draw)
    : Element(pos, size, draw) {
    this->name = L"Button";
    this->callback = nullptr;
    this->clickable = true;
    this->sdl_pressed_texture = nullptr;
    this->pressed_down = false;
}

Button::Button(const Vec2i& pos,
               const Vec2i& size,
               const VisualTexture& texture,
               const VisualTexture& pressed_texture)
    : Element(pos, size, texture) {
    this->name = L"Button";
    this->callback = nullptr;
    this->clickable = true;
    this->sdl_pressed_texture = nullptr;
    this->pressed_down = false;

    SetDraw(DRAW_VISUAL_TEXTURE);
    SetVisualTexture(texture);
    SetPressedVisualTexture(pressed_texture);
}

void Button::RunCallback() const {
    if (callback != nullptr)
        callback();
}

void Button::PostRefresh() {
    if (draw == DRAW_VISUAL_TEXTURE) {
        UpdateVisualTexture();
        UpdatePressedVisualTexture();
    }
}

void Button::HandleEvent(SDL_Event& event, EventContext& event_summary) {
    HandleEventChildren(event, event_summary);

    if (!event_summary.rapid_context.event_captured && clickable) { // For the buttons it's pretty easy
        switch (event.type) {
            case SDL_EVENT_MOUSE_MOTION: {
                if (event_summary.cursor_changed == CursorChange::NO_CHANGE &&
                    PointCollides((int)event.motion.x, (int)event.motion.y)) {
                    event_summary.cursor_changed = CursorChange::TO_POINTER;
                    SDL_SetCursor(Cursors::sCursorSystemPointer);
                    break;
                }

                break;
            }
            case SDL_EVENT_MOUSE_BUTTON_DOWN: {
                if (event.button.button == SDL_BUTTON_LEFT &&
                    !event_summary.rapid_context.event_captured && PointCollides((int)event.button.x, (int)event.button.y)) {
                    event_summary.rapid_context.event_captured = true;
                    RunCallback();
                    pressed_down = true;
                }

                break;
            }
            case SDL_EVENT_MOUSE_BUTTON_UP: {
                if (event.button.button == SDL_BUTTON_LEFT)
                    pressed_down = false;
                break;
            }
        }
    }
}

void Button::Render() {
    if (draw != ElementDraw::DONT_DRAW) {
        auto drawing = Assets::Get()->GetDrawing();

        if (draw == ElementDraw::DRAW_RECT) {
            auto& fill_color = has_focus ? focus_color : color;
            drawing->SetColor(fill_color);
            drawing->FillRect(GetRect());
        } else if (draw == ElementDraw::DRAW_TEXTURE) {
            if (pressed_down && sdl_pressed_texture != nullptr) {
                drawing->RenderTexture(sdl_pressed_texture, nullptr, GetVisualRect());
            } else {
                drawing->RenderTexture(sdl_texture, nullptr, GetVisualRect());
            }
        } else if (draw == ElementDraw::DRAW_VISUAL_TEXTURE) {
            if (pressed_down && pressed_visual_texture.SDLTexture() != nullptr) {
                drawing->RenderTexture(pressed_visual_texture.SDLTexture(),
                                       nullptr,
                                       pressed_visual_texture.GetVisualRect());
            } else {
                drawing->RenderTexture(visual_texture.SDLTexture(), nullptr, visual_texture.GetVisualRect());
            }
        }
    }

    RenderChildren();
}

void Button::UpdatePressedVisualTexture() {
    pressed_visual_texture.UpdateRect(GetRect());
}