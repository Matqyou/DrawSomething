//
// Created by Matq on 24/12/2024.
//

#pragma once

#include "../FullscreenMenu.h"

class MainMenu : public FullscreenMenu {
private:
    static PreloadTexture texture_header;

    static PreloadTexture texture_game_header;
    static PreloadTexture texture_game_content;
    static PreloadTexture texture_game_create;

    static PreloadTexture texture_profile_base;

    static PreloadTexture texture_friends_header;
    static PreloadTexture texture_friends_link;

public:
    MainMenu();

};
