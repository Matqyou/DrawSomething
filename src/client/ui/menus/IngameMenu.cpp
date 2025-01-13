//
// Created by Matq on 12/01/2025.
//

#include "IngameMenu.h"
#include "../../core/Application.h"

LinkFont IngameMenu::sFontDefault("default");
LinkFont IngameMenu::sFontDefaultBiggest("fredoka.biggest");
LinkFont IngameMenu::sFontDefaultBigger("fredoka.big");
LinkFont IngameMenu::sFontDefaultSmaller("fredoka.small");

IngameMenu::IngameMenu()
    : FullscreenMenu() {
    auto drawing = Application::Get()->GetDrawing();

    // Header
    header = new IngameHeader();

    // Canvas
    canvas = (Canvas*)(new Canvas(Vec2i(0, 0), Vec2i(0, 0)))
        ->SetFullyOccupy(true, false)
        ->SetOccupy(false, true)
        ->SetColor(255, 255, 255, 255)
        ->SetName("Canvas", false);

    // Buttons and stuff
    panel = new IngamePanel();
    color_selector = new IngameColorSelector();
    tool_selector = new IngameToolSelector();

    SetColor(230, 230, 230, 255);
    SetFlex(FLEX_HEIGHT);
    AddChildren({ header, canvas })->SetName("GuessingMenu", false);
    Refresh();

    PrepareDraw();
}

void IngameMenu::PrepareGuess() {
    header->SetTitle("You are guessing");
    header->SetDescription("Matiss B.'s drawing.");
    header->SetTurnNumber(3);
    SetChildren({ header, canvas });
    Refresh();
    canvas->SetIntro(INTRO_GUESS);
    canvas->SetCallback([this]() {
        this->SetChildren({ header, canvas, panel });
        this->Refresh();
    });
}

void IngameMenu::PrepareWatch() {
    header->SetTitle("You are watching Matiss B.");
    header->SetDescription("guess the word CAVE.");
    header->SetTurnNumber(4);
    SetChildren({ header, canvas });
    Refresh();
    canvas->SetIntro(INTRO_WATCH);
    canvas->SetCallback([this]() {
        this->SetChildren({ header, canvas, panel });
        this->Refresh();
    });
}

void IngameMenu::PrepareDraw() {
    header->SetTitle("You are drawing SUNRISE");
    header->SetDescription("for Matiss B.");
    header->SetTurnNumber(5);
    SetChildren({ header, canvas });
    Refresh();
    canvas->SetIntro(INTRO_DRAW);
    canvas->SetCallback([this]() {
        this->SetChildren({ header, color_selector, canvas, tool_selector });
        this->Refresh();
    });
}
