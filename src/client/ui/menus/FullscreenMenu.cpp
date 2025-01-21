//
// Created by Matq on 12/01/2025.
//

#include "FullscreenMenu.h"
#include "../../core/Application.h"

FullscreenMenu::FullscreenMenu()
    : Frame(Vec2i(0, 0), Vec2i(0, 0), DRAW_RECT) {

}

void FullscreenMenu::HandleEvent(SDL_Event& sdl_event, EventContext& event_summary) {
    switch (sdl_event.type) {
        case SDL_EVENT_WINDOW_RESIZED: {
            UpdateElement(Vec2i(0, 0),
                          Application::Get()->GetResolution(),
                          Application::Get()->GetResolution());

            break;
        }
    }

    HandleEventChildren(sdl_event, event_summary);
}