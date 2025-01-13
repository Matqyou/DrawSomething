//
// Created by Matq on 13/01/2025.
//

#pragma once

#include "Button.h"

class OverlayButton : public Button {
private:
    Texture* overlay;

public:
    OverlayButton(const Vec2i& pos, const Vec2i& size, const Vec2i& visual, const Vec2i& offset, Texture* texture, Texture* overlay);
    OverlayButton(const Vec2i& pos, const Vec2i& size, Texture* texture, Texture* overlay);

    // Ticking
    void Render() override;

};
