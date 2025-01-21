//
// Created by Matq on 13/01/2025.
//

#pragma once

#include "../../../../components/element/Frame.h"
#include "../Canvas.h"
#include "../tool_selector/ToolSelector.h"

namespace Ingame {
class ColorSelector : public Frame {
private:

public:
    static PreloadTexture sTextureColor;
    static PreloadTexture sTextureColorOverlay;
    static PreloadTexture sTexturePlus;

//    static Texture sTextureColorComposition;

    static LinkFont sFontDefaultBigger;
    static LinkFont sFontDefaultSmaller;

public:
    ColorSelector(Canvas* canvas, Ingame::ToolSelector* tool_selector);

};
}
