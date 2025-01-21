//
// Created by Matq on 12/01/2025.
//

#include "Panel.h"
#include "../../../../../core/Application.h"
#include "../../../../components/element/Button.h"

namespace Ingame {

PreloadTexture Panel::game_palette_background("game.ingame_panel.letter_palette.background");
PreloadTexture Panel::game_palette_bomba("game.ingame_panel.letter_palette.bomba");

Panel::Panel()
    : Frame(Vec2i(0, 0), Vec2i(0, 0), DONT_DRAW) {
    // Guessing bar
    guessing_bar = new GuessingBar();
    guessing_bar->SetCallback([]() { std::wcout << L"Word guessed correctly!\n"; });

    // Rows frame
    letters = new Letters(guessing_bar);

    // Rows side
    auto rows = (new Frame(Vec2i(0, 0),
                           Vec2i(0, 0),
                           DONT_DRAW))
        ->SetOccupy(true, false)
        ->SetFullyOccupy(false, true)
        ->SetName("Rows")
        ->AddChildren({ letters });

    // Bomba
    auto bomba = (new Button(Vec2i(0, 0),
                             Vec2i(98, 147),
                             game_palette_bomba.GetTexture()))
        ->SetCallback([]() { std::wcout << L"BOMBA\n"; })
        ->SetAlign(Align::CENTER, Align::CENTER)
        ->SetName("Bomba");

    // Bomba side
    auto bomba_frame = (new Frame(Vec2i(0, 0),
                                  Vec2i(114, 0),
                                  DONT_DRAW))
        ->SetFullyOccupy(false, true)
        ->SetFlex(Flex::WIDTH)
        ->SetName("Bomba")
        ->AddChildren({ bomba });

    // Letters bar
    auto letters_bar = (new Frame(Vec2i(0, 0),
                                  Vec2i(0, 165),
                                  game_palette_background.GetTexture()))
        ->SetFullyOccupy(true, false)
        ->SetFlex(Flex::WIDTH)
        ->SetName("Bottom")
        ->AddChildren({ rows, bomba_frame });

    SetFullyOccupy(true, false);
    SetAdaptive(false, true);
    SetFlex(Flex::HEIGHT);
    SetName("Panel");
    AddChildren({ guessing_bar, letters_bar });
}

bool Panel::RandomizeWord(const std::string& word) {
    if (letters->RandomizeWord(word)) { // todo: make dynamic num of letters but stay at 12
        guessing_bar->GenerateForWord(word);
        return true;
    }

    return false;
}
}
