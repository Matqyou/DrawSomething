//
// Created by Matq on 24/01/2025.
//

#pragma once

#include "../../../components/element/TextElement.h"
#include "../../../components/element/Frame.h"

namespace Intermission {
class MiniProfile : public Frame {
private:
    TextElement* text;
    Frame* profile_picture;
    TextureData* profile_picture_composition;

public:
    MiniProfile();
    ~MiniProfile() override;

    // Manipulation
    void SetText(const std::string& new_text);
    void SetProfilePicture(TextureData* new_profilepicture);

};
}
