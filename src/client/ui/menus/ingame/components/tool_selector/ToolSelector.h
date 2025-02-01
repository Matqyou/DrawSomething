//
// Created by Matq on 13/01/2025.
//

#pragma once

#include "../../../../components/element/Frame.h"
#include "../../../../components/element/Button.h"
#include "../../../../components/element/OverlayButton.h"
#include "../Canvas.h"
#include "DrawingToolButton.h"

namespace Ingame {
class ToolSelector : public Frame {
public:
    DrawingToolButton* pencil_tool;
    DrawingToolButton* eraser_tool;
    Button* trash_button;

    using Callback = std::function<void()>;
    Callback callback;

public:
    explicit ToolSelector(Canvas* canvas);

};
}
