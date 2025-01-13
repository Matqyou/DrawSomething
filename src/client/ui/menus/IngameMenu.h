//
// Created by Matq on 12/01/2025.
//

#pragma once

#include "base/FullscreenMenu.h"
#include "../components/Canvas.h"
#include "components/ingame/IngamePanel.h"
#include "components/ingame/IngameHeader.h"
#include "components/ingame/IngameColorSelector.h"
#include "components/ingame/IngameToolSelector.h"

class IngameMenu : public FullscreenMenu {
private:
    IngameHeader* header;
    Canvas* canvas;
    IngamePanel* panel;
    IngameColorSelector* color_selector;
    IngameToolSelector* tool_selector;

public:
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
