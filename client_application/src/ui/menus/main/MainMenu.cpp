//
// Created by Matq on 24/12/2024.
//

#include "MainMenu.h"
#include "ui/menus/confirmation/ConfirmationScreen.h"
#include "ui/menus/loading/LoadingScreen.h"
#include "ui/menus/main/components/Connections.h"
#include "ui/menus/auth/components/ScrollingBackground.h"
#include "game/GameData.h"
#include "ui/menus/auth/AuthMenu.h"
#include "ui/CommonUI.h"
#include "ui/RenderPresets.h"

static LinkTexture sTextureDoodles("doodles"); //

void MainMenu::ReassignChildren()
{
	this->SetChildren({ Centralized.main_header, content, loading_screen, confirmation_screen });
}

MainMenu::MainMenu()
	: HeaderFullscreenMenu()
{
	name = L"MainMenu";

	profile_menu = new ProfileScreen();
	shop_menu = new ShopScreen();
	admin_menu = new AdminScreen();
	games_menu = new GamesScreen();
	settings_menu = new Main::SettingsScreen();

	loading_screen = new LoadingScreen();
	confirmation_screen = new ConfirmationScreen();

//	header = new MainHeader(this);
	games = new Main::CurrentGames(confirmation_screen, loading_screen);
	profile = new Main::Profile();
//    auto friends = new Main::Connections();

	// Background
	auto background = (new Auth::ScrollingBackground())
		->SetFullyOccupy(true, true)
		->SetFlexInvolved(false, false)
		->SetTexture(sTextureDoodles.GetTexture()->SetAlphaMod(75))
		->SetName("Background");

	// Content
	content = (Frame*)(new Frame())
		->SetFullyOccupy(true, false)
		->SetOccupy(false, true)
		->AddChildren({ background, games, profile, admin_menu, games_menu, shop_menu, profile_menu, settings_menu })
		->SetName("Content");

	this->SetColor(0, 211, 112, 255);
	this->SetFlex(Flex::HEIGHT);
	this->SetName("MainMenu");
	this->RefreshMenu();
}
