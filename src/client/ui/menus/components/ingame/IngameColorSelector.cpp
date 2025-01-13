//
// Created by Matq on 13/01/2025.
//

#include "IngameColorSelector.h"
#include "../../../../core/Application.h"

PreloadTexture IngameColorSelector::sTextureColor("game.color_selector.color");
PreloadTexture IngameColorSelector::sTextureColorOverlay("game.color_selector.color_overlay");
PreloadTexture IngameColorSelector::sTexturePlus("game.color_selector.plus");

IngameColorSelector::IngameColorSelector()
    : Frame(Vec2i(0, 0), Vec2i(0, 50), DONT_DRAW) {
    auto drawing = Application::Get()->GetDrawing();

    // Colors
    std::vector<Element*> colors;
    for (int i = 0; i < 4; i++) {
        auto new_color = (new Frame(Vec2i(0, 0),
                                 Vec2i(42, 42),
                                 sTextureColor.GetTexture()))
            ->SetAlign(DONT_ALIGN, ALIGN_CENTER)
            ->SetName(Strings::FString("Color%d", i).c_str(), false);
        colors.push_back(new_color);
    }

    // More colors button
    auto more_colors = (new Frame(Vec2i(0, 0),
                                Vec2i(42, 42),
                                sTexturePlus.GetTexture()))
        ->SetAlign(DONT_ALIGN, ALIGN_CENTER)
        ->SetName("More Colors", false);
    colors.push_back(more_colors);

    // Color selector
    SetFullyOccupy(true, false);
    SetFlex(FLEX_WIDTH, 4);
    SetName("ColorSelector", false);
    AddChildren(colors);
}