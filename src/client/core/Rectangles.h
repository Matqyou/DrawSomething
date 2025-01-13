//
// Created by Matq on 10/01/2025.
//

#pragma once

#include "../../shared/core/Vec2.h"
#include "SDL3/SDL_rect.h"

namespace Rectangles {
template<typename T>
Vec2<T> ScaleByWidth(const Vec2<T>& original, T new_width) {
    double height_to_width = static_cast<double>(original.y) / static_cast<double>(original.x);
    return { new_width, static_cast<T>(height_to_width * new_width) };
}

template<typename T>
Vec2<T> ScaleByHeight(const Vec2<T>& original, T new_height) {
    double width_to_height = static_cast<double>(original.x) / static_cast<double>(original.y);
    return { static_cast<T>(width_to_height * new_height), new_height };
}

template<typename T>
Vec2<T> ScaleToBounds(const Vec2<T>& original, const Vec2<T>& bounds) {
    double original_ratio = static_cast<double>(original.x) / static_cast<double>(original.y);
    double bounds_ratio = static_cast<double>(bounds.y) / static_cast<double>(bounds.x);

    if (original_ratio < bounds_ratio) { return ScaleByHeight(original, bounds.y); }
    else { return ScaleByWidth(original, bounds.x); }
}

template<typename T>
SDL_FRect CenterRelative(const Vec2<T>& original, const Vec2<T>& container) {
    return { static_cast<float>(container.x - original.x) / 2.0f,
             static_cast<float>(container.y - original.y) / 2.0f,
             static_cast<float>(original.x),
             static_cast<float>(original.y) };
}

}