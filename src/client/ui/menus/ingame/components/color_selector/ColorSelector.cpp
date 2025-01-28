//
// Created by Matq on 13/01/2025.
//

#include "ColorSelector.h"
#include "../../../../../core/Application.h"
#include "../../../../CommonUI.h"
#include "../tool_selector/BrushSizeButton.h"

namespace Ingame {
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

static auto sGenerateMoreColors = [](AssetsClass* assets) -> TextureData* {
    TextureData* more_colors = assets->RenderTextBlended(CommonUI::sFontSmaller.GetFont()->TTFFont(),
                                                         "more colors",
                                                         { 173, 173, 173, 255 })
        ->FlagForAutomaticDeletion();
    return more_colors;
};
TextureData* sGeneratePlus(AssetsClass* assets) {
    auto drawing = assets->GetDrawing();
    auto plus_texture = assets->GetTexture("game.color_selector.plus");
    auto plus_size = plus_texture->GetSize();
    TextureData* plus = assets->CreateTexture(SDL_PIXELFORMAT_RGBA8888,
                                                      SDL_TEXTUREACCESS_TARGET,
                                                      (int)plus_size.x, (int)plus_size.y)
        ->FlagForAutomaticDeletion();
    drawing->SetRenderTarget(plus);
    plus_texture->SetColorMod(255, 255, 255);
    drawing->RenderTextureFullscreen(plus_texture->SDLTexture(), nullptr);
    return plus;
}
TextureData* sGeneratePlusPressed(AssetsClass* assets) {
    auto drawing = assets->GetDrawing();
    auto plus_texture = assets->GetTexture("game.color_selector.plus");
    auto plus_size = plus_texture->GetSize();
    TextureData* plus_pressed = assets->CreateTexture(SDL_PIXELFORMAT_RGBA8888,
                                                      SDL_TEXTUREACCESS_TARGET,
                                                      (int)plus_size.x, (int)plus_size.y)
        ->FlagForAutomaticDeletion();
    drawing->SetRenderTarget(plus_pressed);
    plus_texture->SetColorMod(200, 200, 200);
    drawing->RenderTextureFullscreen(plus_texture->SDLTexture(), nullptr);
    return plus_pressed;
}

static LinkTexture sTextureColor("game.color_selector.color", CommonUI::sCallbackScaleNearest);
static LinkTexture sTextureColorOverlay("game.color_selector.color_overlay", CommonUI::sCallbackScaleNearest);
static LinkTexture sTexturePlus_("game.color_selector.plus", CommonUI::sCallbackScaleNearest);

static PregenerateTexture sTextureMoreColors("game.color_selector.more_colors", sGenerateMoreColors);
static PregenerateTexture sTexturePlus("game.color_selector.plus_", sGeneratePlus);
static PregenerateTexture sTexturePlusPressed("game.color_selector.plus_pressed", sGeneratePlusPressed);

ColorSelector::ColorSelector(Canvas* canvas, Ingame::ToolSelector* tool_selector)
    : Frame(Vec2i(0, 0), Vec2i(0, 50), DONT_DRAW) {
    auto assets = Assets::Get();
    auto drawing = assets->GetDrawing();

    // Colors
    std::vector<Element*> colors;
    for (auto sdl_color : sAvailableColors) {
        auto color = sTextureColor.GetTexture();
        auto color_overlay = sTextureColorOverlay.GetTexture();
        TextureData* color_composition = assets->CreateTexture(SDL_PIXELFORMAT_RGBA8888,
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
                                              VisualTexture(color_composition->SDLTexture(),
                                                            Vec2d(1.0, 1.0),
                                                            Vec2d(0.0, 0.0)),
                                              VisualTexture(nullptr, Vec2d(1.0, 1.0), Vec2d(0.0, 0.0))))
            ->SetAlign(Align::DONT, Align::CENTER)
            ->SetName("Color");

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
                                                   VisualTexture(sTexturePlus.GetSDLTexture()),
                                                   VisualTexture(sTexturePlusPressed.GetSDLTexture())))
        ->SetCallback([]() { std::wcout << "More Colors\n"; })
        ->SetAlign(Align::DONT, Align::CENTER)
        ->SetName("MoreColors");
    colors.push_back(more_colors_button);

    auto more_colors = (new Frame(Vec2i(0, 0),
                                  Vec2i(sTextureMoreColors.GetTexture()->GetSize()),
                                  sTextureMoreColors.GetSDLTexture()))
        ->SetAlign(Align::DONT, Align::CENTER)
        ->SetName("MoreColorsText");
    colors.push_back(more_colors);

    // Color selector
    SetFullyOccupy(true, false);
    SetFlex(Flex::WIDTH, 4);
    SetName("ColorSelector");
    AddChildren(colors);
}
}
