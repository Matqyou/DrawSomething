//
// Created by Matq on 22/01/2025.
//

#pragma once

#include "../FullscreenMenu.h"
#include "components/MiniProfile.h"
#include "components/Scribbles.h"

class IntermissionMenu : public FullscreenMenu {
private:
    TextureData* texture_turn_number;
    Frame* text_turn_number;
    Frame* menu_a;

    Intermission::Scribbles* scribbles;
    Intermission::MiniProfile* mini_profile1;
    Intermission::MiniProfile* mini_profile2;
    Frame* menu_b;

public:
    IntermissionMenu();
    ~IntermissionMenu() override;

    // Manipulation
    void ShowMenuA(bool show) {
        menu_a->SetEnabled(show);
        menu_b->SetEnabled(!show);
        RefreshMenu();
    }
    void SetTurnNumber(int turn_number);
    void PlayScribbles();
    void SetProfile1(const std::string& username, TextureData* profile_picture);
    void SetProfile2(const std::string& username, TextureData* profile_picture);

};
