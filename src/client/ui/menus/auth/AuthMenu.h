//
// Created by Matq on 16/01/2025.
//

#pragma once

#include "../FullscreenMenu.h"
#include "../../structures/window_texture/WindowTexture.h"

class AuthMenu : public FullscreenMenu {
private:
    Sheet temp_sheet, temp_sheet2;
    WindowTexture temp_field, temp_button;

public:
    AuthMenu();
    ~AuthMenu() override;

};
