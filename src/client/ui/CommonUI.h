//
// Created by Matq on 23/01/2025.
//

#pragma once

#include "../core/app/Assets.h"

namespace CommonUI {
// Load fonts
extern PreloadFont sFontGiant2;
extern PreloadFont sFontGiant1;
extern PreloadFont sFontBiggest;
extern PreloadFont sFontBigger;
extern PreloadFont sFontSmallBold;
extern PreloadFont sFontSmall;
extern PreloadFont sFontSmallerBold;
extern PreloadFont sFontSmaller;
extern PreloadFont sFontDefault;
extern PreloadFont sFontMinecraft24;
extern PreloadFont sFontMath16;

extern PreloadFont sFontMiniProfile;

// Link fonts
extern LinkFont sFontInstructions;
extern LinkFont sFontAuth;

// Callbacks
extern void sCallbackScaleNearest(TextureData* texture);

}