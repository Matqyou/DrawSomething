//
// Created by Matq on 12/01/2025.
//

#pragma once

#include "base/FullscreenMenu.h"
#include "../components/Canvas.h"
#include "components/Panel.h"

class IngameMenu : public FullscreenMenu {
private:
    Frame* header;
    Canvas* canvas;
    Panel* panel;
    Frame* color_selector;
    Frame* brush_selector;

public:
    static PreloadTexture game_header_turn;
    static PreloadTexture game_header_picture;
    static PreloadTexture game_header_background;

    static LinkFont sFontDefault;
    static LinkFont sFontDefaultBiggest;
    static LinkFont sFontDefaultBigger;
    static LinkFont sFontDefaultSmaller;

public:
    IngameMenu();

    // Manipulation
    void PrepareGuess();
    void PrepareWatch();
    void PrepareDraw();

};
