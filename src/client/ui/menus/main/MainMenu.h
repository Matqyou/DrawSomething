//
// Created by Matq on 24/12/2024.
//

#pragma once

#include "../FullscreenMenu.h"
#include "../../structures/window_texture/WindowTexture.h"
#include "client/ui/menus/main/components/games/CurrentGames.h"
#include "components/SettingsScreen.h"
#include "components/Header.h"
#include "components/Profile.h"
#include "client/ui/menus/main/admin/AdminScreen.h"
#include "client/ui/menus/main/shop/ShopScreen.h"

class MainMenu : public FullscreenMenu
{
private:
	Main::Header *header;
	Main::CurrentGames *games;
	Main::Profile *profile;

	ShopScreen *shop_menu;
	AdminScreen *admin_menu;
	LoadingScreen *loading_screen;

public:
	MainMenu();

	// Sense
	[[nodiscard]] Main::Header *Header() const { return header; }
	[[nodiscard]] Main::CurrentGames *Games() const { return games; }
	[[nodiscard]] Main::Profile *Profile() const { return profile; }
	[[nodiscard]] AdminScreen *GetAdminScreen() const { return admin_menu; }
	[[nodiscard]] ShopScreen *GetShopScreen() const { return shop_menu; }
	[[nodiscard]] LoadingScreen *GetLoadingScreen() const { return loading_screen; }

};
