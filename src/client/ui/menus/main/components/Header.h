//
// Created by Matq on 26/01/2025.
//

#pragma once

#include "../../../components/element/Frame.h"
#include "SettingsScreen.h"
#include "../../../components/element/TextElement.h"
#include "../../../components/element/Button.h"
#include "client/ui/menus/main/admin/AdminScreen.h"
#include "client/ui/menus/main/shop/ShopScreen.h"

namespace Main
{
class Header : public Frame
{
private:
	TextElement *num_coins;

	Button *more_coins_button;
	Button *shop_button;
	Button *admin_button;

public:
	explicit Header(SettingsScreen *settings, ShopScreen *shop_menu, AdminScreen *admin_menu);
	~Header() override;

	// Manipulating
	void RefreshData();

};
}
