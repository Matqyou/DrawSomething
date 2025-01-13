//
// Created by Matq on 13/01/2025.
//

#pragma once

#include "../../../components/element/Frame.h"
#include "../../../components/element/Button.h"
#include "../../../components/element/OverlayButton.h"

class IngameToolSelector : public Frame {
public:
    OverlayButton* pencil_tool;
    OverlayButton* eraser_tool;
    Button* trash_button;

    using Callback = std::function<void()>;
    Callback callback;

public:
    static PreloadTexture sTextureOutline;
    static PreloadTexture sTextureOutlineSelected;
    static PreloadTexture sTexturePencil;
    static PreloadTexture sTextureEraser;
    static PreloadTexture sTextureTrash;
    static PreloadTexture sTextureGear;
    static PreloadTexture sTextureDoneButton;

    static LinkFont sFontDefaultBigger;
    static LinkFont sFontDefaultSmaller;

public:
    IngameToolSelector();

};
