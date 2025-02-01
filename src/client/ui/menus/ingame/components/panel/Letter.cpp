//
// Created by Matq on 21/01/2025.
//

#include "Letter.h"
#include "../../../../CommonUI.h"

namespace Ingame {
SDL_Color Letter::sBlueColor(41, 77, 104, 255);
SDL_Color Letter::sBlueColorBackground(100, 190, 255, 255);
static LinkTexture sTextureLetterNormal("game.ingame_panel.letter.normal");

Letter::Letter()
    : Button(Vec2i(0, 0),
             Vec2i(66, 66),
             VisualTexture(nullptr),
             VisualTexture(nullptr)) {
    this->letter = '#';
    this->generated = nullptr;
    this->occupy_slot = nullptr;

    SetAlign(Align::DONT, Align::CENTER);
    SetName("Letter");
}

Letter::~Letter() {
    delete generated;
}

Letter* Letter::UpdateRender(char letter, SDL_Color background, SDL_Color tone) {
    auto assets = Assets::Get();
    auto drawing = assets->GetDrawing();
    this->letter = letter;

    TextureData* character_render = assets->RenderTextBlended(CommonUI::sFontBiggest.GetFont()->TTFFont(),
                                                              std::string(1, letter),
                                                              { 255, 255, 255, 255 });
    auto background_texture = sTextureLetterNormal.GetTexture();
    auto size = sTextureLetterNormal.GetTexture()->GetSize();
    SDL_FRect character_rect = {
        (float)(size.x / 2 - character_render->GetWidth()),
        (float)(size.y / 2 - character_render->GetHeight()),
        (float)(character_render->GetWidth() * 2),
        (float)(character_render->GetHeight() * 2),
    };
    SDL_FRect character_rect_higher = character_rect;
    delete generated;
    generated = assets->CreateTexture(SDL_PIXELFORMAT_RGBA8888,
                                      SDL_TEXTUREACCESS_TARGET,
                                      (int)sTextureLetterNormal.GetTexture()->GetWidth(),
                                      (int)sTextureLetterNormal.GetTexture()->GetHeight())
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

    SetVisualTexture(VisualTexture(generated->SDLTexture()));
    return this;
}

void Letter::SetOccupySlot(LetterSlot* occupy_slot) {
    this->occupy_slot = occupy_slot;
}

void Letter::ResetLetter() {
    SetClickable(true);
    SetDraw(ElementDraw::DRAW_VISUAL_TEXTURE);
    SetOccupySlot(nullptr);
}

void Letter::SetAnsweredCorrectly() {
    this->UpdateRender(this->letter, { 75, 200, 75, 255 }, { 52, 117, 46, 255 });
    this->UpdateVisualTexture();
}
}
