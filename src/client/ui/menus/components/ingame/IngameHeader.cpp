//
// Created by Matq on 13/01/2025.
//

#include "IngameHeader.h"
#include "../../../../core/Application.h"

PreloadTexture IngameHeader::game_header_turn("game.header.turn");
PreloadTexture IngameHeader::game_header_background("game.header.background");

LinkFont IngameHeader::sFontDefaultBigger("fredoka.big");
LinkFont IngameHeader::sFontDefaultSmaller("fredoka.small");

IngameHeader::IngameHeader()
    : Frame(Vec2i(0, 0), Vec2i(0, 73), DONT_DRAW) {
    text_turn = nullptr;
    text_turn_number = nullptr;
    text_title = nullptr;
    text_description = nullptr;
    picture = nullptr;

    auto assets = Assets::Get();
    texture_profile_picture = assets->GetTexture(Strings::FString("profile_pictures.cat%d", 1 + rand() % 6));
    texture_turn = nullptr;
    texture_turn_number = nullptr;
    texture_title = nullptr;
    texture_description = nullptr;

    // Texts
    TTF_Font* font_smaller = sFontDefaultSmaller.GetFont()->TTFFont();
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
                                   texture_turn))
        ->SetName("TurnTitle", false);

    // Turn number text
    text_turn_number = (Frame*)(new Frame(Vec2i(0, -4),
                                          Vec2i(texture_turn_number->GetSize()),
                                          texture_turn_number))
        ->SetAlign(ALIGN_CENTER, DONT_ALIGN)
        ->SetName("TurnNumber", false);

    // Turn text frame
    auto turn_info_frame = (new Frame(Vec2i(0, 0), Vec2i(50, 30), DONT_DRAW))
        ->SetFlex(FLEX_HEIGHT)
        ->SetAlign(ALIGN_CENTER, ALIGN_CENTER)
        ->SetAdaptive(true, true)
        ->SetName("TurnInfo", false)
        ->AddChildren({ text_turn, text_turn_number });

    // Turn display
    auto turn_display = (new Frame(Vec2i(0, 0), Vec2i(73, 73), game_header_turn.GetTexture()))
        ->SetName("Turn", false)
        ->AddChildren({ turn_info_frame });

    // Profile picutre
    picture = (Frame*)(new Frame(Vec2i(3, 3), Vec2i(60, 60), texture_profile_picture))
        ->SetName("Picture");

    // Game info text1
    text_title = (Frame*)(new Frame(Vec2i(0, 0), Vec2i(texture_title->GetSize()), texture_title))
        ->SetName("GuessingText", false);

    // Game info text2
    text_description = (Frame*)(new Frame(Vec2i(0, 0), Vec2i(texture_description->GetSize()), texture_description))
        ->SetName("DrawingText", false);

    // Game info text frame
    auto round_info_frame = (new Frame(Vec2i(3, 0), Vec2i(50, 30), DONT_DRAW))
        ->SetFlex(FLEX_HEIGHT)
        ->SetAlign(DONT_ALIGN, ALIGN_CENTER)
        ->SetAdaptive(true, true)
        ->SetName("RoundInfoText")
        ->AddChildren({ text_title, text_description });

    // Game info
    auto round_info = (new Frame(Vec2i(0, 0), Vec2i(0, 73), game_header_background.GetTexture()))
        ->SetFlex(FLEX_WIDTH, 7)
        ->SetOccupy(true, false)
        ->SetName("RoundInfo")
        ->AddChildren({ picture, round_info_frame });

    // Header
    SetFullyOccupy(true, false);
    SetFlex(FLEX_WIDTH);
    SetName("Header", false);
    AddChildren({ turn_display, round_info });
}

IngameHeader::~IngameHeader() {
    delete texture_title;
    delete texture_description;
    delete texture_turn;
    delete texture_turn_number;
}

void IngameHeader::SetTitle(const std::string& new_title) {
    auto assets = Assets::Get();
    delete texture_title;
    texture_title = assets->RenderTextBlended(sFontDefaultSmaller.GetFont()->TTFFont(),
                                              new_title,
                                              { 0, 0, 0, 255 });
    if (text_title) {
        text_title->visual_texture = texture_title;
        text_title->size = Vec2i(texture_title->GetSize());
        text_title->visual_size = Vec2i(texture_title->GetSize());
    }
}

void IngameHeader::SetDescription(const std::string& new_description) {
    auto assets = Assets::Get();
    delete texture_description;
    texture_description = assets->RenderTextBlended(sFontDefaultSmaller.GetFont()->TTFFont(),
                                                    new_description,
                                                    { 0, 0, 0, 255 });
    if (text_description) {
        text_description->visual_texture = texture_description;
        text_description->size = Vec2i(texture_description->GetSize());
        text_description->visual_size = Vec2i(texture_description->GetSize());
    }
}

void IngameHeader::SetTurnNumber(int turn_number) {
    auto assets = Assets::Get();
    delete texture_turn_number;
    texture_turn_number = assets->RenderTextBlended(sFontDefaultBigger.GetFont()->TTFFont(),
                                                    Strings::FString("%d", turn_number),
                                                    { 255, 255, 255, 255 });
    if (text_turn_number) {
        text_turn_number->visual_texture = texture_turn_number;
        text_turn_number->size = Vec2i(texture_turn_number->GetSize());
        text_turn_number->visual_size = Vec2i(texture_turn_number->GetSize());
    }
}

void IngameHeader::SetProfilePicture(Texture* profile_picture) {
    texture_profile_picture = profile_picture;
}
