//
// Created by Matq on 12/03/2025.
//

#pragma once

#include "ui/components/element/base/Element.h"

class Scrollable : public Element {
private:
    double scroll;
    Orientation scroll_direction;

public:
    Scrollable();
    ~Scrollable() override;

    // Ticking
    void HandleEvent(const SDL_Event& sdl_event, EventContext& event_summary) override;
    void PreComposition() override;

    // Options
    Scrollable* SetScrollDirection(Orientation direction) {
        scroll_direction = direction;
        return this;
    }

};
