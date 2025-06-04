//
// Created by Matq on 28/01/2025.
//

#include "SettingsScreen.h"
#include "ui/components/element/TextElement.h"
#include "ui/CommonUI.h"
#include "ui/components/element/Button.h"

namespace Main {
static LinkTexture sTextureCard("main_menu.settings.card");
static LinkTexture sTextureClose("main_menu.settings.x");
static LinkTexture sTextureClosePressed("main_menu.settings.x_pressed");
static LinkTexture sTextureSliderOff("main_menu.settings.slider_off");

SettingsScreen::SettingsScreen()
    : ScreenMenu() {

    auto close_button = (Button*)(new Button(sTextureClose.GetTexture(),
                                             sTextureClosePressed.GetTexture()))
        ->SetSize(Vec2i(32, 32))
        ->SetColor(150, 0, 0, 255)
        ->SetAlign(Align::RIGHT, Align::TOP)
        ->SetFlexInvolved(false, false)
        ->SetName("CloseButton");
    close_button->SetCallback([this]() {
        this->SetEnabled(false);
    });

    auto title = (new TextElement())
        ->UpdateText(CommonUI::sFontBigger.GetTTFFont(), "Game Settings", { 255, 255, 255, 255 })
		->SetRelative(Vec2i(3, 0))
        ->SetName("Title");

    auto sounds_name = (new TextElement())
        ->UpdateText(CommonUI::sFontSmallerBold.GetTTFFont(), "Sound effects", { 255, 255, 255, 255 })
		->SetAlign(Align::DONT, Align::CENTER)
        ->SetName("SettingTitle");
    auto sounds_button = (new Button())
        ->SetSize(Vec2i(44, 32))
		->SetTexture(sTextureSliderOff.GetTexture())
        ->SetDraw(DRAW_TEXTURE)
        ->SetColor(0, 150, 0, 255)
        ->SetAlign(Align::RIGHT, Align::CENTER)
        ->SetName("SettingButton");
    auto sounds_frame = (new Frame())
		->SetSize(Vec2i(390, 0))
		->SetAlign(Align::CENTER, Align::DONT)
        ->SetAdaptive(false, true)
        ->SetName("Setting1")
        ->AddChildren({ sounds_name, sounds_button });

	auto particles_name = (new TextElement())
		->UpdateText(CommonUI::sFontSmallerBold.GetTTFFont(), "Particle effects", { 255, 255, 255, 255 })
		->SetAlign(Align::DONT, Align::CENTER)
		->SetName("SettingTitle");
	auto particles_button = (new Button())
		->SetSize(Vec2i(44, 32))
		->SetTexture(sTextureSliderOff.GetTexture())
		->SetDraw(DRAW_TEXTURE)
		->SetColor(0, 150, 0, 255)
		->SetAlign(Align::RIGHT, Align::CENTER)
		->SetName("SettingButton");
	auto particles_frame = (new Frame())
		->SetSize(Vec2i(390, 0))
		->SetAlign(Align::CENTER, Align::DONT)
		->SetAdaptive(false, true)
		->SetName("Setting1")
		->AddChildren({ particles_name, particles_button });

	auto settings_frame = (new Frame())
		->SetRelative(Vec2i(0, 12))
		->SetSize(Vec2i(403, 0))
		->SetAlign(Align::CENTER, Align::DONT)
		->SetFlex(Flex::HEIGHT, 5)
		->SetAdaptive(false, true)
		->SetName("SettingsFrame")
		->AddChildren({ close_button, title, sounds_frame, particles_frame });

    auto settings_card = (new Frame())
        ->SetSize(Vec2i(427, 309))
		->SetTexture(sTextureCard.GetTexture())
        ->SetDraw(DRAW_TEXTURE)
        ->SetAlign(Align::CENTER, Align::CENTER)
        ->SetName("SettingsFrame")
        ->AddChildren({ settings_frame });

	this->SetEnabled(false);
	this->SetColor(0, 0, 0, 200);
	this->SetDraw(DRAW_RECT);
	this->SetFullyOccupy(true, true);
	this->SetName("SettingsMenu");
	this->AddChildren({ settings_card });
}

SettingsScreen::~SettingsScreen() {

}
}
