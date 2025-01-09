//
// Created by Matq on 24/12/2024.
//

#pragma once

#include "../components/element/Frame.h"

class MainMenu : public Frame {
private:
    static LoadedTexture texture_header;

    static LoadedTexture texture_game_header;
    static LoadedTexture texture_game_content;
    static LoadedTexture texture_game_create;

    static LoadedTexture texture_profile_base;

    static LoadedTexture texture_friends_header;
    static LoadedTexture texture_friends_link;

public:
    MainMenu();

    // Ticking
    void HandleEvent(SDL_Event& sdl_event, EventContext& event_summary) override;

};
