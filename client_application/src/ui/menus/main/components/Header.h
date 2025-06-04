//
// Created by Matq on 26/01/2025.
//

#pragma once

#include "ui/components/element/Frame.h"
#include "SettingsScreen.h"
#include "ui/components/element/TextElement.h"
#include "ui/components/element/Button.h"
#include "ui/menus/main/admin/AdminScreen.h"
#include "ui/menus/main/shop/ShopScreen.h"

class MainMenu;
namespace Main
{
class Header : public Frame
{
private:
	TextElement *num_coins;
	TextElement *num_bombs;

//	Button *more_coins_button;
	Button *shop_button;
	Button *admin_button;
	Button *games_button;
	Button *profile_button;

public:
	explicit Header(MainMenu* parent);
	~Header() override;

	// Manipulating
	void RefreshData();

};
}
