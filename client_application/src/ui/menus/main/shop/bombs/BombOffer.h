//
// Created by Matq on 15/06/2025.
//

#pragma once

#include "ui/components/element/Frame.h"
#include "ui/components/element/Button.h"
#include "ui/components/element/TextElement.h"

class BombOffer : public Frame
{
private:
	int display_id;
	int price;

	Button *buy_button;
	TextElement *price_text;

public:
	BombOffer(int display_id);
	~BombOffer();

	// Manipulating
	void UpdatePrice();

};
