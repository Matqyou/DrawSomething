//
// Created by Matq on 12/03/2025.
//

#include "Scrollable.h"

Scrollable::Scrollable()
    : Element() {
    name = L"Scrollable";

    scroll = 0;
    scroll_direction = Orientation::VERTICAL;
}

Scrollable::~Scrollable() {

}

void Scrollable::HandleEvent(const SDL_Event& sdl_event, EventContext& event_summary) {
    HandleEventChildren(sdl_event, event_summary);

    switch (sdl_event.type) {
        case SDL_EVENT_MOUSE_WHEEL: {
            if (!event_summary.rapid_context.event_captured &&
                PointCollides((int)sdl_event.wheel.mouse_x, (int)sdl_event.wheel.mouse_y)) {
                event_summary.rapid_context.event_captured = true;

                scroll += sdl_event.wheel.y * 25;
                this->parent->Refresh();
            }

            break;
        }
    }
}

//void Scrollable::Tick() {
//    if (scroll_updated) {
//        scroll_updated = false;
//        Refresh();
//    }
//}

void Scrollable::PreComposition() {
    for (auto child : children) {
        child->composition_pos += scroll_direction == Orientation::VERTICAL ?
                                  Vec2i(0, (int)scroll) : Vec2i((int)scroll, 0);
    }
}
