//
// Created by Matq on 12/01/2025.
//

#pragma once

#include "../FullscreenMenu.h"
#include "../../../words/Words.h"
#include "components/Canvas.h"
#include "components/panel/Panel.h"
#include "components/Header.h"
#include "components/color_selector/ColorSelector.h"
#include "components/tool_selector/ToolSelector.h"

class IngameMenu : public FullscreenMenu {
public:
    Words* words_list;

    Ingame::Header* header;
    Ingame::Panel* panel;
    Ingame::ColorSelector* color_selector;
    Ingame::ToolSelector* tool_selector;
    Ingame::Canvas* canvas;

    static LinkFont sFontDefault;
    static LinkFont sFontDefaultBiggest;
    static LinkFont sFontDefaultBigger;
    static LinkFont sFontDefaultSmaller;

public:
    explicit IngameMenu(Words* words_list);

    // Manipulation
    void PrepareGuess();
    void PrepareWatch();
    void PrepareDraw();

};
