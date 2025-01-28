//
// Created by Matq on 13/01/2025.
//

#include "OverlayButton.h"
#include "../../../core/Application.h"

OverlayButton::OverlayButton(const Vec2i& pos,
                             const Vec2i& size,
                             const VisualTexture& texture,
                             const VisualTexture& pressed_texture,
                             const VisualTexture& overlay_texture)
    : Button(pos, size, texture, pressed_texture) {
    name = L"OverlayButton";
    this->overlay_visual_texture = overlay_texture;
}

void OverlayButton::PostRefresh() {
    if (draw == DRAW_VISUAL_TEXTURE) {
        UpdateVisualTexture();
        UpdatePressedVisualTexture();
        UpdateOverlayVisualTexture();
    }
}

void OverlayButton::Render() {
    if (draw != ElementDraw::DONT_DRAW) {
        auto drawing = Application::Get()->GetDrawing();
        auto& fill_color = has_focus ? focus_color : color;

        if (draw == ElementDraw::DRAW_RECT) {
            drawing->SetColor(fill_color);
            drawing->FillRect(GetRect());
        } else if (draw == ElementDraw::DRAW_TEXTURE) {
            drawing->RenderTexture(sdl_texture, nullptr, GetVisualRect());
            if (has_focus)
                drawing->RenderTexture(overlay_visual_texture.SDLTexture(), nullptr, GetVisualRect());
        } else if (draw == ElementDraw::DRAW_VISUAL_TEXTURE) {
            if (pressed_down && pressed_visual_texture.SDLTexture() != nullptr) {
                drawing->RenderTexture(pressed_visual_texture.SDLTexture(), nullptr, pressed_visual_texture.GetVisualRect());
            } else {
                drawing->RenderTexture(visual_texture.SDLTexture(), nullptr, visual_texture.GetVisualRect());
            }
            if (has_focus)
                drawing->RenderTexture(overlay_visual_texture.SDLTexture(), nullptr, overlay_visual_texture.GetVisualRect());
        }
    }

    RenderChildren();
}

void OverlayButton::UpdateOverlayVisualTexture() {
    overlay_visual_texture.UpdateRect(GetRect());
}