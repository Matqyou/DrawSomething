//
// Created by Matq on 03/02/2025.
//

#pragma once

#include "../../../../../../ui/structures/visual_texture/files/HitboxFile.h"
#include "../../../../../../ui/components/element/TextElement.h"
#include "TextureStateIndicator.h"

namespace Editor {
class TextureButton : public TextElement {
private:
    std::string texture_key;
    HitboxFile hitbox_file;
    TextureStateIndicator* state_indicator;

public:
    TextureButton(const char* texture_key, TextureStateIndicator* state_indicator);
    ~TextureButton() override;

    // Getting
    [[nodiscard]] const std::string& GetTextureKey() const { return texture_key; }
    [[nodiscard]] Texture* GetAssignedTexture() { return Assets::Get()->GetTexture(texture_key); }
    [[nodiscard]] TextureStateIndicator* GetStateIndicator() const { return state_indicator; }
    [[nodiscard]] HitboxFile& GetHitboxFile() { return hitbox_file; }

    // Manipulating
    void UpdateTextColor();

};
}
