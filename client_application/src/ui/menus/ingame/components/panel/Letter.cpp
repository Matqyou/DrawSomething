//
// Created by Matq on 21/01/2025.
//

#include "Letter.h"
#include "ui/CommonUI.h"

namespace Ingame {
SDL_Color Letter::sBlueColor(41, 77, 104, 255);
SDL_Color Letter::sBlueColorBackground(100, 190, 255, 255);
static LinkTexture sTextureLetterNormal("game.ingame_panel.letter.normal");

Letter::Letter()
    : Button(nullptr, nullptr) {
    this->letter = '#';
    this->generated = nullptr;
    this->occupy_slot = nullptr;

    this->SetSize(Vec2i(66, 66));
    this->SetAlign(Align::DONT, Align::CENTER);
    this->SetName("Letter");
}

Letter::~Letter() {
    delete generated;
}

Letter* Letter::UpdateRender(char letter, SDL_Color background, SDL_Color tone) {
    auto assets = Assets::Get();
    auto drawing = assets->GetDrawing();
    this->letter = letter;

    Texture* character_render = assets->RenderTextBlended(CommonUI::sFontBiggest.GetFont()->TTFFont(),
                                                          std::string(1, letter),
                                                          { 255, 255, 255, 255 });
    Vec2f render_size = character_render->GetOriginalSize();
    auto background_texture = sTextureLetterNormal.GetTexture();
    auto size = sTextureLetterNormal.GetTexture()->GetOriginalSize();
    SDL_FRect character_rect = {
        (float)(size.x / 2 - render_size.x),
        (float)(size.y / 2 - render_size.y),
        (float)(render_size.x * 2),
        (float)(render_size.y * 2),
    };
    SDL_FRect character_rect_higher = character_rect;
    delete generated;
    this->generated = assets->CreateTexture(SDL_PIXELFORMAT_RGBA8888,
                                            SDL_TEXTUREACCESS_TARGET,
                                            (int)size.x, (int)size.y)
        ->SetBlendMode(SDL_BLENDMODE_BLEND);
    drawing->SetRenderTarget(generated);
    background_texture->SetColorMod(background);
    drawing->RenderTextureFullscreen(background_texture->SDLTexture(), nullptr);
    character_render->SetColorMod(tone);
    for (int i = 0; i < 5; i++) {
        character_rect_higher.y--;
        drawing->RenderTexture(character_render->SDLTexture(), nullptr, character_rect_higher);
    }
    character_render->SetColorMod(255, 255, 255);
    drawing->RenderTexture(character_render->SDLTexture(), nullptr, character_rect);
    delete character_render;

    SetTexture(generated);
    return this;
}

void Letter::SetOccupySlot(LetterSlot* occupy_slot) {
    this->occupy_slot = occupy_slot;
}

void Letter::ResetLetter() {
    SetClickable(true);
    SetDraw(ElementDraw::DRAW_TEXTURE);
    SetOccupySlot(nullptr);
}

void Letter::SetAnsweredCorrectly() {
    this->UpdateRender(this->letter, { 75, 200, 75, 255 }, { 52, 117, 46, 255 });
    this->UpdateTexturePlacement();
}
}
