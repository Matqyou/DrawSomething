//
// Created by Matq on 13/01/2025.
//

#pragma once

#include "../../../components/element/Frame.h"
#include "../../../components/Canvas.h"

class IngameColorSelector : public Frame {
private:

public:
    static PreloadTexture sTextureColor;
    static PreloadTexture sTextureColorOverlay;
    static PreloadTexture sTexturePlus;

//    static Texture sTextureColorComposition;

    static LinkFont sFontDefaultBigger;
    static LinkFont sFontDefaultSmaller;

public:
    IngameColorSelector(Canvas* canvas);

};
