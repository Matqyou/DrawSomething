//
// Created by Matq on 28/04/2025.
//

#include "ShopScreen.h"
#include "ui/menus/main/shop/bundle/ColorBundle.h"
//#include "client/game/GameData.h"

static LinkTexture sTextureColorsTitle("main_menu.shop.buy_colors");
static LinkTexture sTextureBombsTitle("main_menu.shop.buy_bombs");
static LinkTexture sTextureCard("main_menu.shop.card");
static LinkTexture sTextureSubcard("main_menu.shop.subcard");
//static LinkTexture sTextureAdminCard("main_menu.admin.card");

ShopScreen::ShopScreen()
	: Frame()
{
	Vec2i half;
	{
		auto halff = sTextureCard.GetTexture()->GetOriginalSize() / 2;
		halff.x /= 2;
		half = Vec2i(halff);
	}

	auto bombs_title = (Frame *)(new Frame())
		->SetRelative(Vec2i(0, 10))
		->SetSize(Vec2i(sTextureColorsTitle.GetTexture()->GetOriginalSize() / 2))
		->SetTexture(sTextureBombsTitle.GetTexture())
		->SetDraw(DRAW_TEXTURE)
		->SetAlign(Align::CENTER, Align::DONT)
		->SetName("BuyColors");

	auto bombs_card = (Frame *)(new Frame())
		->SetSize(Vec2i(sTextureSubcard.GetTexture()->GetOriginalSize() / 2))
		->SetTexture(sTextureSubcard.GetTexture())
		->SetDraw(DRAW_TEXTURE)
		->SetAlign(Align::CENTER, Align::CENTER)
		->SetFlex(Flex::HEIGHT)
		->SetName("Colors")
		->AddChildren({ bombs_title });

	auto left = (Frame *)(new Frame())
		->SetSize(half)
		->SetName("Left")
		->AddChildren({ bombs_card });

	auto colors_title = (Frame *)(new Frame())
		->SetRelative(Vec2i(0, 10))
		->SetSize(Vec2i(sTextureColorsTitle.GetTexture()->GetOriginalSize() / 2))
		->SetTexture(sTextureColorsTitle.GetTexture())
		->SetDraw(DRAW_TEXTURE)
		->SetAlign(Align::CENTER, Align::DONT)
		->SetName("BuyColors");

	colors_frame = (Frame *)(new Frame())
		->SetRelative(Vec2i(0, 10))
		->SetAdaptive(true, true)
		->SetAlign(Align::CENTER, Align::DONT)
		->SetFlex(Flex::HEIGHT,  5)
		->SetName("ColorsFrame")
		->AddChildren({ });

	auto colors_card = (Frame *)(new Frame())
		->SetSize(Vec2i(sTextureSubcard.GetTexture()->GetOriginalSize() / 2))
		->SetTexture(sTextureSubcard.GetTexture())
		->SetDraw(DRAW_TEXTURE)
		->SetAlign(Align::CENTER, Align::CENTER)
		->SetFlex(Flex::HEIGHT, 10)
		->SetName("Colors")
		->AddChildren({ colors_title, colors_frame });

	auto right = (Frame *)(new Frame())
		->SetSize(half)
		->SetName("Right")
		->AddChildren({ colors_card });

	Frame *card = (Frame *)(new Frame())
		->SetSize(Vec2i(sTextureCard.GetTexture()->GetOriginalSize() / 2))
		->SetTexture(sTextureCard.GetTexture())
		->SetDraw(DRAW_TEXTURE)
		->SetAlign(Align::CENTER, Align::CENTER)
		->SetFlex(Flex::WIDTH)
		->SetName("Card")
		->AddChildren({ left, right });

	this->SetEnabled(false);
	this->SetColor(0, 0, 0, 200);
	this->SetDraw(DRAW_RECT);
	this->SetFullyOccupy(true, true);
	this->SetName("ShopMenu");
	this->AddChildren({ card });
}

ShopScreen::~ShopScreen()
{

}

void ShopScreen::ParseFromJson(const json& shop_data)
{
	for (auto bundle : colors_frame->children)
		delete bundle;
	colors_frame->children.clear();

	if (shop_data.contains("bundles") && shop_data["bundles"].is_array())
	{
		for (const json& bundle_data : shop_data["bundles"])
		{
//			int rarity = bundle_data.value("rarity", 1);
//			std::string bundle_name = bundle_data.value("name", "No name provided");
//			int price = bundle_data.value("price", -1);

			auto bundle = (new ColorBundle())
				->ParseFromJson(bundle_data);
			colors_frame->AddChildren({ bundle });
		}
	}
}

//void ShopScreen::RefreshData()
//{
//	for (auto user : users_frame->children)
//		delete user;
//	users_frame->children.clear();
//
//	for (auto user : Centralized.GetAdminUserList())
//		users_frame->AddChildren({ (new UserRecord())->UpdateInfo(user) });
//}
