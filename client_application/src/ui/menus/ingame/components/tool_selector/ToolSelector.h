//
// Created by Matq on 13/01/2025.
//

#pragma once

#include "ui/components/element/Frame.h"
#include "ui/components/element/Button.h"
#include "ui/components/element/OverlayButton.h"
#include "ui/menus/ingame/components/Canvas.h"
#include "DrawingToolButton.h"

namespace Ingame {
class ToolSelector : public Frame {
public:
    DrawingToolButton* pencil_tool;
    DrawingToolButton* eraser_tool;
//    Button* trash_button;
    Button* done_button;

    using Callback = std::function<void()>;

public:
    explicit ToolSelector(Canvas* canvas);

	// Manipulating
	void SetDoneCallback(Callback new_callback);

};
}
