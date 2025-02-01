//
// Created by Matq on 20/01/2025.
//

#include "Colors.h"
#include <cmath>

namespace Colors {
SDL_FColor ColorToFloat(SDL_Color sdl_color) {
    return SDL_FColor((float)sdl_color.r / 255.0f,
                      (float)sdl_color.g / 255.0f,
                      (float)sdl_color.b / 255.0f,
                      (float)sdl_color.a / 255.0f);
}

SDL_Color ColorToInt(SDL_FColor sdl_fcolor) {
    return SDL_Color((int)(sdl_fcolor.r * 255.0f),
                     (int)(sdl_fcolor.g * 255.0f),
                     (int)(sdl_fcolor.b * 255.0f),
                     (int)(sdl_fcolor.a * 255.0f));
}

template<class T>
T Clamp(T value, T minimum, T maximum) {
    if (value < minimum)
        return minimum;

    if (value > maximum)
        return maximum;

    return value;
}

SDL_Color HSLtoRGB(const ColorHSL& color_hsl) {
    double Hue = std::fmod(color_hsl.h, 360.0);
    double Saturation = Clamp(color_hsl.s, 0.0, 1.0);
    double Light = Clamp(color_hsl.l, 0.0, 1.0);

    double C = Saturation * Light;
    double X = C * (1 - std::abs(std::fmod(Hue / 60.0, 2) - 1));
    double m = Light - C;
    double r, g, b;

    if (Hue >= 0 && Hue < 60) { r = C, g = X, b = 0; }
    else if (Hue >= 60 && Hue < 120) { r = X, g = C, b = 0; }
    else if (Hue >= 120 && Hue < 180) { r = 0, g = C, b = X; }
    else if (Hue >= 180 && Hue < 240) { r = 0, g = X, b = C; }
    else if (Hue >= 240 && Hue < 300) { r = X, g = 0, b = C; }
    else { r = C, g = 0, b = X; }

    return { (Uint8)((r + m) * 255), (Uint8)((g + m) * 255), (Uint8)((b + m) * 255), 255 };
}
}
