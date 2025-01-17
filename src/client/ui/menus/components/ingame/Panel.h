//
// Created by Matq on 12/01/2025.
//

#pragma once

#include "../../../components/element/Frame.h"

namespace Ingame {
class Panel : public Frame {
private:

public:
    static PreloadTexture game_palette_background;
    static PreloadTexture game_palette_bomba;
    static PreloadTexture game_guessing_bar;
    static PreloadTexture game_letter_normal;
    static PreloadTexture game_letter_slot;
    static PreloadTexture game_letter_slot_background;

    static PregenerateTexture game_letter_slot_;

    static LinkFont sFontDefaultBiggest;

public:
    Panel();

};
}
