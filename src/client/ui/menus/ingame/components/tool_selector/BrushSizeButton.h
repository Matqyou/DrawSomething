//
// Created by Matq on 16/01/2025.
//

#pragma once

#include "../../../../components/element/OverlayButton.h"
#include "../Canvas.h"
#include "../../../../components/element/Frame.h"
#include "../../../../cursors/Circular.h"

namespace Ingame {
class BrushSizeButton : public OverlayButton {
private:
    TextureData* brush_texture;
    TextureData* composition_texture;
    Cursors::Circular brush_cursor;

public:
    explicit BrushSizeButton(Canvas* canvas, Frame* parent, float brush_size);
    ~BrushSizeButton() override;

    // Getting
    [[nodiscard]] SDL_Cursor* GetBrushCursor() const { return brush_cursor.GetCursor(); }

    // Manipulation
    void UpdateColor(SDL_Color color);

};
}
