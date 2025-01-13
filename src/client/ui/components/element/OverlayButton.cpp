//
// Created by Matq on 13/01/2025.
//

#include "OverlayButton.h"
#include "../../../core/Application.h"

OverlayButton::OverlayButton(const Vec2i& pos, const Vec2i& size, const Vec2i& visual, const Vec2i& offset, Texture* texture, Texture* overlay)
    : Button(pos, size, visual, offset, texture) {
    this->overlay = overlay;
}

OverlayButton::OverlayButton(const Vec2i& pos, const Vec2i& size, Texture* texture, Texture* overlay)
    : Button(pos, size, size, Vec2i(0, 0), texture){
    this->overlay = overlay;
}

void OverlayButton::Render() {
    if (draw != ElementDraw::DONT_DRAW) {
        auto drawing = Application::Get()->GetDrawing();
        auto& fill_color = has_focus ? focus_color : color;

        if (draw == ElementDraw::DRAW_RECT) {
            drawing->SetColor(fill_color);
            drawing->FillRect(GetRect());
        } else if (draw == ElementDraw::DRAW_TEXTURE) {
            drawing->RenderTexture(visual_texture->SDLTexture(), nullptr, GetVisualRect());
            if (has_focus)
                drawing->RenderTexture(overlay->SDLTexture(), nullptr, GetVisualRect());
        }
    }

    RenderChildren();
}