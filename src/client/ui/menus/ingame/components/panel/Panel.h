//
// Created by Matq on 12/01/2025.
//

#pragma once

#include "../../../../components/element/Frame.h"
#include "GuessingBar.h"
#include "Letters.h"

namespace Ingame {
class Panel : public Frame {
private:
    GuessingBar* guessing_bar;
    Letters* letters;

public:
    static PreloadTexture game_palette_background;
    static PreloadTexture game_palette_bomba;

public:
    Panel();

    // Manipulation
    bool RandomizeWord(const std::string& word);

};
}
