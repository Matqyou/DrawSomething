//
// Created by Matq on 21/01/2025.
//

#include "LetterSlot.h"
#include "Letter.h"

namespace Ingame {
SDL_Color LetterSlot::sColorBlue(100, 190, 255, 255);
PreloadTexture LetterSlot::game_letter_slot("game.ingame_panel.letter.slot");
PreloadTexture LetterSlot::game_letter_slot_background("game.ingame_panel.letter.slot_background");

LetterSlot::LetterSlot()
    : Button(Vec2i(0, 0),
             Vec2i(66, 66),
             nullptr) {
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
//    drawing->SetRenderTarget(nullptr);
    SetVisualTexture(generated);
    return this;
}

void LetterSlot::OccupyLetter(Letter* new_letter) {
    occupied = new_letter;
}

void LetterSlot::RemoveLetter() {
    if (occupied != nullptr) {
        occupied->SetDraw(ElementDraw::DRAW_TEXTURE);
        occupied->SetClickable(true);
        SetClickable(false);
        occupied = nullptr;
    }
}

void LetterSlot::Render() {
    if (draw != ElementDraw::DONT_DRAW) {
        auto drawing = Assets::Get()->GetDrawing();

        if (occupied == nullptr) {
            if (draw == ElementDraw::DRAW_RECT) {
                auto& fill_color = has_focus ? focus_color : color;
                drawing->SetColor(fill_color);
                drawing->FillRect(GetRect());
            } else if (draw == ElementDraw::DRAW_TEXTURE) {
                drawing->RenderTexture(visual_texture->SDLTexture(), nullptr, GetVisualRect());
            }
        } else {
            drawing->RenderTexture(occupied->GetVisualTexture()->SDLTexture(), nullptr, GetVisualRect());
        }
    }

    RenderChildren();
}
}
