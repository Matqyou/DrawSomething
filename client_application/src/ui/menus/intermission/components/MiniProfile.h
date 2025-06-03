//
// Created by Matq on 24/01/2025.
//

#pragma once

#include "ui/components/element/TextElement.h"
#include "ui/components/element/Frame.h"

namespace Intermission {
class MiniProfile : public Frame {
private:
    TextElement* text;
    Frame* profile_picture;
    Texture* profile_picture_composition;

public:
    MiniProfile();
    ~MiniProfile() override;

    // Manipulation
    void SetText(const std::string& new_text);
    void SetProfilePicture(Texture* new_profilepicture);

};
}
