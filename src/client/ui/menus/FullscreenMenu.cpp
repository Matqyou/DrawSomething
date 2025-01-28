//
// Created by Matq on 12/01/2025.
//

#include "FullscreenMenu.h"
#include "../../core/Application.h"

FullscreenMenu::FullscreenMenu()
    : Frame(Vec2i(0, 0), Vec2i(0, 0), DRAW_RECT) {
    name = L"FullscreenMenu";
}

void FullscreenMenu::HandleEvent(SDL_Event& sdl_event, EventContext& event_summary) {
    HandleEventChildren(sdl_event, event_summary);

    // When multiple menus are stacked, the oldest one will prevent events from passing onto lower menus
    event_summary.rapid_context.event_captured = true;

    switch (sdl_event.type) {
        case SDL_EVENT_WINDOW_RESIZED: {
            RefreshMenu();
            break;
        }
    }
}

void FullscreenMenu::RefreshMenu() {
    UpdateElement(Vec2i(0, 0),
                  Application::Get()->GetResolution(),
                  Application::Get()->GetResolution());
}