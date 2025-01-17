//
// Created by Matq on 16/12/2024.
//

#include "Button.h"
#include "../../cursors/Cursors.h"

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
        case SDL_EVENT_MOUSE_MOTION: {
            if (event_summary.cursor_changed == CursorChange::NO_CHANGE &&
                PointCollides(event.motion.x, event.motion.y)) {
                event_summary.cursor_changed = CursorChange::TO_POINTER;
                SDL_SetCursor(Cursors::sCursorSystemPointer);
//                SDL_SetCursor(SDL_CreateSystemCursor(SDL_SYSTEM_CURSOR_POINTER));
                break;
            }

            break;
        }
        case SDL_EVENT_MOUSE_BUTTON_DOWN: {
            if (
                PointCollides(event.button.x, event.button.y)) {
//                SetFocus(this);
                RunCallback();
                break;
            }

            break;
        }
    }

    HandleEventChildren(event, event_summary);
}
