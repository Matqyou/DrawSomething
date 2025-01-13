//
// Created by Matq on 13/01/2025.
//

#pragma once

#include "../../../components/element/Frame.h"

class IngameHeader : public Frame {
private:

public:
    static PreloadTexture game_header_turn;
    static PreloadTexture game_header_picture;
    static PreloadTexture game_header_background;

    static LinkFont sFontDefaultBigger;
    static LinkFont sFontDefaultSmaller;

public:
    IngameHeader();

};
