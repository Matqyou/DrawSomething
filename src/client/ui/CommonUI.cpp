//
// Created by Matq on 23/01/2025.
//

#include "CommonUI.h"

namespace CommonUI {
PreloadFont sFontGiant2("fredoka.giant2", "fredoka.bold", 150);
PreloadFont sFontGiant1("fredoka.giant1", "fredoka.bold", 60);
PreloadFont sFontBiggest("fredoka.biggest", "fredoka.bold", 46);
PreloadFont sFontBigger("fredoka.big", "fredoka.bold", 36);
PreloadFont sFontSmall("fredoka.small", "fredoka.semibold", 32);
PreloadFont sFontSmallerBold("fredoka.smaller.semibold", "fredoka.semibold", 18);
PreloadFont sFontSmaller("fredoka.smaller.medium", "fredoka.medium", 18);
PreloadFont sFontDefault("default", "fredoka.bold", 26);
PreloadFont sFontMinecraft24("minecraft", "minecraft", 24);
PreloadFont sFontMath16("math", "computer_modern.cmunrm", 16);

PreloadFont sFontMiniProfile("fredoka.smaller_bold", "fredoka.semibold", 20);

// Link fonts
LinkFont sFontInstructions("fredoka.biggest");
LinkFont sFontAuth("fredoka.smaller.medium");

// Callbacks
void sCallbackScaleNearest(TextureData* texture) {
    texture->SetScaleMode(SDL_SCALEMODE_NEAREST);
}

}