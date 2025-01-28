//
// Created by Matq on 13/01/2025.
//

#pragma once

#include "Button.h"

class OverlayButton : public Button {
private:
    VisualTexture overlay_visual_texture;

public:
    OverlayButton(const Vec2i& pos,
                  const Vec2i& size,
                  const VisualTexture& texture,
                  const VisualTexture& pressed_texture,
                  const VisualTexture& overlay_texture);

    // Ticking
    void PostRefresh();
    void Render() override;
    void UpdateOverlayVisualTexture();

};
