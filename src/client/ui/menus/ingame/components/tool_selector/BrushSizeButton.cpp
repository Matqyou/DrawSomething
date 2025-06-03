//
// Created by Matq on 16/01/2025.
//

#include "BrushSizeButton.h"

#include "../../../../../../shared/core/Numbers.h"
#include "../../../../CommonUI.h"

namespace Ingame {
static LinkTexture sTextureOutlineBackground2("game.tool_selector.outline_background", CommonUI::sCallbackScaleNearest);

BrushSizeButton::BrushSizeButton(float brush_size)
    : OverlayButton(nullptr, nullptr, nullptr) {
    this->name = L"BrushSize";
    this->SetSize(Vec2i(54, 54));

    this->composition_texture = nullptr;
    this->brush_size = brush_size;

    auto assets = Assets::Get();
    auto drawing = assets->GetDrawing();
    float visual_brush_size = Numbers::mapValue(brush_size, 5.0f, 65.0f, 5.0f, 20.0f);
    this->brush_texture = assets->CreateTexture(SDL_PIXELFORMAT_RGBA8888,
                                                SDL_TEXTUREACCESS_TARGET,
                                                size.x, size.y)
        ->SetScaleMode(SDL_SCALEMODE_NEAREST)
        ->SetBlendMode(SDL_BLENDMODE_BLEND);
    drawing->SetRenderTarget(brush_texture);
    drawing->FillCircle(Vec2f(size) / 2.0f, visual_brush_size, { 255, 255, 255, 255 });

    this->UpdateColor(SDL_Color(0, 0, 0, 255));
}

BrushSizeButton::~BrushSizeButton() {
    delete brush_texture;
    delete composition_texture;
}

void BrushSizeButton::UpdateColor(SDL_Color sdl_color) {
    this->brush_cursor.Generate(brush_size, sdl_color);
    auto assets = Assets::Get();
    auto drawing = assets->GetDrawing();
    auto button_outline = sTextureOutlineBackground2.GetTexture();

    delete this->composition_texture;
    this->composition_texture = assets->CreateTexture(SDL_PIXELFORMAT_RGBA8888,
                                                      SDL_TEXTUREACCESS_TARGET,
                                                      size.x, size.y);
    this->composition_texture->SetScaleMode(SDL_SCALEMODE_NEAREST);
    this->composition_texture->SetBlendMode(SDL_BLENDMODE_BLEND);
    drawing->SetRenderTarget(composition_texture);
    drawing->RenderTextureFullscreen(button_outline->SDLTexture(), nullptr);
    this->brush_texture->SetColorMod(sdl_color);
    drawing->RenderTextureFullscreen(brush_texture->SDLTexture(), nullptr);

    this->texture_instance.ChangeTexture(composition_texture);
}
}
