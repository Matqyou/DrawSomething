//
// Created by Matq on 13/01/2025.
//

#pragma once

#include "../../../components/element/TextElement.h"
#include "../../../components/element/Frame.h"

namespace Ingame {
class Header : public Frame {
private:
    TextElement* text_turn;
    TextElement* text_turn_number;
    TextElement* text_title;
    TextElement* text_description;
    Frame* picture;

//    TextureData* texture_title;
//    TextureData* texture_description;
//    TextureData* texture_turn;
//    TextureData* texture_turn_number;
//    TextureData* texture_profile_picture;

public:
    static LinkTexture game_header_turn;
    static LinkTexture game_header_background;

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
