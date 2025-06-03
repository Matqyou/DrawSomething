//
// Created by Matq on 24/12/2024.
//

#include "MainMenu.h"
#include "../confirmation/ConfirmationScreen.h"
#include "../loading/LoadingScreen.h"
#include "components/Connections.h"
#include "../auth/components/ScrollingBackground.h"

static LinkTexture sTextureDoodles("doodles"); //

MainMenu::MainMenu()
    : FullscreenMenu() {
    name = L"MainMenu";

    loading_screen = new LoadingScreen();
	admin_menu = new AdminScreen();
	shop_menu = new ShopScreen();

    auto confirmation = new ConfirmationScreen();
    auto settings = new Main::SettingsScreen();
    header = new Main::Header(settings, shop_menu, admin_menu);
    games = new Main::CurrentGames(confirmation, loading_screen);
    profile = new Main::Profile();
//    auto friends = new Main::Connections();

	// Background
	auto background = (new Auth::ScrollingBackground())
		->SetFullyOccupy(true, true)
		->SetFlexInvolved(false, false)
		->SetTexture(sTextureDoodles.GetTexture()->SetAlphaMod(75))
		->SetName("Background");

	// Content
	auto content = (new Frame())
		->SetFullyOccupy(true, false)
		->SetOccupy(false, true)
		->AddChildren({ background, games, profile, admin_menu, shop_menu, settings })
		->SetName("Content");

    this->SetColor(0, 211, 112, 255);
	this->SetFlex(Flex::HEIGHT);
    this->SetName("MainMenu");
    this->AddChildren({ header, content, loading_screen, confirmation });
    this->RefreshMenu();
}
