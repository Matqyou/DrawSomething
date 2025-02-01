//
// Created by Matq on 31/01/2025.
//

#include "ConfirmationScreen.h"
#include "../../CommonUI.h"
#include "../../components/element/Button.h"

ConfirmationScreen::ConfirmationScreen()
    : FullscreenMenu() {

    auto title = (TextElement*)(new TextElement(Vec2i(0, 0)))
        ->UpdateText(CommonUI::sFontBigger.GetTTFFont(), "Are you sure?", { 255, 255, 255, 255 })
        ->SetAlign(Align::CENTER, Align::DONT)
        ->SetName("Title");

    question = (TextElement*)(new TextElement(Vec2i(0, 0)))
        ->UpdateText(CommonUI::sFontSmaller.GetTTFFont(),
                     "?",
                     { 255, 255, 255, 255 })
        ->SetAlign(Align::CENTER, Align::DONT)
        ->SetName("Question");

    auto yes = (Button*)(new Button(Vec2i(0, 0),
                           Vec2i(110, 50),
                           DRAW_RECT))
        ->SetColor(0, 255, 0, 255)
        ->SetName("Yes");
    yes->SetCallback([this]() {
        this->SetEnabled(false);
    });

    auto no = (Button*)(new Button(Vec2i(0, 0),
                          Vec2i(110, 50),
                          DRAW_RECT))
        ->SetColor(255, 0, 0, 255)
        ->SetName("No");
    no->SetCallback([this]() {
        this->SetEnabled(false);
    });

    auto buttons_frame = (new Frame(Vec2i(0, -15), Vec2i(0, 0), DONT_DRAW))
        ->SetFlexInvolved(false, false)
        ->SetAlign(Align::CENTER, Align::BOTTOM)
        ->SetFlex(Flex::WIDTH, 50)
        ->SetAdaptive(true, true)
        ->SetName("ButtonsFrame")
        ->AddChildren({ no, yes });

    auto confirmation_frame = (Frame*)(new Frame(Vec2i(0, 0),
                                                 Vec2i(400, 200),
                                                 DRAW_RECT))
        ->SetAlign(Align::CENTER, Align::CENTER)
        ->SetFlex(Flex::HEIGHT, 10)
        ->AddChildren({ title, question, buttons_frame });

    SetEnabled(false);
    SetColor(0, 0, 0, 150);
    SetFlexInvolved(false, false);
    SetFullyOccupy(true, true);
    SetName("ConfirmationScreen");
    AddChildren({ confirmation_frame });
}

ConfirmationScreen::~ConfirmationScreen() {

}

void ConfirmationScreen::Prompt(const char* prompt_question) {
    this->SetEnabled(true);
    this->question->UpdateText(CommonUI::sFontSmaller.GetTTFFont(),
                               prompt_question,
                               { 255, 255, 255, 255 });
    this->parent->Refresh();
}
