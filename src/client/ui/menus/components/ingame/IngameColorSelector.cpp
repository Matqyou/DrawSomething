//
// Created by Matq on 13/01/2025.
//

#include "IngameColorSelector.h"
#include "../../../../core/Application.h"
#include "../../../components/element/Button.h"

auto static sCallbackScaleNearest = [](Texture* texture) {
    SDL_SetTextureScaleMode(texture->SDLTexture(), SDL_SCALEMODE_NEAREST);
};

PreloadTexture IngameColorSelector::sTextureColor("game.color_selector.color", sCallbackScaleNearest);
PreloadTexture IngameColorSelector::sTextureColorOverlay("game.color_selector.color_overlay", sCallbackScaleNearest);
PreloadTexture IngameColorSelector::sTexturePlus("game.color_selector.plus", sCallbackScaleNearest);

static auto sGenerateMoreColors = [](AssetsClass* assets) -> Texture* {
    Texture* more_colors = assets->RenderTextBlended(assets->GetFont("fredoka.small")->TTFFont(),
                                                     "more colors",
                                                     { 173, 173, 173, 255 })
        ->FlagForAutomaticDeletion();
    return more_colors;
};

static PregenerateTexture sTextureMoreColors("game.color_selector.more_colors", sGenerateMoreColors);

static SDL_FColor sAvailableColors[] = {
    { 0, 0, 0, 255 },
    { 255, 0, 0, 255 },
    { 255, 255, 0, 255 },
    { 0, 0, 255, 255 },
    { 0, 255, 0, 255 }, //
    { 255, 0, 255, 255 }, //
    { 0, 255, 255, 255 }, //
};

IngameColorSelector::IngameColorSelector(Canvas* canvas)
    : Frame(Vec2i(0, 0), Vec2i(0, 50), DONT_DRAW) {
    auto assets = Assets::Get();
    auto drawing = assets->GetDrawing();

    // Colors
    std::vector<Element*> colors;
    for (int i = 0; i < sizeof(sAvailableColors) / sizeof(SDL_FColor); i++) {
        auto sdl_color = sAvailableColors[i];
        auto color = IngameColorSelector::sTextureColor.GetTexture();
        auto color_overlay = IngameColorSelector::sTextureColorOverlay.GetTexture();
        Texture* color_composition = assets->CreateTexture(SDL_PIXELFORMAT_RGBA8888,
                                                           SDL_TEXTUREACCESS_TARGET,
                                                           (int)color->GetWidth(),
                                                           (int)color->GetHeight())
            ->FlagForAutomaticDeletion();
        SDL_SetTextureScaleMode(color_composition->SDLTexture(), SDL_SCALEMODE_NEAREST);
        color_composition->SetBlendMode(SDL_BLENDMODE_BLEND);
        drawing->SetRenderTarget(color_composition);
        color->SetColorMod(sdl_color);
        drawing->RenderTextureFullscreen(color->SDLTexture(), nullptr);
        drawing->RenderTextureFullscreen(color_overlay->SDLTexture(), nullptr);

        auto new_color = (Button*)(new Button(Vec2i(0, 0),
                                     Vec2i(42, 42),
                                     color_composition))
            ->SetAlign(DONT_ALIGN, ALIGN_CENTER)
            ->SetName(Strings::FString("Color%d", i).c_str(), false);
        new_color->SetCallback([canvas, sdl_color]() {
            canvas->SetDrawColor(sdl_color);
        });
        colors.push_back(new_color);
    }

    // More colors button
    auto more_colors_button = (new Button(Vec2i(0, 0),
                                   Vec2i(42, 42),
                                   sTexturePlus.GetTexture()))
        ->SetAlign(DONT_ALIGN, ALIGN_CENTER)
        ->SetName("MoreColors", false);
    colors.push_back(more_colors_button);

    auto more_colors = (new Frame(Vec2i(0, 0),
                                   Vec2i(sTextureMoreColors.GetTexture()->GetSize()),
                                   sTextureMoreColors.GetTexture()))
        ->SetAlign(DONT_ALIGN, ALIGN_CENTER)
        ->SetName("MoreColorsText", false);
    colors.push_back(more_colors);

    // Color selector
    SetFullyOccupy(true, false);
    SetFlex(FLEX_WIDTH, 4);
    SetName("ColorSelector", false);
    AddChildren(colors);
}