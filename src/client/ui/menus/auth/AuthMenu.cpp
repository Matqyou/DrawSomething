//
// Created by Matq on 16/01/2025.
//

#include "AuthMenu.h"
#include "../../../core/Application.h"
#include "../../components/element/TextBox.h"
#include "../../structures/window_texture/WindowTexture.h"
#include "../../components/element/Button.h"

LinkFont sFontAuth("fredoka.small");
LinkFont sFontDefaultBiggest("fredoka.biggest");
//LinkFont IngameMenu::sFontDefaultBigger("fredoka.big");
//LinkFont IngameMenu::sFontDefaultSmaller("fredoka.small");

static auto sCallbackHeader = [](Texture* texture) {
    texture->SetColorMod(100, 190, 255);
};

PreloadTexture texture_header("header", sCallbackHeader);

AuthMenu::AuthMenu()
    : FullscreenMenu(),
      temp_field(&temp_sheet, 18, 18, 18, 18),
      temp_button(&temp_sheet2, 18, 18, 18, 18) {
    auto assets = Assets::Get();

    // Header
    auto header = (new Frame(Vec2i(0, 0),
                             Vec2i(0, 58),
                             Vec2i(0, 80),
                             Vec2i(0, 0),
                             texture_header.GetTexture()))
        ->SetFullyOccupy(true, false)
        ->SetName("Header", false);

    temp_sheet.Generate(assets->GetTexture("window.sheet2"));
    temp_field.Generate(Vec2f(300, 50));

    temp_sheet2.Generate(assets->GetTexture("window.sheet3"));
    temp_button.Generate(Vec2f(200, 50));

    auto texture_title = assets->RenderTextBlended(sFontDefaultBiggest.GetFont()->TTFFont(),
                                                   "Login Boi",
                                                   { 0, 0, 0, 255 })
        ->FlagForAutomaticDeletion();

    // Title
    auto title = (Frame*)(new Frame(Vec2i(0, 0),
                                    Vec2i(texture_title->GetSize()),
                                    texture_title))
        ->SetAlign(Align::CENTER, Align::DONT)
        ->SetName("Title", false);

    // Test field
    auto test_field = (new TextBox(Vec2i(0, 0),
                                   Vec2i(300, 50),
                                   Vec2i(300, 50),
                                   Vec2i(0, 0),
                                   temp_field.GetGenerated()))
        ->SetCallback([](std::string& text) {
            std::wcout << Strings::FStringColorsW(L"Callback: %s\n", text.c_str());
        })
        ->SetTextAlign(SimpleAlign::CENTER, SimpleAlign::CENTER)
        ->SetPlaceholder("Username")
        ->SetTextColor({ 255, 255, 255 })
        ->SetFont(sFontAuth.GetFont()->TTFFont())
        ->SetColor(255, 255, 255, 255)
        ->SetName("Field", false);

    // Test field2
    auto test_field2 = (new TextBox(Vec2i(0, 0),
                                    Vec2i(300, 50),
                                    Vec2i(300, 50),
                                    Vec2i(0, 0),
                                    temp_field.GetGenerated()))
        ->SetCallback([](std::string& text) {
            std::wcout << Strings::FStringColorsW(L"Callback: %s\n", text.c_str());
        })
        ->SetTextAlign(SimpleAlign::CENTER, SimpleAlign::CENTER)
        ->SetPlaceholder("Password")
        ->SetTextColor({ 255, 255, 255 })
        ->SetFont(sFontAuth.GetFont()->TTFFont())
        ->SetColor(255, 255, 255, 255)
        ->SetName("Field", false);

    // Auth frame
    auto auth_frame = (new Frame(Vec2i(0, 0),
                                 Vec2i(0, 58),
                                 DONT_DRAW))
        ->SetFlex(Flex::HEIGHT, 5)
        ->SetAlign(Align::CENTER, Align::CENTER)
        ->SetAdaptive(true, true)
        ->SetName("Auth");

    // Test button
    auto test_button = (Button*)(new Button(Vec2i(0, 0),
                                            Vec2i(200, 50),
                                            Vec2i(200, 50),
                                            Vec2i(0, 0),
                                            temp_button.GetGenerated()))
        ->SetAlign(Align::CENTER, Align::DONT)
        ->SetName("Test");
    test_button->SetCallback([auth_frame, test_button]() { auth_frame->SetFocus(test_button); });
    auth_frame->AddChildren({ title, test_field, test_field2, test_button });

    // Content
    auto content = (new Frame(Vec2i(0, 0),
                              Vec2i(0, 0),
                              DONT_DRAW))
        ->SetFullyOccupy(true, false)
        ->SetOccupy(false, true)
        ->AddChildren({ auth_frame })
        ->SetName("Content");

    SetColor(94, 152, 224, 255);
    SetFlex(Flex::HEIGHT);
    AddChildren({ header, content });
    SetName("GuessingMenu", false);
    Refresh();
}

AuthMenu::~AuthMenu() {

}
