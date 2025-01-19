//
// Created by Matq on 12/01/2025.
//

#pragma once

#include "base/FullscreenMenu.h"
#include "../../words/Words.h"
#include "../components/Canvas.h"
#include "components/ingame/Panel.h"
#include "components/ingame/Header.h"
#include "components/ingame/ColorSelector.h"
#include "components/ingame/ToolSelector.h"

class IngameMenu : public FullscreenMenu {
public:
    Words* words_list;

    Ingame::Header* header;
    Ingame::Panel* panel;
    Ingame::ColorSelector* color_selector;
    Ingame::ToolSelector* tool_selector;
    Canvas* canvas;

    static LinkFont sFontDefault;
    static LinkFont sFontDefaultBiggest;
    static LinkFont sFontDefaultBigger;
    static LinkFont sFontDefaultSmaller;

public:
    IngameMenu(Words* words_list);

    // Manipulation
    void PrepareGuess();
    void PrepareWatch();
    void PrepareDraw();

};
