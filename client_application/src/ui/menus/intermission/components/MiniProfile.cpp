//
// Created by Matq on 24/01/2025.
//

#include "MiniProfile.h"
#include "ui/CommonUI.h"

namespace Intermission {
MiniProfile::MiniProfile()
    : Frame() {
   this->text = nullptr;
   this->profile_picture = nullptr;
   this->profile_picture_composition = nullptr;

    auto assets = Assets::Get();

    this->text = (TextElement*)(new TextElement())
        ->SetAlign(Align::CENTER, Align::DONT)
        ->SetName("MiniProfileText");

    this->profile_picture = (Frame*)(new Frame())
        ->SetSize(Vec2i(82, 82))
        ->SetDraw(DRAW_TEXTURE)
        ->SetColor(255, 255, 255, 255)
        ->SetAlign(Align::CENTER, Align::DONT)
        ->SetName("MiniProfilePicture");

    this->SetText("Username");
    this->SetProfilePicture(assets->GetTexture(Strings::FString("profile_pictures.cat%d", 1 + rand() % 12)));

    this->SetFlex(Flex::HEIGHT, 6);
    this->SetAdaptive(true, true);
    this->SetName("MiniProfile");
    this->AddChildren({ text, profile_picture });
}

MiniProfile::~MiniProfile() {
    delete profile_picture_composition;
}

void MiniProfile::SetText(const std::string& new_text) {
    text->UpdateText(CommonUI::sFontMiniProfile.GetFont()->TTFFont(),
                     new_text.c_str(), { 255, 255, 255, 255 });
}

void MiniProfile::SetProfilePicture(Texture* new_profilepicture) {
    auto assets = Assets::Get();
    auto drawing = assets->GetDrawing();

    auto profilepicture_size = profile_picture->size - 8;
    SDL_FRect profilepicture_rect = {
        4.0f, 4.0f,
        (float)profilepicture_size.x,
        (float)profilepicture_size.y
    };

    delete profile_picture_composition;
    profile_picture_composition = assets->CreateTexture(SDL_PIXELFORMAT_RGBA8888,
                                                        SDL_TEXTUREACCESS_TARGET,
                                                        profile_picture->size.x, profile_picture->size.y)
        ->SetScaleMode(SDL_SCALEMODE_NEAREST)
        ->SetBlendMode(SDL_BLENDMODE_BLEND);
    drawing->SetRenderTarget(profile_picture_composition);
    drawing->SetColor(255, 255, 255, 255);
    drawing->FillAll();
    drawing->RenderTexture(new_profilepicture->SDLTexture(), nullptr, profilepicture_rect);

    profile_picture->SetTexture(profile_picture_composition);
}
}
