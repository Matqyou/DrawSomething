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

    header = new IngameHeader();

    // Canvas
    canvas = (Canvas*)(new Canvas(Vec2i(0, 0),
                                  Vec2i(0, 0),
                                  DRAW_RECT))
//        ->SetCallback([this]() {
//            this->AddChildren({ });
//            this->Refresh();
//        })
        ->SetFullyOccupy(true, false)
        ->SetOccupy(false, true)
        ->SetColor(255, 255, 255, 255)
        ->SetName("Canvas", false);

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
    SetChildren({ header, canvas });
    Refresh();
    canvas->SetIntro(INTRO_GUESS);
    canvas->SetCallback([this]() {
        this->SetChildren({ header, canvas, panel });
        this->Refresh();
    });
}

void IngameMenu::PrepareWatch() {
    SetChildren({ header, canvas });
    Refresh();
    canvas->SetIntro(INTRO_WATCH);
    canvas->SetCallback([this]() {
        this->SetChildren({ header, canvas, panel });
        this->Refresh();
    });
}

void IngameMenu::PrepareDraw() {
    SetChildren({ header, canvas });
    Refresh();
    canvas->SetIntro(INTRO_DRAW);
    canvas->SetCallback([this]() {
        this->SetChildren({ header, color_selector, canvas, tool_selector });
        this->Refresh();
    });
}
