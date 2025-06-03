//
// Created by Matq on 13/01/2025.
//

#include "Header.h"
#include "core/Application.h"
#include "ui/CommonUI.h"

namespace Ingame {
LinkTexture Header::game_header_turn("game.header.turn");
LinkTexture Header::game_header_background("game.header.background");

Header::Header()
    : Frame() {
    this->name = L"Header";

    auto assets = Assets::Get();

    // Turn title text
    this->text_turn = (TextElement*)(new TextElement())
        ->UpdateText(CommonUI::sFontSmaller.GetFont()->TTFFont(), "turn", { 255, 255, 255, 255 })
        ->SetRelative(Vec2i(0, 4))
        ->SetName("TurnTitle");

    // Turn number text
    this->text_turn_number = (TextElement*)(new TextElement())
        ->UpdateText(CommonUI::sFontBigger.GetFont()->TTFFont(), "NaN", { 255, 255, 255, 255 })
        ->SetRelative(Vec2i(0, -4))
        ->SetAlign(Align::CENTER, Align::DONT)
        ->SetName("TurnNumber");

    // Turn text frame
    auto turn_info_frame = (new Frame())
        ->SetSize(Vec2i(50, 30))
        ->SetFlex(Flex::HEIGHT)
        ->SetAlign(Align::CENTER, Align::CENTER)
        ->SetAdaptive(true, true)
        ->SetName("TurnInfo")
        ->AddChildren({ text_turn, text_turn_number });

    // Turn display
    auto turn_display = (new Frame())
        ->SetSize(Vec2i(73, 73))
        ->SetTexture(game_header_turn.GetTexture())
        ->SetDraw(DRAW_TEXTURE)
        ->SetName("Turn")
        ->AddChildren({ turn_info_frame });

    // Profile picutre
    auto random_cat = assets->GetTexture(Strings::FString("profile_pictures.cat%d", 1 + rand() % 12));
    this->picture = (Frame*)(new Frame())
        ->SetRelative(Vec2i(3, 3))
        ->SetSize(Vec2i(60, 60))
        ->SetTexture(random_cat)
        ->SetDraw(DRAW_TEXTURE)
        ->SetName("Picture");

    // Game info text1
    this->text_title = (TextElement*)(new TextElement())
        ->UpdateText(CommonUI::sFontSmaller.GetFont()->TTFFont(),
                     "Title", { 0, 0, 0, 255 })
        ->SetName("GuessingText");

    // Game info text2
    text_description = (TextElement*)(new TextElement())
        ->UpdateText(CommonUI::sFontSmaller.GetFont()->TTFFont(),
                     "Description", { 0, 0, 0, 255 })
        ->SetName("DrawingText");

    // Game info text frame
    auto round_info_frame = (new Frame())
        ->SetRelative(Vec2i(3, 0))
        ->SetSize(Vec2i(50, 30))
        ->SetFlex(Flex::HEIGHT)
        ->SetAlign(Align::DONT, Align::CENTER)
        ->SetAdaptive(true, true)
        ->SetName("RoundInfoText")
        ->AddChildren({ text_title, text_description });

    // Game info
    auto round_info = (new Frame())
        ->SetSize(Vec2i(0, 73))
        ->SetFlex(Flex::WIDTH, 7)
        ->SetTexture(game_header_background.GetTexture())
        ->SetDraw(DRAW_TEXTURE)
        ->SetOccupy(true, false)
        ->SetName("RoundInfo")
        ->AddChildren({ picture, round_info_frame });

    // Header
    SetSize(Vec2i(0, 73));
    SetFullyOccupy(true, false);
    SetFlex(Flex::WIDTH);
    SetName("Header");
    AddChildren({ turn_display, round_info });
}

Header::~Header() {

}

void Header::SetTitle(const std::string& new_title) {
    text_title->UpdateText(CommonUI::sFontSmaller.GetFont()->TTFFont(),
                           new_title.c_str(), { 0, 0, 0, 255 });
}

void Header::SetDescription(const std::string& new_description) {
    text_description->UpdateText(CommonUI::sFontSmaller.GetFont()->TTFFont(),
                                 new_description.c_str(), { 0, 0, 0, 255 });
}

void Header::SetTurnNumber(int turn_number) {
    text_turn_number->UpdateText(CommonUI::sFontBigger.GetFont()->TTFFont(),
                                 Strings::FString("%d", turn_number).c_str(),
                                 { 255, 255, 255, 255 });
}

void Header::SetProfilePicture(Texture* profile_picture) {
    picture->SetTexture(profile_picture);
}
}
