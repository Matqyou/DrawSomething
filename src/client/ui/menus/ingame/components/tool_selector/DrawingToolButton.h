//
// Created by Matq on 31/01/2025.
//

#pragma once

#include "../../../../components/element/OverlayButton.h"
#include "../../../../components/element/Frame.h"
#include "../Canvas.h"

namespace Ingame {
class ToolSelector;
class DrawingToolButton : public OverlayButton {
private:
    Frame* brush_frame;
    ToolSelector* tool_selector_;

    TextureData* generated_overlay;

    void GenerateOverlayColor(const SDL_Color& sdl_color);

public:
    DrawingToolButton(Canvas* canvas,
                      ToolSelector* tool_selector,
                      const Vec2i& pos,
                      const VisualTexture& texture);
    ~DrawingToolButton() override;

    // Getting
    [[nodiscard]] SDL_Cursor* GetSelectedCursor();

    // Manipulating
    void GenerateBrushesColor(const SDL_Color& sdl_color);
    void HideBrushSizeButtons();
    void ToggleShowBrushSizeButtons();

};
}
