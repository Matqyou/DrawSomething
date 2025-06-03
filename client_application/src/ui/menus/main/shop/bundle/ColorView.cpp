//
// Created by Matq on 21/05/2025.
//

#include "ColorView.h"

void ColorView::UpdateTexture()
{
	delete view_texture;
	auto assets = Assets::Get();
	auto drawing = assets->GetDrawing();

	std::string frame_key = Strings::FString("main_menu.shop.color.%d", rarity);
	Texture *frame = assets->GetTexture(frame_key);
	Texture *overlay = assets->GetTexture("main_menu.shop.color.overlay");
	auto size = frame->GetOriginalSize();
	view_texture = Assets::Get()->CreateTexture(SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, (int)size.x, (int)size.y);

	drawing->SetRenderTarget(view_texture);
	drawing->SetColor(color);
	drawing->Clear();
	drawing->RenderTextureFullscreen(frame->SDLTexture(), nullptr);
	drawing->RenderTextureFullscreen(overlay->SDLTexture(), nullptr);
	drawing->SetRenderTarget(nullptr);

	SetTexture(view_texture);
	SetSize(Vec2i(size/2));
}

ColorView::ColorView(int color_id, int rarity, Uint8 r, Uint8 g, Uint8 b)
	: Frame()
{
	this->color_id = color_id;
	this->rarity = rarity;
	this->color = { r, g, b, 255 };
	this->view_texture = nullptr;

	UpdateTexture();

	SetDraw(DRAW_TEXTURE);
	SetName("ColorView");
}

ColorView::~ColorView()
{
	delete view_texture;
}
