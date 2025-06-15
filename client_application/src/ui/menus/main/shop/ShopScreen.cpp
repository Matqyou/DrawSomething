//
// Created by Matq on 28/04/2025.
//

#include "ShopScreen.h"
#include "ui/menus/main/shop/bundle/ColorBundle.h"
#include "ui/RenderPresets.h"
#include "ui/CommonUI.h"
#include "ui/menus/main/shop/bombs/BombOffer.h"

static LinkTexture sTextureColorsTitle("main_menu.shop.buy_colors");
static LinkTexture sTextureCard("main_menu.shop.card");
static LinkTexture sTextureCardOutline("main_menu.shop.card_outline");
static LinkTexture sTextureClose("main_menu.settings.x");

static LinkTexture sTextureBombs("main_menu.shop.bombs");
static LinkTexture sTextureBombCard("main_menu.shop.bomb_card");
static LinkTexture sTextureBombCardOutline("main_menu.shop.bomb_card_outline");

ShopScreen::ShopScreen()
	: ScreenMenu()
{
	auto bomb_card_texture = RenderPresets::Composition({ sTextureBombCard.GetTexture(),
														  sTextureBombCardOutline.GetTexture() }, {
															SDL_Color(255, 110, 0) });

	auto bombs_title = (Frame *)(new Frame())
		->SetRelative(Vec2i(0, 30))
		->SetSize(Vec2i(sTextureBombs.GetTexture()->GetOriginalSize()))
		->SetDraw(DRAW_TEXTURE)
		->SetTexture(sTextureBombs.GetTexture())
		->SetAlign(Align::CENTER, Align::DONT)
		->SetName("Title");

	bombs1 = new BombOffer(0);
	bombs2 = new BombOffer(1);
	bombs3 = new BombOffer(2);

	auto bombs_card = (Frame *)(new Frame())
		->SetRelative(Vec2i(550, 100))
		->SetDraw(DRAW_TEXTURE)
		->SetTexture(bomb_card_texture)
		->SetSize(Vec2i(bomb_card_texture->GetOriginalSize()))
		->SetFlex(Flex::HEIGHT, 10)
		->SetName("Bombs")
		->AddChildren({ bombs_title, bombs1, bombs2, bombs3 });

	auto colors_title = (Frame *)(new Frame())
		->SetRelative(Vec2i(0, 10))
		->SetSize(Vec2i(sTextureColorsTitle.GetTexture()->GetOriginalSize() / 2))
		->SetTexture(sTextureColorsTitle.GetTexture())
		->SetDraw(DRAW_TEXTURE)
		->SetAlign(Align::CENTER, Align::DONT)
		->SetName("BuyColors");

	colors_frame = (ScrollFrame *)(new ScrollFrame())
		->SetScrollDirection(Orientation::VERTICAL)
		->SetRelative(Vec2i(40, 100))
		->SetSize(Vec2i(0, 505))
		->SetAdaptive(true, false)
		->SetFlex(Flex::HEIGHT, 5)
		->SetName("ColorsFrame")
		->AddChildren({ });

	auto close_button = (Button *)(new Button(sTextureClose.GetTexture(),
											  sTextureClose.GetTexture()))
		->SetRelative(Vec2i(-10, 10))
		->SetSize(Vec2i(32, 32))
		->SetColor(150, 0, 0, 255)
		->SetAlign(Align::RIGHT, Align::TOP)
		->SetFlexInvolved(false, false)
		->SetName("CloseButton");
	close_button->SetCallback([this]()
							  {
								  this->SetEnabled(false);
							  });

	auto card_texture = RenderPresets::ColorButton(sTextureCard.GetTexture(), { 226, 139, 72, 255 },
												   sTextureCardOutline.GetTexture(), { 255, 255, 255, 255 });
	auto card = (Frame *)(new Frame())
		->SetSize(Vec2i(card_texture->GetOriginalSize()))
		->SetTexture(card_texture)
		->SetDraw(DRAW_TEXTURE)
		->SetAlign(Align::CENTER, Align::CENTER)
		->SetName("Colors")
		->AddChildren({ colors_title, colors_frame, bombs_card, close_button });

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
			auto bundle = (new ColorBundle())
				->ParseFromJson(bundle_data);
			colors_frame->AddChildren({ bundle });
		}
	}

	UpdateBombPrices();
}

void ShopScreen::UpdateOwnedBundles()
{
	for (auto bundle_ : colors_frame->children)
	{
		auto bundle = (ColorBundle *)bundle_;
		bundle->UpdatePrice();
	}
}

void ShopScreen::UpdateBombPrices()
{
	bombs1->UpdatePrice();
	bombs2->UpdatePrice();
	bombs3->UpdatePrice();
}
