//
// Created by Matq on 28/04/2025.
//

#pragma once

#include "network/NetworkClient.h"
#include "ui/menus/ScreenMenu.h"
#include "ui/components/element/ScrollFrame.h"
#include "ui/menus/main/shop/bombs/BombOffer.h"

class ShopScreen : public ScreenMenu
{
private:
	BombOffer *bombs1;
	BombOffer *bombs2;
	BombOffer *bombs3;
	ScrollFrame* colors_frame;

public:
	ShopScreen();
	~ShopScreen() override;

	// Manipulating
	void ParseFromJson(const json& shop_data);
	void UpdateOwnedBundles();
	void UpdateBombPrices();

};
