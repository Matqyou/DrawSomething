//
// Created by Matq on 24/12/2024.
//

#pragma once

#include "ui/menus/FullscreenMenu.h"
#include "ui/structures/window_texture/WindowTexture.h"
#include "ui/menus/main/components/games/CurrentGames.h"
#include "ui/menus/main/components/SettingsScreen.h"
#include "ui/menus/main/components/MainHeader.h"
#include "ui/menus/main/components/Profile.h"
#include "ui/menus/main/admin/AdminScreen.h"
#include "ui/menus/main/shop/ShopScreen.h"
#include "ui/menus/main/profile/ProfileScreen.h"
#include "ui/menus/main/games/GamesScreen.h"
#include "ui/menus/HeaderFullscreenMenu.h"

class MainMenu : public HeaderFullscreenMenu
{
private:
	Frame *content;

	Main::CurrentGames *games;
	Main::Profile *profile;

	ProfileScreen *profile_menu;
	ShopScreen *shop_menu;
	AdminScreen *admin_menu;
	GamesScreen *games_menu;
	Main::SettingsScreen *settings_menu;

	LoadingScreen *loading_screen;
	ConfirmationScreen *confirmation_screen;

	void ReassignChildren() override;

public:
	MainMenu();

	// Sense
	[[nodiscard]] Main::CurrentGames *Games() const { return games; }
	[[nodiscard]] Main::Profile *Profile() const { return profile; }
	[[nodiscard]] AdminScreen *GetAdminScreen() const { return admin_menu; }
	[[nodiscard]] GamesScreen *GetGamesScreen() const { return games_menu; }
	[[nodiscard]] ShopScreen *GetShopScreen() const { return shop_menu; }
	[[nodiscard]] ProfileScreen *GetProfileScreen() const { return profile_menu; }
	[[nodiscard]] Main::SettingsScreen *GetSettingsScreen() const { return settings_menu; }
	[[nodiscard]] LoadingScreen *GetLoadingScreen() const { return loading_screen; }
	[[nodiscard]] ConfirmationScreen *GetConfirmationScreen() const { return confirmation_screen; }

};
