//
// Created by Matq on 16/01/2025.
//

#include "AuthMenu.h"
#include "../../core/Application.h"
#include "../components/element/TextBox.h"

LinkFont sFontDefault("default");
//LinkFont IngameMenu::sFontDefaultBiggest("fredoka.biggest");
//LinkFont IngameMenu::sFontDefaultBigger("fredoka.big");
//LinkFont IngameMenu::sFontDefaultSmaller("fredoka.small");

static auto sCallbackHeader = [](Texture* texture) {
    texture->SetColorMod(100, 190, 255);
};

PreloadTexture texture_header("header", sCallbackHeader);

AuthMenu::AuthMenu()
    : FullscreenMenu() {

    // Header
    auto header = (new Frame(Vec2i(0, 0),
                             Vec2i(0, 58),
                             Vec2i(0, 80),
                             Vec2i(0, 0),
                             texture_header.GetTexture()))
        ->SetFullyOccupy(true, false)
        ->SetName("Header", false);

    // Test field
    auto test_field = (new TextBox(Vec2i(0, 0),
                                   Vec2i(300, 50)))
        ->SetCallback([](std::string& text) {
            std::wcout << Strings::FStringColorsW(L"Callback: %s", text.c_str()) << std::endl;
        })
        ->SetFont(sFontDefault.GetFont()->TTFFont())
        ->SetColor(255, 255, 255, 255)
        ->SetName("Field", false);

    // Test field2
    auto test_field2 = (new TextBox(Vec2i(0, 0),
                                    Vec2i(300, 50)))
        ->SetCallback([](std::string& text) {
            std::wcout << Strings::FStringColorsW(L"Callback: %s", text.c_str()) << std::endl;
        })
        ->SetFont(sFontDefault.GetFont()->TTFFont())
        ->SetColor(255, 255, 255, 255)
        ->SetName("Field", false);

    // Auth frame
    auto auth_frame = (new Frame(Vec2i(0, 0),
                                 Vec2i(0, 58),
                                 DONT_DRAW))
        ->SetFlex(FLEX_HEIGHT, 5)
        ->SetAlign(ALIGN_CENTER, ALIGN_BOTTOM)
        ->SetAdaptive(true, true)
        ->AddChildren({ test_field, test_field2 })
        ->SetName("Auth");

    // Content
    auto content = (new Frame(Vec2i(0, 0),
                              Vec2i(0, 0),
                              DONT_DRAW))
        ->SetFullyOccupy(true, false)
        ->SetOccupy(false, true)
        ->AddChildren({ auth_frame })
        ->SetName("Content");

    SetColor(94, 152, 224, 255);
    SetFlex(FLEX_HEIGHT);
    AddChildren({ header, content });
    SetName("GuessingMenu", false);
    Refresh();
}
