//
// Created by Matq on 03/02/2025.
//

#include "TextureButton.h"
#include "ui/CommonUI.h"

namespace Editor {
TextureButton::TextureButton(const char* texture_key, TextureStateIndicator* state_indicator)
 : TextElement() {
    this->texture_key = texture_key;
    this->state_indicator = state_indicator;

    this->hitbox_file.OpenFile(R"(.\assets\images\)", texture_key);
    this->state_indicator->SetTextureState(hitbox_file.UsesHitbox());

    this->UpdateTextColor();
    this->SetName("TextureKey");
}

TextureButton::~TextureButton() {

}

void TextureButton::UpdateTextColor() {
    auto text_color = hitbox_file.UsesHitbox() ? SDL_Color(100, 255, 100, 255) : SDL_Color(255, 100, 100, 255);
    this->UpdateText(CommonUI::sFontSmaller.GetTTFFont(), texture_key.c_str(), text_color);
}
}
