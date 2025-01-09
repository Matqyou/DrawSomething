//
// Created by Matq on 16/12/2024.
//

#include "Button.h"

Button::Button(const Vec2i& pos, const Vec2i& size)
    : Element(ELEMENT_BUTTON, pos, size, DRAW_RECT) {
    callback = nullptr;
}

Button::Button(const Vec2i& pos, const Vec2i& size, ElementDraw draw)
    : Element(ELEMENT_BUTTON, pos, size, draw){
    callback = nullptr;
}

Button::Button(const Vec2i& pos, const Vec2i& size, const Vec2i& visual, const Vec2i& offset)
    : Element(ELEMENT_BUTTON, pos, size, visual, offset) {
    callback = nullptr;
}

Button::Button(const Vec2i& pos, const Vec2i& size, const Vec2i& visual, const Vec2i& offset, Texture* texture)
    : Element(ELEMENT_BUTTON, pos, size, visual, offset, texture) {
    callback = nullptr;
}

Button::Button(const Vec2i& pos, const Vec2i& size, Texture* texture)
    : Element(ELEMENT_BUTTON, pos, size, size, Vec2i(0, 0), texture){
    callback = nullptr;
}

void Button::RunCallback() const {
    if (callback != nullptr)
        callback();
}

void Button::HandleEvent(SDL_Event& event, EventContext& event_summary) {
    switch (event.type) {
        case SDL_MOUSEMOTION: {
            if (event_summary.cursor_changed == CursorChange::NO_CHANGE &&
                PointCollides(event.motion.x, event.motion.y)) {
                event_summary.cursor_changed = CursorChange::TO_POINTER;
                SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_HAND));
                break;
            }

            break;
        }
        case SDL_MOUSEBUTTONDOWN: {
            if (
                PointCollides(event.button.x, event.button.y)) {
                SetFocus(this);
                RunCallback();
                break;
            }

            break;
        }
    }
}

//void Button::Render(Drawing* drawing) const {
//    drawing->SetColor(color);
//    drawing->FillRect(GetRect());
//}
