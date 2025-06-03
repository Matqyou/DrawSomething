//
// Created by Matq on 28/03/2025.
//

#include "TextureStateIndicator.h"

namespace Editor {
static LinkTexture sTextureVisual("editor.visual_texture");
static LinkTexture sTextureNone("editor.none");

TextureStateIndicator::TextureStateIndicator() {
    this->name = L"TextureStateIndicator";
    this->SetDraw(DRAW_TEXTURE);

    this->SetTextureState(false);
}

TextureStateIndicator::~TextureStateIndicator() = default;

void TextureStateIndicator::SetTextureState(bool using_visual_texture) {
    Texture* texture = (using_visual_texture ? sTextureVisual : sTextureNone).GetTexture();
    this->SetTexture(texture);
    this->UpdateTexturePlacement();
}
}
