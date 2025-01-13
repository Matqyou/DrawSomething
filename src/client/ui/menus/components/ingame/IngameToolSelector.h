//
// Created by Matq on 13/01/2025.
//

#pragma once

#include "../../../components/element/Frame.h"

class IngameToolSelector : public Frame {
private:

public:
    static PreloadTexture sTextureOutline;
    static PreloadTexture sTextureOutlineSelected;
    static PreloadTexture sTexturePencil;
    static PreloadTexture sTextureEraser;
    static PreloadTexture sTextureDoneButton;

    static LinkFont sFontDefaultBigger;
    static LinkFont sFontDefaultSmaller;

public:
    IngameToolSelector();

};
