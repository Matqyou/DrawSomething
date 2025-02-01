//
// Created by Matq on 28/01/2025.
//

#include "Settings.h"
#include "../../../components/element/TextElement.h"
#include "../../../CommonUI.h"
#include "../../../components/element/Button.h"

namespace Main {
LinkTexture sTextureClose("main_menu.settings.x");
LinkTexture sTextureClosePressed("main_menu.settings.x_pressed");

Settings::Settings()
    : FullscreenMenu() {

    auto close_button = (Button*)(new Button(Vec2i(-8, 8),
                                             Vec2i(32, 32),
                                             VisualTexture(sTextureClose.GetSDLTexture()),
                                             VisualTexture(sTextureClosePressed.GetSDLTexture())))
        ->SetColor(150, 0, 0, 255)
        ->SetAlign(Align::RIGHT, Align::TOP)
        ->SetFlexInvolved(false, false)
        ->SetName("CloseButton");
    close_button->SetCallback([this]() {
        this->SetEnabled(false);
    });

    auto title = (new TextElement(Vec2i(0, 0)))
        ->UpdateText(CommonUI::sFontBigger.GetTTFFont(), "Account Settings", { 255, 255, 255, 255 })
//        ->SetAlign(Align::CENTER, Align::DONT)
        ->SetName("Title");

    auto setting_name1 = (new TextElement(Vec2i(0, 0)))
        ->UpdateText(CommonUI::sFontDefault.GetTTFFont(), "Sound", { 255, 255, 255, 255 })
        ->SetName("SettingTitle");

    auto setting_button1 = (new Button(Vec2i(0, 0),
                                       Vec2i(44, 32),
                                       DRAW_RECT))
        ->SetColor(0, 150, 0, 255)
        ->SetAlign(Align::DONT, Align::CENTER)
        ->SetName("SettingButton");

    auto setting_frame1 = (new Frame())
        ->SetAdaptive(true, true)
        ->SetFlex(Flex::WIDTH)
        ->SetName("Setting1")
        ->AddChildren({ setting_name1, setting_button1 });

    auto settings_frame = (new Frame(Vec2i(0, 0),
                                     Vec2i(400, 0),
                                     DRAW_RECT))
        ->SetAdaptive(false, true)
        ->SetFlex(Flex::HEIGHT)
        ->SetAlign(Align::CENTER, Align::CENTER)
        ->SetName("SettingsFrame")
        ->AddChildren({ close_button, title, setting_frame1 });

    SetEnabled(false);
    SetColor(0, 0, 0, 150);
    SetFlexInvolved(false, false);
    SetFullyOccupy(true, true);
    SetName("Settings");
    AddChildren({ settings_frame });
}

Settings::~Settings() {

}
}
