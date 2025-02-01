//
// Created by Matq on 13/01/2025.
//

#pragma once

#include "Button.h"

class OverlayButton : public Button {
protected:
    VisualTexture overlay_visual_texture;

public:
    OverlayButton(const Vec2i& pos,
                  const Vec2i& size,
                  const VisualTexture& texture,
                  const VisualTexture& pressed_texture,
                  const VisualTexture& overlay_texture);

    // Options
    Button* SetOverlayVisualTexture(const VisualTexture& visual_texture) {
        this->overlay_visual_texture = visual_texture;
        return this;
    }

    // Ticking
    void PostRefresh() override;
    void Render() override;
    void UpdateOverlayVisualTexture();

};
