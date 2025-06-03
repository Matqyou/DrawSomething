//
// Created by Matq on 19/01/2025.
//

#pragma once

#include "../../../core/app/Assets.h"
#include "Sheet.h"

class WindowTexture {
private:
    Sheet* sheet;
    Texture* generated;
    Vec2f generate_size;

    float left_scaling;
    float right_scaling;
    float top_scaling;
    float bottom_scaling;

public:
    WindowTexture(Sheet* sheet, float left_scaling, float right_scaling, float top_scaling, float bottom_scaling);
    ~WindowTexture();

    // Getting
    [[nodiscard]] Texture* GetGenerated() const { return generated; }
    [[nodiscard]] Vec2f GetSize() const { return generate_size; }

    // Manipulation
    Texture* Generate(AssetsClass* assets, const Vec2f& new_size);

};
