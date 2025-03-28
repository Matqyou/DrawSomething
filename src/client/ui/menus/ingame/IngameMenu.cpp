//
// Created by Matq on 12/01/2025.
//

#include "IngameMenu.h"
#include "../../../core/Application.h"

IngameMenu::IngameMenu(Words* words_list)
    : FullscreenMenu() {
    name = L"IngameMenu";
    this->words_list = words_list;

    // Header
    header = new Ingame::Header();

    // Canvas
    canvas = (Ingame::Canvas*)(new Ingame::Canvas(Vec2i(0, 0), Vec2i(0, 0)))
        ->SetFullyOccupy(true, false)
        ->SetOccupy(false, true)
        ->SetColor(255, 255, 255, 255)
        ->SetName("Canvas");

    // Buttons and stuff
    panel = new Ingame::Panel();
    tool_selector = new Ingame::ToolSelector(canvas);
    color_selector = new Ingame::ColorSelector(canvas, tool_selector);

    SetColor(230, 230, 230, 255);
    SetFlex(Flex::HEIGHT);
    AddChildren({ header, canvas });
    SetName("GuessingMenu");
    RefreshMenu();

    PrepareDraw();
}

void IngameMenu::PrepareGuess() {
    auto random_word = Strings::ToUpperCase(words_list->GetRandomWord());

    header->SetTitle("You are guessing");
    header->SetDescription("Matiss B.'s drawing.");
    header->SetTurnNumber(3);
    panel->RandomizeWord(random_word);

    SetChildren({ header, canvas });
    Refresh();
    canvas->ClearCanvas();
    canvas->SetPlaybackMode(Ingame::PlaybackMode::DO_NOTHING);
    canvas->SetMode(Ingame::CANVAS_GUESS);
    canvas->SetCallback([this]() {
        canvas->SetPlaybackMode(Ingame::PlaybackMode::REPLAY);
        this->SetChildren({ header, canvas, panel });
        this->Refresh();
    });
}

void IngameMenu::PrepareWatch() {
    auto random_word = Strings::ToUpperCase(words_list->GetRandomWord());

    header->SetTitle("You are watching Matiss B.");
    header->SetDescription(Strings::FString("guess the word %s.", random_word.c_str()));
    header->SetTurnNumber(4);
    panel->RandomizeWord(random_word);

    SetChildren({ header, canvas });
    Refresh();
    canvas->ClearCanvas();
    canvas->SetPlaybackMode(Ingame::PlaybackMode::DO_NOTHING);
    canvas->SetMode(Ingame::CANVAS_WATCH);
    canvas->SetCallback([this]() {
        canvas->LoadExample();
        this->SetChildren({ header, canvas, panel });
        this->Refresh();
    });
}

void IngameMenu::PrepareDraw() {
    auto random_word = Strings::ToUpperCase(words_list->GetRandomWord());

    header->SetTitle(Strings::FString("You are drawing %s", random_word.c_str()));
    header->SetDescription("for Matiss B.");
    header->SetTurnNumber(5);
    canvas->ClearCanvas();
    canvas->SetPlaybackMode(Ingame::PlaybackMode::DO_NOTHING);

    SetChildren({ header, canvas });
    Refresh();
    canvas->SetMode(Ingame::CANVAS_DRAW);
    canvas->SetCallback([this]() {
        canvas->SetPlaybackMode(Ingame::PlaybackMode::RECORD);
        this->SetChildren({ header, color_selector, canvas, tool_selector });
        this->Refresh();
    });
}
