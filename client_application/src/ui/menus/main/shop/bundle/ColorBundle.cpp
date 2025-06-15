//
// Created by Matq on 21/05/2025.
//

#include "ColorBundle.h"
#include "ui/components/element/TextElement.h"
#include "ui/CommonUI.h"
#include "ColorView.h"
#include "ui/RenderPresets.h"
#include "ui/components/element/Button.h"
#include "game/GameData.h"
#include "ui/menus/main/MainMenu.h"

static LinkTexture sTextureButton("button");
static LinkTexture sTextureButtonOutline("button_outline");

static SDL_Color RarityColor[] = {
	SDL_Color(125, 150, 125),
	SDL_Color(100, 200, 100),
	SDL_Color(200, 200, 100),
};

void ColorBundle::UpdateRarity()
{
	auto assets = Assets::Get();
	auto content_texture = assets->GetTexture("main_menu.shop.color_background.content");
	auto outline_texture = assets->GetTexture("main_menu.shop.color_background.outline");
	auto background = RenderPresets::Composition({ content_texture, outline_texture }, { RarityColor[rarity - 1] });

	SetTexture(background);
	SetDraw(DRAW_TEXTURE);
	ResizeToTexture();
}

void ColorBundle::UpdateBundleName()
{
	bundle_name_text->UpdateTextOutline(CommonUI::sFontMiniProfile2.GetTTFFont(), bundle_name.c_str(), 2,
										{ 0, 0, 0, 255 }, { 255, 255, 255, 255 });
}

void ColorBundle::UpdatePrice()
{
	auto& account = Centralized.GetAccount();
	bool already_owned = std::find(account.color_bundles.begin(), account.color_bundles.end(), bundle_id) != account.color_bundles.end();
	bool enough_money = already_owned || Centralized.GetAccount().coins >= price;
	bool can_purchase_now = enough_money && !already_owned;
	bundle_price_text->UpdateTextOutline(CommonUI::sFontSmallerBold.GetTTFFont(), Strings::FString("%d Coins", price).c_str(), 2,
										 enough_money ? SDL_Color(255, 255, 255, 255) : SDL_Color(255, 0, 0, 255),
										 { 0, 0, 0, 255 });
	bundle_price_text->SetEnabled(!already_owned);
	buy_button->SetEnabled(can_purchase_now);
	owned_text->SetEnabled(already_owned);

	if (can_purchase_now)
	{
		buy_button->SetCallback([this]()
								{
									json buy_data;
									buy_data["bundle_id"] = bundle_id;

									auto packet = new Packet("/buy_colors", "POST", &buy_data);
									packet->SetResponseCallback([packet](const NetworkResponse& server_response)
																{
																	if (server_response.response_json.contains("user") && server_response.response_json["user"].is_object())
																	{
																		json user_data = server_response.response_json["user"];
																		auto account = Centralized.GetAccount().ParseFromJson(user_data);

																		Centralized.main_header->RefreshData();
																		Centralized.main_menu->Profile()->RefreshData();
																		Centralized.main_menu->GetShopScreen()->UpdateOwnedBundles();
																		Centralized.main_menu->GetShopScreen()->UpdateBombPrices();
																		Centralized.main_menu->RefreshMenu();
																	}

																	delete packet;
																});
									packet->SetErroredCallback([packet]()
															   { delete packet; });
									packet->Send();
								});
	}
}

ColorBundle::ColorBundle()
	: Frame()
{
	this->bundle_id = 0;
	this->rarity = 1;
	this->bundle_name = "No name provided";
	this->price = -1;

	colors = (Frame * )(new Frame())
		->SetRelative(Vec2i(8, -15))
		->SetAlign(Align::CENTER, Align::BOTTOM)
		->SetFlex(Flex::WIDTH, 5)
		->SetAdaptive(true, true)
		->SetName("Colors");

	bundle_name_text = (TextElement *)(new TextElement())
		->UpdateTextOutline(CommonUI::sFontRegular.GetTTFFont(), "No name provided", 2,
							{ 0, 0, 0, 255 }, { 255, 255, 255, 255 })
		->SetRelative(Vec2i(8, 5))
		->SetAlign(Align::CENTER, Align::DONT)
		->SetName("Name");

	bundle_price_text = (TextElement *)(new TextElement())
		->UpdateText(CommonUI::sFontSmallerBold.GetTTFFont(),
					 "No price provided",
					 { 255, 255, 255, 255 })
		->SetAlign(Align::CENTER, Align::ABOVE_TOP)
		->SetName("Price");

	auto assets = Assets::Get();
	auto buy_text = assets->RenderTextBlendedOutline(CommonUI::sFontSmaller2x.GetTTFFont(), "Buy", 2,
													 { 255, 255, 255, 255 },
													 { 0, 0, 0, 255 })
		->FlagForAutomaticDeletion();
	auto buy_button_texture = RenderPresets::ColorButton(sTextureButton.GetTexture(),
														 { 0, 255, 0 },
														 sTextureButtonOutline.GetTexture(), buy_text)
		->FlagForAutomaticDeletion();
	buy_button = (Button * )(new Button(buy_button_texture,
										buy_button_texture))
		->SetSize(Vec2i(buy_button_texture->GetOriginalSize() * 0.4))
		->SetAlign(Align::CENTER, Align::UNDER_BOTTOM)
		->SetName("Buy");

	owned_text = (TextElement *)(new TextElement())
		->UpdateTextOutline(CommonUI::sFontSmallerBold.GetTTFFont(), "Owned", 2,
							{ 0, 255, 0, 255 },
							{ 0, 0, 0, 255 })
		->SetAlign(Align::CENTER, Align::UNDER_BOTTOM)
		->SetEnabled(false)
		->SetName("Owned");

	auto left = (Frame * )(new Frame())
		->SetOccupy(true, false)
		->SetFullyOccupy(false, true)
		->SetName("Left")
		->AddChildren({ bundle_name_text, colors });

	auto right = (Frame * )(new Frame())
		->SetRelative(Vec2i(0, -7))
		->SetSize(Vec2i(120, 0))
		->SetAlign(Align::DONT, Align::CENTER)
		->SetName("Right")
		->AddChildren({ bundle_price_text, buy_button, owned_text });

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
	bundle_id = bundle_data.value("bundle_id", 0);
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