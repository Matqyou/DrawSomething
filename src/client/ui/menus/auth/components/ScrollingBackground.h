//
// Created by Matq on 05/04/2025.
//

#pragma once

#include "../../../components/element/Frame.h"

namespace Auth {
class ScrollingBackground : public Frame {
private:
    Vec2f scrolling_offset;
    Vec2f scrolling_direction;
    float upscale_texture;

	bool start_offset;//

public:
    ScrollingBackground();
    ~ScrollingBackground() override;

    // Ticking
    void Tick(double elapsed_seconds) override;
    void Render() override;

};
}
