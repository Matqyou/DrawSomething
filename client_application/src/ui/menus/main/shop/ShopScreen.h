//
// Created by Matq on 28/04/2025.
//

#pragma once

#include "ui/components/element/Frame.h"
#include "network/NetworkClient.h"

class ShopScreen : public Frame
{
private:
	Frame* colors_frame;


public:
	ShopScreen();
	~ShopScreen() override;

	// Manipulating
	void ParseFromJson(const json& shop_data);

};
