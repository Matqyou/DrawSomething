//
// Created by Matq on 16/01/2025.
//

#include "BrushSizeButton.h"

#include "../../../../../../shared/core/Numbers.h"

auto static sCallbackScaleNearest = [](Texture* texture) {
    SDL_SetTextureScaleMode(texture->SDLTexture(), SDL_SCALEMODE_NEAREST);
};

PreloadTexture sTextureOutlineSelected2("game.tool_selector.outline_selected", sCallbackScaleNearest); // todo: not a good idea ( duplicate names aswell )
PreloadTexture sTextureOutlineBackground2("game.tool_selector.outline_background", sCallbackScaleNearest);

namespace Ingame {
BrushSizeButton::BrushSizeButton(Canvas* canvas, Frame* parent, float brush_size)
    : OverlayButton(Vec2i(0, 0),
                    Vec2i(54, 54),
                    nullptr,
                    sTextureOutlineSelected2.GetTexture()) {
    this->composition_texture = nullptr;

    auto assets = Assets::Get();
    auto drawing = assets->GetDrawing();
    float visual_brush_size = Numbers::mapValue(brush_size, 5.0f, 35.0f, 5.0f, 20.0f);
    this->brush_texture = assets->CreateTexture(SDL_PIXELFORMAT_RGBA8888,
                                                SDL_TEXTUREACCESS_TARGET,
                                                size.x, size.y);
    SDL_SetTextureScaleMode(brush_texture->SDLTexture(), SDL_SCALEMODE_NEAREST);
    brush_texture->SetBlendMode(SDL_BLENDMODE_BLEND);
    drawing->SetRenderTarget(brush_texture);
    drawing->FillCircle(Vec2f(size) / 2.0f, visual_brush_size, { 255, 255, 255, 255 });

    brush_cursor.Generate(brush_size);
    UpdateColor({ 0, 0, 0, 255 });
    SetName("BrushSize", false);
}

BrushSizeButton::~BrushSizeButton() {
    delete brush_texture;
    delete composition_texture;
}

void BrushSizeButton::UpdateColor(SDL_FColor color) {
    auto assets = Assets::Get();
    auto drawing = assets->GetDrawing();
    auto button_outline = sTextureOutlineBackground2.GetTexture();

    delete composition_texture;
    composition_texture = assets->CreateTexture(SDL_PIXELFORMAT_RGBA8888,
                                                SDL_TEXTUREACCESS_TARGET,
                                                size.x, size.y);
    composition_texture->SetScaleMode(SDL_SCALEMODE_NEAREST);
    composition_texture->SetBlendMode(SDL_BLENDMODE_BLEND);
    drawing->SetRenderTarget(composition_texture);
    drawing->RenderTextureFullscreen(button_outline->SDLTexture(), nullptr);
    brush_texture->SetColorMod(color);
    drawing->RenderTextureFullscreen(brush_texture->SDLTexture(), nullptr);

    SetVisualTexture(composition_texture);
}
}
