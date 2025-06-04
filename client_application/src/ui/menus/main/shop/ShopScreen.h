//
// Created by Matq on 28/04/2025.
//

#pragma once

#include "network/NetworkClient.h"
#include "ui/menus/ScreenMenu.h"

class ShopScreen : public ScreenMenu
{
private:
	Frame* colors_frame;


public:
	ShopScreen();
	~ShopScreen() override;

	// Manipulating
	void ParseFromJson(const json& shop_data);
	void UpdateOwnedBundles();

};
