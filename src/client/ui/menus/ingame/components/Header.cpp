//
// Created by Matq on 13/01/2025.
//

#include "Header.h"
#include "../../../../core/Application.h"
#include "../../../CommonUI.h"

namespace Ingame {
LinkTexture Header::game_header_turn("game.header.turn");
LinkTexture Header::game_header_background("game.header.background");

Header::Header()
    : Frame(Vec2i(0, 0), Vec2i(0, 73), DONT_DRAW) {
    name = L"Header";

    text_turn = nullptr;
    text_turn_number = nullptr;
    text_title = nullptr;
    text_description = nullptr;
    picture = nullptr;

    auto assets = Assets::Get();
    texture_profile_picture = assets->GetTexture(Strings::FString("profile_pictures.cat%d", 1 + rand() % 12));
    texture_turn = nullptr;
    texture_turn_number = nullptr;
    texture_title = nullptr;
    texture_description = nullptr;

    // Texts
    TTF_Font* font_smaller = CommonUI::sFontSmaller.GetFont()->TTFFont();
    const char* turn_text = "turn";
    const char* title_text = "This is the title!";
    const char* description_text = "This is the description.";
    texture_turn = assets->RenderTextBlended(font_smaller, turn_text, { 255, 255, 255, 255 });
    SetTitle(title_text);
    SetDescription(description_text);
    SetTurnNumber(-1);

    // Turn title text
    text_turn = (Frame*)(new Frame(Vec2i(0, 4),
                                   Vec2i(texture_turn->GetSize()),
                                   texture_turn->SDLTexture()))
        ->SetName("TurnTitle");

    // Turn number text
    text_turn_number = (Frame*)(new Frame(Vec2i(0, -4),
                                          Vec2i(texture_turn_number->GetSize()),
                                          texture_turn_number->SDLTexture()))
        ->SetAlign(Align::CENTER, Align::DONT)
        ->SetName("TurnNumber");

    // Turn text frame
    auto turn_info_frame = (new Frame(Vec2i(0, 0), Vec2i(50, 30), DONT_DRAW))
        ->SetFlex(Flex::HEIGHT)
        ->SetAlign(Align::CENTER, Align::CENTER)
        ->SetAdaptive(true, true)
        ->SetName("TurnInfo")
        ->AddChildren({ text_turn, text_turn_number });

    // Turn display
    auto turn_display = (new Frame(Vec2i(0, 0), Vec2i(73, 73), game_header_turn.GetSDLTexture()))
        ->SetName("Turn")
        ->AddChildren({ turn_info_frame });

    // Profile picutre
    picture = (Frame*)(new Frame(Vec2i(3, 3), Vec2i(60, 60), texture_profile_picture->SDLTexture()))
        ->SetName("Picture");

    // Game info text1
    text_title = (Frame*)(new Frame(Vec2i(0, 0), Vec2i(texture_title->GetSize()), texture_title->SDLTexture()))
        ->SetName("GuessingText");

    // Game info text2
    text_description = (Frame*)(new Frame(Vec2i(0, 0), Vec2i(texture_description->GetSize()), texture_description->SDLTexture()))
        ->SetName("DrawingText");

    // Game info text frame
    auto round_info_frame = (new Frame(Vec2i(3, 0), Vec2i(50, 30), DONT_DRAW))
        ->SetFlex(Flex::HEIGHT)
        ->SetAlign(Align::DONT, Align::CENTER)
        ->SetAdaptive(true, true)
        ->SetName("RoundInfoText")
        ->AddChildren({ text_title, text_description });

    // Game info
    auto round_info = (new Frame(Vec2i(0, 0), Vec2i(0, 73), game_header_background.GetSDLTexture()))
        ->SetFlex(Flex::WIDTH, 7)
        ->SetOccupy(true, false)
        ->SetName("RoundInfo")
        ->AddChildren({ picture, round_info_frame });

    // Header
    SetFullyOccupy(true, false);
    SetFlex(Flex::WIDTH);
    SetName("Header");
    AddChildren({ turn_display, round_info });
}

Header::~Header() {
    delete texture_title;
    delete texture_description;
    delete texture_turn;
    delete texture_turn_number;
}

void Header::SetTitle(const std::string& new_title) {
    auto assets = Assets::Get();
    delete texture_title;
    texture_title = assets->RenderTextBlended(CommonUI::sFontSmaller.GetFont()->TTFFont(),
                                              new_title,
                                              { 0, 0, 0, 255 });
    if (text_title) {
        text_title->sdl_texture = texture_title->SDLTexture();
        text_title->size = Vec2i(texture_title->GetSize());
        text_title->visual_size = Vec2i(texture_title->GetSize());
    }
}

void Header::SetDescription(const std::string& new_description) {
    auto assets = Assets::Get();
    delete texture_description;
    texture_description = assets->RenderTextBlended(CommonUI::sFontSmaller.GetFont()->TTFFont(),
                                                    new_description,
                                                    { 0, 0, 0, 255 });
    if (text_description) {
        text_description->sdl_texture = texture_description->SDLTexture();
        text_description->size = Vec2i(texture_description->GetSize());
        text_description->visual_size = Vec2i(texture_description->GetSize());
    }
}

void Header::SetTurnNumber(int turn_number) {
    auto assets = Assets::Get();
    delete texture_turn_number;
    texture_turn_number = assets->RenderTextBlended(CommonUI::sFontBigger.GetFont()->TTFFont(),
                                                    Strings::FString("%d", turn_number),
                                                    { 255, 255, 255, 255 });
    if (text_turn_number) {
        text_turn_number->sdl_texture = texture_turn_number->SDLTexture();
        text_turn_number->size = Vec2i(texture_turn_number->GetSize());
        text_turn_number->visual_size = Vec2i(texture_turn_number->GetSize());
    }
}

void Header::SetProfilePicture(TextureData* profile_picture) {
    texture_profile_picture = profile_picture;
}
}
