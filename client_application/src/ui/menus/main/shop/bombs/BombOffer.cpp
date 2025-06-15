//
// Created by Matq on 15/06/2025.
//

#include "BombOffer.h"
#include "ui/RenderPresets.h"
#include "ui/components/element/TextElement.h"
#include "ui/CommonUI.h"
#include "ui/menus/main/MainMenu.h"
#include "game/GameData.h"

static LinkTexture sTextureBombContent1("main_menu.shop.bomb_content");
static LinkTexture sTextureBombContent2("main_menu.shop.bomb_content2");
static LinkTexture sTextureBombContent3("main_menu.shop.bomb_content3");
static LinkTexture sTextureBombOutline("main_menu.shop.bomb_outline");
static LinkTexture sTextureBombIcon("icons.bomb");
static LinkTexture sTextureButton("button");
static LinkTexture sTextureButtonOutline("button_outline");

void BombOffer::UpdatePrice()
{
	auto& account = Centralized.GetAccount();
	bool enough_money = Centralized.GetAccount().coins >= price;
//	price_text->UpdateText(CommonUI::sFontSmallerBold.GetTTFFont(),
//								  Strings::FString("%d Coins", price).c_str(),
//								  enough_money ? SDL_Color(255, 255, 255, 255) : SDL_Color(255, 0, 0, 255));
	price_text->UpdateTextOutline(CommonUI::sFontSmallerBold.GetTTFFont(), Strings::FString("%d Coins", price).c_str(), 2,
								  enough_money ? SDL_Color(255, 255, 255, 255) : SDL_Color(255, 0, 0, 255),
								  { 0, 0, 0, 255 });
	buy_button->SetEnabled(enough_money);

	if (enough_money)
	{
		buy_button->SetCallback([this]()
			{
				json buy_data;
				buy_data["bombs_id"] = display_id;

				auto packet = new Packet("/buy_bombs", "POST", &buy_data);
				packet->SetResponseCallback([packet](const NetworkResponse& server_response)
					{
						if (server_response.response_json.contains("user") && server_response.response_json["user"].is_object())
						{
							json user_data = server_response.response_json["user"];
							auto account = Centralized.GetAccount().ParseFromJson(user_data);

							Centralized.main_header->RefreshData();
							Centralized.main_menu->Profile()->RefreshData();
							Centralized.main_menu->GetShopScreen()->UpdateBombPrices();
							Centralized.main_menu->GetShopScreen()->UpdateOwnedBundles();
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

BombOffer::BombOffer(int display_id)
	: Frame()
{
	this->display_id = display_id;
	auto bomb_content_texture = display_id == 0 ? RenderPresets::Composition({ sTextureBombContent1.GetTexture(),
																			   sTextureBombOutline.GetTexture() }, {
																				 SDL_Color(255, 186, 0) }) :
								display_id == 1 ?
								RenderPresets::Composition({ sTextureBombContent2.GetTexture(),
															 sTextureBombOutline.GetTexture() }, {
															   SDL_Color(255, 100, 0) }) :
								RenderPresets::Composition({ sTextureBombContent3.GetTexture(),
															 sTextureBombOutline.GetTexture() }, {
															   SDL_Color(255, 50, 0) });
	const char *bomb_offer_text = display_id == 0 ? "Get a bomb" :
								  display_id == 1 ? "Get 3 bombs" : "Get 10 bombs";
	this->price = display_id == 0 ? 5 :
		display_id == 1 ? 12 : 30;

	auto assets = Assets::Get();
	auto buy_text = assets->RenderTextBlendedOutline(CommonUI::sFontSmaller2x.GetTTFFont(), "Buy", 2,
													 { 255, 255, 255, 255 },
													 { 0, 0, 0, 255 })
		->FlagForAutomaticDeletion();
	auto buy_button_texture = RenderPresets::ColorButton(sTextureButton.GetTexture(),
														 { 0, 255, 0 },
														 sTextureButtonOutline.GetTexture(), buy_text)
		->FlagForAutomaticDeletion();
	buy_button = (Button *)(new Button(buy_button_texture,
									   buy_button_texture))
		->SetSize(Vec2i(buy_button_texture->GetOriginalSize() * 0.4))
		->SetAlign(Align::CENTER, Align::UNDER_BOTTOM)
		->SetName("Buy");

	price_text = (TextElement *)(new TextElement())
		->UpdateText(CommonUI::sFontSmallerBold.GetTTFFont(),
					 Strings::FString("%d Coins", price).c_str(),
					 { 255, 255, 255, 255 })
		->SetAlign(Align::CENTER, Align::ABOVE_TOP)
		->SetName("Price");

	auto right = (Frame *)(new Frame())
		->SetRelative(Vec2i(0, -7))
		->SetSize(Vec2i(120, 0))
		->SetAlign(Align::RIGHT, Align::CENTER)
		->SetName("Right")
		->AddChildren({ price_text, buy_button });

	this->SetRelative(Vec2i(0, 30));
	this->SetSize(Vec2i(bomb_content_texture->GetOriginalSize()));
	this->SetTexture(bomb_content_texture);
	this->SetDraw(DRAW_TEXTURE);
	this->SetAlign(Align::CENTER, Align::DONT);
	this->SetName("BombsOffer");
	for (int i = 0; i < display_id + 1; i++)
		this->AddChildren({
							  (new Frame())
								  ->SetDraw(DRAW_TEXTURE)
								  ->SetTexture(sTextureBombIcon.GetTexture())
								  ->SetSize(Vec2i(80, 80))
								  ->SetRelative(Vec2i(i * 20, 0))
						  });
	this->AddChildren({
						  (new TextElement())
							  ->UpdateTextOutline(CommonUI::sFontMiniProfile2.GetTTFFont(), bomb_offer_text, 3,
												  { 0, 0, 0, 255 }, { 255, 255, 255, 255 })
							  ->SetAlign(Align::CENTER, Align::CENTER),
						  right
					  });

	UpdatePrice();
}

BombOffer::~BombOffer()
{

}
