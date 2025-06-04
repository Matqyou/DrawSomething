//
// Created by Matq on 21/05/2025.
//

#pragma once

#include "ui/components/element/Frame.h"
#include "network/NetworkClient.h"
#include "ui/components/element/TextElement.h"
#include "ui/components/element/Button.h"

class ColorBundle : public Frame
{
private:
	int bundle_id;
	int rarity;
	std::string bundle_name;
	int price;

	Frame *colors;
	TextElement *bundle_name_text;
	TextElement *bundle_price_text;

	Button *buy_button;
	TextElement *owned_text;

public:
	ColorBundle();
	~ColorBundle() override;

	// Options
	ColorBundle *ParseFromJson(const json& bundle_data);

	// Manipulating
	void UpdateRarity();
	void UpdateBundleName();
	void UpdatePrice();

};
