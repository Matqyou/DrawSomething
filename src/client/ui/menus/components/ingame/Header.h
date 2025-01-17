//
// Created by Matq on 13/01/2025.
//

#pragma once

#include "../../../components/element/Frame.h"

namespace Ingame {
class Header : public Frame {
private:
    Frame* text_turn;
    Frame* text_turn_number;
    Frame* text_title;
    Frame* text_description;
    Frame* picture;

    Texture* texture_title;
    Texture* texture_description;
    Texture* texture_turn;
    Texture* texture_turn_number;
    Texture* texture_profile_picture;

public:
    static PreloadTexture game_header_turn;
    static PreloadTexture game_header_background;

    static LinkFont sFontDefaultBigger;
    static LinkFont sFontDefaultSmaller;

public:
    Header();
    ~Header() override;

    // Manipulation
    void SetTitle(const std::string& new_title);
    void SetDescription(const std::string& new_description);
    void SetTurnNumber(int turn_number);
    void SetProfilePicture(Texture* profile_picture);

};
}
