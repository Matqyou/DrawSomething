//
// Created by Matq on 16/01/2025.
//

#pragma once

#include "ui/components/element/OverlayButton.h"
#include "ui/components/element/Frame.h"
#include "ui/cursors/Circular.h"
#include "ui/menus/ingame/components/Canvas.h"

namespace Ingame {
class BrushSizeButton : public OverlayButton {
private:
    float brush_size;
    Texture* brush_texture;
    Texture* composition_texture;
    Cursors::Circular brush_cursor;

public:
    explicit BrushSizeButton(float brush_size);
    ~BrushSizeButton() override;

    // Getting
    [[nodiscard]] SDL_Cursor* GetBrushCursor() const { return brush_cursor.GetCursor(); }

    // Manipulation
    void UpdateColor(SDL_Color color);

};
}
