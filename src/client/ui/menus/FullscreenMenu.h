//
// Created by Matq on 12/01/2025.
//

#pragma once

#include "../components/element/Frame.h"

class FullscreenMenu : public Frame {
public:
    FullscreenMenu();

    // Ticking
    void FullscreenEvent(const SDL_Event& sdl_event, EventContext& event_summary);
    void HandleEvent(const SDL_Event& sdl_event, EventContext& event_summary) override;

    // Manipulation
    void RefreshMenu();

};

