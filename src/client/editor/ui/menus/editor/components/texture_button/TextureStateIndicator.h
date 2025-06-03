//
// Created by Matq on 28/03/2025.
//

#pragma once

#include "../../../../../../ui/components/element/Frame.h"

namespace Editor {
class TextureStateIndicator : public Frame {
private:

public:
    TextureStateIndicator();
    ~TextureStateIndicator() override;

    // Manipulating
    void SetTextureState(bool using_visual_texture);

};
}
