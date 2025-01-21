//
// Created by Matq on 21/01/2025.
//

#pragma once

#include <string>
#include "../../../../components/element/Button.h"

namespace Ingame {
class Letter : public Button {
private:
    std::string letter;
    Texture* generated;

public:
    static SDL_Color sBlueColor;
    static SDL_Color sBlueColorBackground;
    static PreloadTexture sTextureLetterNormal;

public:
    Letter();
    ~Letter() override;

    // Getting
    [[nodiscard]] std::string GetLetter() const { return letter; }

    // Manipulation
    Letter* UpdateRender(const std::string& letter, SDL_Color background, SDL_Color tone);

};
}
