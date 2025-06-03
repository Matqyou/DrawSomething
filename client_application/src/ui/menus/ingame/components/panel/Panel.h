//
// Created by Matq on 12/01/2025.
//

#pragma once

#include "ui/components/element/Frame.h"
#include "GuessingBar.h"
#include "Letters.h"

namespace Ingame {
class Panel : public Frame {
private:
    GuessingBar* guessing_bar;
    Letters* letters;
    Button* bomba_button;

public:
    static LinkTexture game_palette_background;
    static LinkTexture game_palette_bomba;

    void ResetBomba();

public:
    Panel();

    // Manipulation
    bool RandomizeWord(const std::string& word);

};
}
