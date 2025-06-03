//
// Created by Matq on 31/01/2025.
//

#pragma once

#include "ui/components/element/OverlayButton.h"
#include "ui/components/element/Frame.h"
#include "ui/menus/ingame/components/Canvas.h"

namespace Ingame {
class ToolSelector;
class DrawingToolButton : public OverlayButton {
private:
    Frame* brush_frame;
    ToolSelector* tool_selector_;

    Texture* generated_overlay;

    void GenerateOverlayColor(const SDL_Color& sdl_color);

public:
    DrawingToolButton(Canvas* canvas, ToolSelector* tool_selector);
    ~DrawingToolButton() override;

    // Getting
    [[nodiscard]] SDL_Cursor* GetSelectedCursor();

    // Manipulating
    void GenerateBrushesColor(const SDL_Color& sdl_color);
    void HideBrushSizeButtons();
    void ToggleShowBrushSizeButtons();

};
}
