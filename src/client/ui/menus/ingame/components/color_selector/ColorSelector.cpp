//
// Created by Matq on 13/01/2025.
//

#include "ColorSelector.h"
#include "../../../../../core/Application.h"
#include "../tool_selector/BrushSizeButton.h"

auto static sCallbackScaleNearest = [](Texture* texture) {
    SDL_SetTextureScaleMode(texture->SDLTexture(), SDL_SCALEMODE_NEAREST);
};

static auto sGenerateMoreColors = [](AssetsClass* assets) -> Texture* {
    Texture* more_colors = assets->RenderTextBlended(assets->GetFont("fredoka.small")->TTFFont(),
                                                     "more colors",
                                                     { 173, 173, 173, 255 })
        ->FlagForAutomaticDeletion();
    return more_colors;
};

static PregenerateTexture sTextureMoreColors("game.color_selector.more_colors", sGenerateMoreColors);

static SDL_Color sAvailableColors[13] = {
    SDL_Color(0, 0, 0, 255),
    SDL_Color(255, 0, 0, 255),
    SDL_Color(255, 255, 0, 255),
    SDL_Color(0, 0, 255, 255),
    SDL_Color(0, 255, 0, 255), //
    SDL_Color(255, 0, 255, 255), //
    SDL_Color(0, 255, 255, 255), //
    SDL_Color(255, 128, 0, 255), //
    SDL_Color(128, 255, 0, 255), //
    SDL_Color(0, 255, 128, 255), //
    SDL_Color(0, 128, 255, 255), //
    SDL_Color(128, 0, 255, 255), //
    SDL_Color(255, 0, 128, 255), //
};

namespace Ingame {
PreloadTexture ColorSelector::sTextureColor("game.color_selector.color", sCallbackScaleNearest);
PreloadTexture ColorSelector::sTextureColorOverlay("game.color_selector.color_overlay", sCallbackScaleNearest);
PreloadTexture ColorSelector::sTexturePlus("game.color_selector.plus", sCallbackScaleNearest);

ColorSelector::ColorSelector(Canvas* canvas, Ingame::ToolSelector* tool_selector)
    : Frame(Vec2i(0, 0), Vec2i(0, 50), DONT_DRAW) {
    auto assets = Assets::Get();
    auto drawing = assets->GetDrawing();

    // Colors
    std::vector<Element*> colors;
    for (int i = 0; i < 13; i++) {
        SDL_Color sdl_color = sAvailableColors[i];
        auto color = ColorSelector::sTextureColor.GetTexture();
        auto color_overlay = ColorSelector::sTextureColorOverlay.GetTexture();
        Texture* color_composition = assets->CreateTexture(SDL_PIXELFORMAT_RGBA8888,
                                                           SDL_TEXTUREACCESS_TARGET,
                                                           (int)color->GetWidth(),
                                                           (int)color->GetHeight())
            ->FlagForAutomaticDeletion();
        color_composition->SetScaleMode(SDL_SCALEMODE_NEAREST);
        color_composition->SetBlendMode(SDL_BLENDMODE_BLEND);
        drawing->SetRenderTarget(color_composition);
        color->SetColorMod(sdl_color);
        drawing->RenderTextureFullscreen(color->SDLTexture(), nullptr);
        drawing->RenderTextureFullscreen(color_overlay->SDLTexture(), nullptr);

        auto new_color = (Button*)(new Button(Vec2i(0, 0),
                                              Vec2i(42, 42),
                                              color_composition))
            ->SetAlign(Align::DONT, Align::CENTER)
            ->SetName("Color", false);

        new_color->SetCallback([canvas, sdl_color, tool_selector]() {
            canvas->SetDrawColor(sdl_color);
            canvas->SetTool(TOOL_PENCIL);
            tool_selector->SetFocus(tool_selector->pencil_tool);
            for (auto brush_size_button : tool_selector->pencil_brush_frame->children) {
                auto button = (BrushSizeButton*)brush_size_button;
                button->UpdateColor(sdl_color);
            }
        });
        colors.push_back(new_color);
    }

    // More colors button
    auto more_colors_button = (Button*)(new Button(Vec2i(0, 0),
                                                   Vec2i(42, 42),
                                                   sTexturePlus.GetTexture()))
        ->SetCallback([]() { std::wcout << "More Colors\n"; })
        ->SetAlign(Align::DONT, Align::CENTER)
        ->SetName("MoreColors", false);
    colors.push_back(more_colors_button);

    auto more_colors = (new Frame(Vec2i(0, 0),
                                  Vec2i(sTextureMoreColors.GetTexture()->GetSize()),
                                  sTextureMoreColors.GetTexture()))
        ->SetAlign(Align::DONT, Align::CENTER)
        ->SetName("MoreColorsText", false);
    colors.push_back(more_colors);

    // Color selector
    SetFullyOccupy(true, false);
    SetFlex(Flex::WIDTH, 4);
    SetName("ColorSelector", false);
    AddChildren(colors);
}
}
