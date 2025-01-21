//
// Created by Matq on 21/01/2025.
//

#pragma once

#include "../../../../components/element/Button.h"

namespace Ingame {
class Letter;
class LetterSlot : public Button {
private:
    Texture* generated;
    Letter* occupied;

public:
    static SDL_Color sColorBlue;
    static PreloadTexture game_letter_slot;
    static PreloadTexture game_letter_slot_background;

public:
    LetterSlot();
    ~LetterSlot();

    // Getting
    [[nodiscard]] Letter* GetOccupyLetter() const { return occupied; }

    // Manipulation
    LetterSlot* UpdateRender(SDL_Color tone);
    void OccupyLetter(Letter* new_letter);
    void RemoveLetter();

    // Ticking
    void Render() override;

};
}
