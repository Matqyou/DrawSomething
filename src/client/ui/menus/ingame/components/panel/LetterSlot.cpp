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
    : Button(Vec2i(0, 0),
             Vec2i(66, 66),
             VisualTexture(nullptr),
             VisualTexture(nullptr)) {
    generated = nullptr;
    occupied = nullptr;

    SetClickable(false);
    SetAlign(Align::DONT, Align::CENTER);
    SetName("Slot");
}

LetterSlot::~LetterSlot() {

}

LetterSlot* LetterSlot::UpdateRender(SDL_Color tone) {
    auto assets = Assets::Get();
    auto drawing = assets->GetDrawing();
    auto letter_slot_alone = Ingame::LetterSlot::game_letter_slot.GetTexture();
    auto letter_slot_background = Ingame::LetterSlot::game_letter_slot_background.GetTexture();
    delete generated;
    generated = assets->CreateTexture(SDL_PIXELFORMAT_RGBA8888,
                                      SDL_TEXTUREACCESS_TARGET,
                                      (int)letter_slot_alone->GetWidth(),
                                      (int)letter_slot_alone->GetHeight())
        ->SetBlendMode(SDL_BLENDMODE_BLEND);
    drawing->SetRenderTarget(generated);
    drawing->RenderTextureFullscreen(letter_slot_background->SDLTexture(), nullptr);
    letter_slot_alone->SetColorMod(tone);
    drawing->RenderTextureFullscreen(letter_slot_alone->SDLTexture(), nullptr);
    SetVisualTexture(VisualTexture(generated->SDLTexture()));
    return this;
}

void LetterSlot::OccupyLetter(Letter* new_letter) {
    occupied = new_letter;
}

void LetterSlot::RemoveLetter() {
    if (occupied != nullptr) {
        occupied->SetDraw(ElementDraw::DRAW_TEXTURE);
        occupied->SetClickable(true);
        occupied->SetOccupySlot(nullptr);
        SetClickable(false);
        occupied = nullptr;
    }
}

void LetterSlot::Render() {
    if (draw != ElementDraw::DONT_DRAW) {
        auto drawing = Assets::Get()->GetDrawing();

        if (occupied == nullptr) {
            if (draw == ElementDraw::DRAW_VISUAL_TEXTURE) {
                if (pressed_down && pressed_visual_texture.SDLTexture() != nullptr) {
                    drawing->RenderTexture(pressed_visual_texture.SDLTexture(),
                                           nullptr,
                                           pressed_visual_texture.GetVisualRect());
                } else {
                    drawing->RenderTexture(visual_texture.SDLTexture(), nullptr, visual_texture.GetVisualRect());
                }
            }
        } else {
            // can be problematic if letters themselves have visual offsets and ratios
            drawing->RenderTexture(occupied->visual_texture.SDLTexture(), nullptr, visual_texture.GetVisualRect());
        }
    }

    RenderChildren();
}
}
