//
// Created by Matq on 12/01/2025.
//

#pragma once

#include "../components/element/Frame.h"

class FullscreenMenu : public Frame {
public:
    FullscreenMenu();

    // Ticking
    void HandleEvent(SDL_Event& sdl_event, EventContext& event_summary) override;

    // Manipulation
    void RefreshMenu();

};

