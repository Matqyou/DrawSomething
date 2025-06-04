//
// Created by Matq on 13/01/2025.
//

#pragma once

#include "ui/components/element/Frame.h"
#include "ui/menus/ingame/components/Canvas.h"
#include "ui/menus/ingame/components/tool_selector/ToolSelector.h"

namespace Ingame {
class ColorSelector : public Frame {
private:

public:
    ColorSelector(Canvas* canvas, Ingame::ToolSelector* tool_selector);

	// Manipulating
	void RefreshData();

};
}
