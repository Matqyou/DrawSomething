//
// Created by Matq on 21/01/2025.
//

#pragma once

#include <string>
#include "../../../../components/element/Button.h"

namespace Ingame {
class LetterSlot;
class Letter : public Button {
private:
    char letter;
    TextureData* generated;
    LetterSlot* occupy_slot;

public:
    static SDL_Color sBlueColor;
    static SDL_Color sBlueColorBackground;

public:
    Letter();
    ~Letter() override;

    // Getting
    [[nodiscard]] char GetLetter() const { return letter; }
    [[nodiscard]] LetterSlot* GetOccupySlot() const { return occupy_slot; }

    // Manipulation
    Letter* UpdateRender(char letter, SDL_Color background, SDL_Color tone);
    void SetOccupySlot(LetterSlot* occupy_slot);
    void ResetLetter();

};
}
