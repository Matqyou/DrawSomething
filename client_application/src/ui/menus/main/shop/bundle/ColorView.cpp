//
// Created by Matq on 21/05/2025.
//

#include "ColorView.h"
#include "ui/RenderPresets.h"

void ColorView::UpdateTexture()
{
	delete view_texture;
	auto assets = Assets::Get();

	Texture *fill = assets->GetTexture("main_menu.shop.color.color_fill");
//	Texture *base = assets->GetTexture("main_menu.shop.color.color_base");
	Texture *overlay = assets->GetTexture("main_menu.shop.color.color_overlay");
	view_texture = RenderPresets::Composition({fill, overlay}, { color });

	SetTexture(view_texture);
}

ColorView::ColorView(int color_id, int rarity, Uint8 r, Uint8 g, Uint8 b)
	: Frame()
{
	this->color_id = color_id;
	this->rarity = rarity;
	this->color = { r, g, b, 255 };
	this->view_texture = nullptr;

	UpdateTexture();

	this->SetSize(Vec2i(44, 44));
	this->SetDraw(DRAW_TEXTURE);
	this->SetName("ColorView");
}

ColorView::~ColorView()
{
	delete view_texture;
}
