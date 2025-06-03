//
// Created by Matq on 16/12/2024.
//

#include "Button.h"
#include "ui/cursors/Cursors.h"

Button::Button()
    : Element(), pressed_texture_instance(nullptr) {
    this->name = L"Button";
    this->callback = nullptr;
    this->clickable = true;
    this->pressed_down = false;

    this->SetDraw(DRAW_RECT);
}

Button::Button(Texture* texture, Texture* pressed_texture)
    : Element(), pressed_texture_instance(pressed_texture) {
    this->name = L"Button";
    this->callback = nullptr;
    this->clickable = true;
    this->pressed_down = false;

    this->SetDraw(DRAW_TEXTURE);
    this->SetTexture(texture);
	this->SetPressedTexture(pressed_texture);
}

void Button::RunCallback() const {
    if (callback != nullptr)
        callback();
}

void Button::PostRefresh() {
    if (draw == DRAW_TEXTURE) {
        UpdateTexturePlacement();
        UpdatePressedVisualTexture();
    }
}

void Button::HandleEvent(const SDL_Event& sdl_event, EventContext& event_summary) {
    HandleEventChildren(sdl_event, event_summary);

    if (!event_summary.rapid_context.event_captured && clickable) { // For the buttons it's pretty easy
        switch (sdl_event.type) {
            case SDL_EVENT_MOUSE_MOTION: {
                if (event_summary.cursor_changed == CursorChange::NO_CHANGE &&
                    PointCollides((int)sdl_event.motion.x, (int)sdl_event.motion.y)) {
                    event_summary.cursor_changed = CursorChange::TO_POINTER;
                    SDL_SetCursor(Cursors::sCursorSystemPointer);
                    break;
                }

                break;
            }
            case SDL_EVENT_MOUSE_BUTTON_DOWN: {
                if (sdl_event.button.button == SDL_BUTTON_LEFT &&
                    !event_summary.rapid_context.event_captured && PointCollides((int)sdl_event.button.x, (int)sdl_event.button.y)) {
                    event_summary.rapid_context.event_captured = true;
                    RunCallback();
                    pressed_down = true;
                }

                break;
            }
            case SDL_EVENT_MOUSE_BUTTON_UP: {
                if (sdl_event.button.button == SDL_BUTTON_LEFT)
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
            SDL_Texture* pressed_sdl_texture = nullptr;
            if (pressed_texture_instance.GetTexture() != nullptr)
                pressed_sdl_texture = pressed_texture_instance.GetTexture()->SDLTexture();

            if (pressed_down && pressed_sdl_texture != nullptr) {
                drawing->RenderTexture(pressed_texture_instance.GetTexture()->SDLTexture(),
                                       nullptr, pressed_texture_instance.GetResultingFRect());
            } else {
                drawing->RenderTexture(texture_instance.GetTexture()->SDLTexture(),
                                       nullptr, texture_instance.GetResultingFRect());
            }
        }
    }

    RenderChildren();
}

void Button::UpdatePressedVisualTexture() {
    pressed_texture_instance.UpdateWithNewPlacement(GetRect());
}
