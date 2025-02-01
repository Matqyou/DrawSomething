//
// Created by Matq on 29/01/2025.
//

#pragma once

#include "../FullscreenMenu.h"
#include "../../components/element/TextElement.h"

class LoadingScreen : public FullscreenMenu {
private:
    Frame* loading_text;
    Frame* loading_frame;
    std::chrono::steady_clock::time_point last_updated_text;
    size_t current_text;
    Uint8 screen_alpha;

public:
    LoadingScreen();
    ~LoadingScreen() override;

    // Manipulating
    void StartShowing();

    // Ticking
    void HandleEvent(SDL_Event& event, EventContext& event_summary) override;
    void Tick() override;

};
