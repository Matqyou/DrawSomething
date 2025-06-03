//
// Created by Matq on 21/05/2025.
//

#pragma once

#include "ui/components/element/Frame.h"
#include "network/NetworkClient.h"
#include "ui/components/element/TextElement.h"

class ColorBundle : public Frame
{
private:
	int rarity;
	std::string bundle_name;
	int price;

	Frame *colors;
	TextElement *bundle_name_text;
	TextElement *bundle_price_text;

	// Manipulating
	void UpdateRarity();
	void UpdateBundleName();
	void UpdatePrice();

public:
	ColorBundle();
	~ColorBundle() override;

	// Options
	ColorBundle *ParseFromJson(const json& bundle_data);

};
