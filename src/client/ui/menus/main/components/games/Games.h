//
// Created by Matq on 26/01/2025.
//

#pragma once

#include "../../../../../game/game_info/GameInfo.h"
#include "../../../../components/element/Frame.h"
#include "../../../confirmation/ConfirmationScreen.h"
#include "../../../loading/LoadingScreen.h"

namespace Main {
class Games : public Frame {
private:
    friend class Game;
    Frame* games;
    ConfirmationScreen* confirmation_screen_;
    LoadingScreen* loading_screen_;

    // Temp
    GameInfo game_info_example1;
    GameInfo game_info_example2;

public:
    Games(ConfirmationScreen* confirmation_screen, LoadingScreen* loading_screen);
    ~Games();

    // Manipulation
    void SortGames();

};
}
