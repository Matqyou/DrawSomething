//
// Created by Matq on 12/01/2025.
//

#pragma once

#include "../FullscreenMenu.h"
#include "components/Canvas.h"
#include "components/panel/Panel.h"
#include "components/Header.h"
#include "components/color_selector/ColorSelector.h"
#include "components/tool_selector/ToolSelector.h"
#include "../../../game/game_info/GameInfo.h"

class IngameMenu : public FullscreenMenu {
public:
    Ingame::Header* header;
    Ingame::Panel* panel;
    Ingame::ColorSelector* color_selector;
    Ingame::ToolSelector* tool_selector;
    Ingame::Canvas* canvas;

    static LinkFont sFontDefault;
    static LinkFont sFontDefaultBiggest;
    static LinkFont sFontDefaultBigger;
    static LinkFont sFontDefaultSmaller;

	using Callback = std::function<void()>;

public:
    IngameMenu();

	// Sense
	[[nodiscard]] Ingame::Canvas* GetCanvas() const { return canvas; }

    // Manipulation
    void PrepareGuess();
    void PrepareWatch();
    void PrepareDraw();

	void SetDoneDrawingCallback(Callback new_callback);

};
