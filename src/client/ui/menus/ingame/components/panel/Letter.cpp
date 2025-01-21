//
// Created by Matq on 21/01/2025.
//

#include "Letter.h"
#include "../../ingame_common.h"

namespace Ingame {
SDL_Color Letter::sBlueColor(41, 77, 104, 255);
SDL_Color Letter::sBlueColorBackground(100, 190, 255, 255);
PreloadTexture Letter::sTextureLetterNormal("game.ingame_panel.letter.normal");

Letter::Letter()
    : Button(Vec2i(0, 0),
             Vec2i(66, 66),
             nullptr) {
    this->letter = "";
    this->generated = nullptr;

    SetAlign(Align::DONT, Align::CENTER);
    SetName("Letter");
}

Letter::~Letter() {
    delete generated;
}

Letter* Letter::UpdateRender(const std::string& letter, SDL_Color background, SDL_Color tone) {
    auto assets = Assets::Get();
    auto drawing = assets->GetDrawing();
    this->letter = letter;

    Texture* character_render =
        assets->RenderTextBlended(Ingame::sFontDefaultBiggest.GetFont()->TTFFont(), letter, { 255, 255, 255, 255 });
    auto background_texture = Letter::sTextureLetterNormal.GetTexture();
    auto size = Letter::sTextureLetterNormal.GetTexture()->GetSize();
    SDL_FRect character_rect = {
        (float)(size.x / 2 - character_render->GetWidth()),
        (float)(size.y / 2 - character_render->GetHeight()),
        (float)(character_render->GetWidth() * 2),
        (float)(character_render->GetHeight() * 2),
    };
    SDL_FRect character_rect_higher = character_rect;
    character_rect_higher.y -= 4;
    delete generated;
    generated = assets->CreateTexture(SDL_PIXELFORMAT_RGBA8888,
                                      SDL_TEXTUREACCESS_TARGET,
                                      (int)Letter::sTextureLetterNormal.GetTexture()->GetWidth(),
                                      (int)Letter::sTextureLetterNormal.GetTexture()->GetHeight())
        ->SetBlendMode(SDL_BLENDMODE_BLEND);
    drawing->SetRenderTarget(generated);
    background_texture->SetColorMod(background);
    drawing->RenderTextureFullscreen(background_texture->SDLTexture(), nullptr);
    character_render->SetColorMod(tone);
    drawing->RenderTexture(character_render->SDLTexture(), nullptr, character_rect_higher);
    character_render->SetColorMod(255, 255, 255);
    drawing->RenderTexture(character_render->SDLTexture(), nullptr, character_rect);
//    drawing->SetRenderTarget(nullptr);
    delete character_render;

    SetVisualTexture(generated);
    return this;
}
}
