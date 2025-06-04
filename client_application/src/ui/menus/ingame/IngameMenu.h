//
// Created by Matq on 12/01/2025.
//

#pragma once

#include "ui/menus/FullscreenMenu.h"
#include "ui/menus/ingame/components/Canvas.h"
#include "ui/menus/ingame/components/panel/Panel.h"
#include "ui/menus/ingame/components/Header.h"
#include "ui/menus/ingame/components/color_selector/ColorSelector.h"
#include "ui/menus/ingame/components/tool_selector/ToolSelector.h"
#include "game/game_info/GameInfo.h"

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
	[[nodiscard]] Ingame::ColorSelector* GetColorSelector() const { return color_selector; }
	[[nodiscard]] Ingame::ToolSelector* GetToolSelector() const { return tool_selector; }

    // Manipulation
    void PrepareGuess();
    void PrepareWatch();
    void PrepareDraw();

	void SetDoneDrawingCallback(Callback new_callback);

};
