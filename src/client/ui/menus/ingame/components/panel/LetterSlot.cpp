//
// Created by Matq on 21/01/2025.
//

#include "LetterSlot.h"
#include "Letter.h"

namespace Ingame {
SDL_Color LetterSlot::sColorBlue(100, 190, 255, 255);
LinkTexture LetterSlot::game_letter_slot("game.ingame_panel.letter.slot");
LinkTexture LetterSlot::game_letter_slot_background("game.ingame_panel.letter.slot_background");

LetterSlot::LetterSlot()
    : Button(nullptr, nullptr) {
    this->generated = nullptr;
    this->occupied = nullptr;

    this->SetSize(Vec2i(66, 66));
    this->SetClickable(false);
    this->SetAlign(Align::DONT, Align::CENTER);
    this->SetName("Slot");
}

LetterSlot::~LetterSlot() {

}

LetterSlot* LetterSlot::UpdateRender(SDL_Color tone) {
    auto assets = Assets::Get();
    auto drawing = assets->GetDrawing();
    auto letter_slot_frame = Ingame::LetterSlot::game_letter_slot.GetTexture();
    Vec2f frame_size = letter_slot_frame->GetOriginalSize();
    auto letter_slot_background = Ingame::LetterSlot::game_letter_slot_background.GetTexture();
    delete generated;
    generated = assets->CreateTexture(SDL_PIXELFORMAT_RGBA8888,
                                      SDL_TEXTUREACCESS_TARGET,
                                      (int)frame_size.x, (int)frame_size.y)
        ->SetBlendMode(SDL_BLENDMODE_BLEND);
    drawing->SetRenderTarget(generated);
    drawing->RenderTextureFullscreen(letter_slot_background->SDLTexture(), nullptr);
    letter_slot_frame->SetColorMod(tone);
    drawing->RenderTextureFullscreen(letter_slot_frame->SDLTexture(), nullptr);
    SetTexture(generated);
    return this;
}

void LetterSlot::OccupyLetter(Letter* new_letter) {
    occupied = new_letter;
}

void LetterSlot::RemoveLetter() {
    if (occupied != nullptr) {
        occupied->ResetLetter();
        SetClickable(false);
        occupied = nullptr;
    }
}

void LetterSlot::Render() {
    if (draw != ElementDraw::DONT_DRAW) {
        auto drawing = Assets::Get()->GetDrawing();

        if (occupied == nullptr) {
            if (draw == ElementDraw::DRAW_TEXTURE) {
                SDL_Texture* pressed_sdl_texture = nullptr;
                if (pressed_texture_instance.GetTexture() != nullptr)
                    pressed_sdl_texture = pressed_texture_instance.GetTexture()->SDLTexture();

                if (pressed_down && pressed_sdl_texture != nullptr) {
                    drawing->RenderTexture(pressed_sdl_texture,
                                           nullptr, pressed_texture_instance.GetResultingFRect());
                } else {
                    drawing->RenderTexture(texture_instance.GetTexture()->SDLTexture(),
                                           nullptr, texture_instance.GetResultingFRect());
                }
            }
        } else {
            // can be problematic if letters themselves have visual offsets and ratios
            drawing->RenderTexture(occupied->texture_instance.GetTexture()->SDLTexture(),
                                   nullptr, texture_instance.GetResultingFRect());
        }
    }

    RenderChildren();
}
}
