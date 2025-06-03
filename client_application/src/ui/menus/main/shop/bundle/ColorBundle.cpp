//
// Created by Matq on 21/05/2025.
//

#include "ColorBundle.h"
#include "ui/components/element/TextElement.h"
#include "ui/CommonUI.h"
#include "ColorView.h"

void ColorBundle::UpdateRarity()
{
	Texture *background = Assets::Get()->GetTexture(Strings::FString("main_menu.shop.color_background.%d", rarity));
	SetTexture(background);
	SetDraw(DRAW_TEXTURE);
	SetSize(Vec2i(background->GetOriginalSize() / 2));
}

void ColorBundle::UpdateBundleName()
{
	bundle_name_text->UpdateText(CommonUI::sFontRegular.GetTTFFont(),
								 bundle_name.c_str(),
								 { 255, 255, 255, 255 });
}

void ColorBundle::UpdatePrice()
{
	bundle_price_text->UpdateText(CommonUI::sFontSmallerBold.GetTTFFont(),
								  Strings::FString("%d Coins", price).c_str(),
								  { 255, 255, 255, 255 });
}

ColorBundle::ColorBundle()
	: Frame()
{
	this->rarity = 1;
	this->bundle_name = "No name provided";
	this->price = -1;

	colors = (Frame *)(new Frame())
		->SetRelative(Vec2i(8, 3))
		->SetAlign(Align::CENTER, Align::DONT)
		->SetFlex(Flex::WIDTH, 5)
		->SetAdaptive(true, true)
		->SetName("Colors");

	bundle_name_text = (TextElement *)(new TextElement())
		->UpdateText(CommonUI::sFontRegular.GetTTFFont(),
					 "No name provided",
					 { 255, 255, 255, 255 })
		->SetRelative(Vec2i(8, 3))
		->SetAlign(Align::CENTER, Align::DONT)
		->SetName("Name");

	bundle_price_text = (TextElement *)(new TextElement())
		->UpdateText(CommonUI::sFontSmallerBold.GetTTFFont(),
					 "No price provided",
					 { 255, 255, 255, 255 })
		->SetAlign(Align::CENTER, Align::CENTER)
		->SetName("Price");

	auto left = (Frame *)(new Frame())
		->SetFlex(Flex::HEIGHT)
		->SetOccupy(true, false)
		->SetFullyOccupy(false, true)
		->SetName("Left")
		->AddChildren({ bundle_name_text, colors });

	auto right = (Frame *)(new Frame())
		->SetSize(Vec2i(120, 0))
		->SetFullyOccupy(false, true)
		->SetFlex(Flex::HEIGHT)
		->SetName("Right")
		->AddChildren({ bundle_price_text });

	UpdateRarity();
	UpdateBundleName();
	UpdatePrice();

	SetAlign(Align::CENTER, Align::DONT);
	SetFlex(Flex::WIDTH);
	SetName("ColorBundle");
	AddChildren({ left, right });
}

ColorBundle::~ColorBundle()
{

}

// Modify the color bundles object based on incoming json data
ColorBundle *ColorBundle::ParseFromJson(const json& bundle_data)
{
	// Get commonly found data about bundles
	rarity = bundle_data.value("rarity", 1);
	bundle_name = bundle_data.value("name", "No name provided");
	price = bundle_data.value("price", -1);

	// Remove old ui elements resembling color bundles
	for (auto color : colors->children)
		delete color;
	colors->children.clear();

	// Create new ui elements resembling color bundles
	std::vector<Element *> vColors;
	if (bundle_data.contains("colors") && bundle_data["colors"].is_array()) // Check if colors are in our json
	{
		for (const json& color_data : bundle_data["colors"])
		{
			if (color_data.is_object())
			{
				// Get commonly found data about individual colors
				int color_id = color_data.value("color_id", -1);
				int color_rarity = color_data.value("rarity", 1);
				int r = color_data.value("r", 1);
				int g = color_data.value("g", 1);
				int b = color_data.value("b", 1);
				colors->AddChildren({ new ColorView(color_id, color_rarity, r, g, b) });
			}
		}
	}

	// Update the UI element for the current color bundle
	UpdateRarity();
	UpdateBundleName();
	UpdatePrice();

	// Return the object for other options
	return this;
}