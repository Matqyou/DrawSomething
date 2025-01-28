//
// Created by Matq on 12/01/2025.
//

#include "Panel.h"
#include "../../../../../core/Application.h"
#include "../../../../components/element/Button.h"

namespace Ingame {
LinkTexture Panel::game_palette_background("game.ingame_panel.letter_palette.background");
LinkTexture Panel::game_palette_bomba("game.ingame_panel.letter_palette.bomba");

void Panel::ResetBomba() {
    bomba_button->SetClickable(true);
}

Panel::Panel()
    : Frame(Vec2i(0, 0), Vec2i(0, 0), DONT_DRAW) {
    // Guessing bar
    guessing_bar = new GuessingBar();
    guessing_bar->SetCallback([]() { std::wcout << Strings::FStringColorsW(L"&aWord guessed correctly!\n"); });

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
    bomba_button = (Button*)(new Button(Vec2i(0, 0),
                                        Vec2i(98, 147),
                                        VisualTexture(game_palette_bomba.GetSDLTexture(), Vec2d(1.0, 1.0), Vec2d(0.0, 0.0)),
                                        VisualTexture(nullptr, Vec2d(1.0, 1.0), Vec2d(0.0, 0.0))))
        ->SetAlign(Align::CENTER, Align::CENTER)
        ->SetName("Bomba");
    bomba_button->SetCallback([this]() {
        letters->BlowUp();
        bomba_button->SetClickable(false);
    });

    // Bomba side
    auto bomba_frame = (new Frame(Vec2i(0, 0),
                                  Vec2i(114, 0),
                                  DONT_DRAW))
        ->SetFullyOccupy(false, true)
        ->SetFlex(Flex::WIDTH)
        ->SetName("Bomba")
        ->AddChildren({ bomba_button });

    // Letters bar
    auto letters_bar = (new Frame(Vec2i(0, 0),
                                  Vec2i(0, 165),
                                  game_palette_background.GetSDLTexture()))
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
        ResetBomba();
        return true;
    }

    return false;
}
}
