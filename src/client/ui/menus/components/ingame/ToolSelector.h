//
// Created by Matq on 13/01/2025.
//

#pragma once

#include "../../../components/element/Frame.h"
#include "../../../components/element/Button.h"
#include "../../../components/element/OverlayButton.h"
#include "../../../components/Canvas.h"

namespace Ingame {
class ToolSelector : public Frame {
public:
    OverlayButton* pencil_tool;
    OverlayButton* eraser_tool;
    Button* trash_button;
    Frame* pencil_brush_frame;
    Frame* eraser_brush_frame;

    using Callback = std::function<void()>;
    Callback callback;

public:
    explicit ToolSelector(Canvas* canvas);

};
}
