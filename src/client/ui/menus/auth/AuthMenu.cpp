//
// Created by Matq on 16/01/2025.
//

#include "AuthMenu.h"
#include "../../../core/Application.h"
#include "../../components/element/TextBox.h"
#include "../../structures/window_texture/WindowTexture.h"
#include "../../components/element/Button.h"
#include "../../CommonUI.h"

namespace Auth {
static auto sCallbackHeader = [](TextureData* texture) {
    texture->SetColorMod(100, 190, 255);
};

static LinkTexture sTextureHeader("header", sCallbackHeader);

}

AuthMenu::AuthMenu()
    : FullscreenMenu(),
      temp_field(&temp_sheet, 18, 18, 18, 18),
      temp_button(&temp_sheet2, 18, 18, 18, 18) {
    name = L"AuthMenu";
    auto assets = Assets::Get();

    // Header
    auto header = (new Frame(Vec2i(0, 0),
                             Vec2i(0, 58),
                             Vec2i(0, 80),
                             Vec2i(0, 0),
                             Auth::sTextureHeader.GetSDLTexture()))
        ->SetFullyOccupy(true, false)
        ->SetName("Header");

    temp_sheet.Generate(assets, assets->GetTexture("window.sheet2"));
    temp_field.Generate(assets, Vec2f(300, 50));

    temp_sheet2.Generate(assets, assets->GetTexture("window.sheet3"));
    temp_button.Generate(assets, Vec2f(200, 50));

    auto texture_title = assets->RenderTextBlended(CommonUI::sFontBiggest.GetFont()->TTFFont(),
                                                   "Login Boi",
                                                   { 0, 0, 0, 255 })
        ->FlagForAutomaticDeletion();

    // Title
    auto title = (Frame*)(new Frame(Vec2i(0, 0),
                                    Vec2i(texture_title->GetSize()),
                                    texture_title->SDLTexture()))
        ->SetAlign(Align::CENTER, Align::DONT)
        ->SetName("Title");

    // Test field
    auto test_field = (new TextBox(Vec2i(0, 0),
                                   Vec2i(300, 50),
                                   Vec2i(300, 50),
                                   Vec2i(0, 0),
                                   temp_field.GetGenerated()->SDLTexture()))
        ->SetCallback([](std::string& text) {
            std::wcout << Strings::FStringColorsW(L"Callback: %s\n", text.c_str());
        })
        ->SetTextAlign(SimpleAlign::CENTER, SimpleAlign::CENTER)
        ->SetPlaceholder("Username")
        ->SetTextColor({ 255, 255, 255 })
        ->SetFont(CommonUI::sFontAuth.GetFont()->TTFFont())
        ->SetColor(255, 255, 255, 255)
        ->SetName("Field");

    // Test field2
    auto test_field2 = (new TextBox(Vec2i(0, 0),
                                    Vec2i(300, 50),
                                    Vec2i(300, 50),
                                    Vec2i(0, 0),
                                    temp_field.GetGenerated()->SDLTexture()))
        ->SetCallback([](std::string& text) {
            std::wcout << Strings::FStringColorsW(L"Callback: %s\n", text.c_str());
        })
        ->SetTextAlign(SimpleAlign::CENTER, SimpleAlign::CENTER)
        ->SetPlaceholder("Password")
        ->SetTextColor({ 255, 255, 255 })
        ->SetFont(CommonUI::sFontAuth.GetFont()->TTFFont())
        ->SetColor(255, 255, 255, 255)
        ->SetName("Field");

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
                                            VisualTexture(temp_button.GetGenerated()->SDLTexture(), Vec2d(1.0, 1.0), Vec2d(0.0, 0.0)),
                                            VisualTexture(nullptr, Vec2d(1.0, 1.0), Vec2d(0.0, 0.0))))
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
    SetName("GuessingMenu");
    RefreshMenu();
}

AuthMenu::~AuthMenu() {

}
