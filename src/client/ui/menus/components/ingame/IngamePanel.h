//
// Created by Matq on 12/01/2025.
//

#pragma once

#include "../../../components/element/Frame.h"

class IngamePanel : public Frame {
private:

public:
    static PreloadTexture game_palette_background;
    static PreloadTexture game_palette_bomba;
    static PreloadTexture game_guessing_bar;
    static PreloadTexture game_letter_normal;

    static Texture game_letter_slot_;

    static LinkFont sFontDefaultBiggest;

public:
    IngamePanel();

};
