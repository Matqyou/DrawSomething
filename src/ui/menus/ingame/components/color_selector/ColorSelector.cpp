//
// Created by Matq on 13/01/2025.
//

#include "ColorSelector.h"
#include "core/Application.h"
#include "ui/CommonUI.h"
#include "ui/menus/ingame/components/tool_selector/BrushSizeButton.h"
#include "ui/components/element/TextElement.h"

namespace Ingame
{
static SDL_Color sAvailableColors[13] = {
	SDL_Color(0, 0, 0, 255),
	SDL_Color(255, 0, 0, 255),
	SDL_Color(255, 255, 0, 255),
	SDL_Color(0, 0, 255, 255),
	SDL_Color(0, 255, 0, 255), //
	SDL_Color(255, 0, 255, 255), //
	SDL_Color(0, 255, 255, 255), //
	SDL_Color(255, 128, 0, 255), //
	SDL_Color(128, 255, 0, 255), //
	SDL_Color(0, 255, 128, 255), //
	SDL_Color(0, 128, 255, 255), //
	SDL_Color(128, 0, 255, 255), //
	SDL_Color(255, 0, 128, 255), //
};

Texture *sGeneratePlus(AssetsClass *assets)
{
	auto drawing = assets->GetDrawing();
	auto plus_texture = assets->GetTexture("game.color_selector.plus");
	auto plus_size = plus_texture->GetOriginalSize();
	Texture *plus = assets->CreateTexture(SDL_PIXELFORMAT_RGBA8888,
										  SDL_TEXTUREACCESS_TARGET,
										  (int)plus_size.x, (int)plus_size.y)
		->FlagForAutomaticDeletion();
	drawing->SetRenderTarget(plus);
	plus_texture->SetColorMod(255, 255, 255);
	drawing->RenderTextureFullscreen(plus_texture->SDLTexture(), nullptr);
	return plus;
}
Texture *sGeneratePlusPressed(AssetsClass *assets)
{
	auto drawing = assets->GetDrawing();
	auto plus_texture = assets->GetTexture("game.color_selector.plus");
	auto plus_size = plus_texture->GetOriginalSize();
	Texture *plus_pressed = assets->CreateTexture(SDL_PIXELFORMAT_RGBA8888,
												  SDL_TEXTUREACCESS_TARGET,
												  (int)plus_size.x, (int)plus_size.y)
		->FlagForAutomaticDeletion();
	drawing->SetRenderTarget(plus_pressed);
	plus_texture->SetColorMod(200, 200, 200);
	drawing->RenderTextureFullscreen(plus_texture->SDLTexture(), nullptr);
	return plus_pressed;
}

static LinkTexture sTextureColor("game.color_selector.color", CommonUI::sCallbackScaleNearest);
static LinkTexture sTextureColorOverlay("game.color_selector.color_overlay", CommonUI::sCallbackScaleNearest);
static LinkTexture sTexturePlus_("game.color_selector.plus", CommonUI::sCallbackScaleNearest);

static PregenerateTexture sTexturePlus("game.color_selector.plus_", sGeneratePlus);
static PregenerateTexture sTexturePlusPressed("game.color_selector.plus_pressed", sGeneratePlusPressed);

ColorSelector::ColorSelector(Canvas *canvas, Ingame::ToolSelector *tool_selector)
	: Frame()
{
	auto assets = Assets::Get();
	auto drawing = assets->GetDrawing();

	// Colors
	std::vector<Element *> colors;
	for (auto sdl_color : sAvailableColors)
	{
		auto color = sTextureColor.GetTexture();
		auto color_overlay = sTextureColorOverlay.GetTexture();
		Texture *color_composition = assets->CreateTexture(SDL_PIXELFORMAT_RGBA8888,
														   SDL_TEXTUREACCESS_TARGET,
														   (int)color->GetOriginalSize().x,
														   (int)color->GetOriginalSize().y)
			->FlagForAutomaticDeletion();
		color_composition->SetScaleMode(SDL_SCALEMODE_NEAREST);
		color_composition->SetBlendMode(SDL_BLENDMODE_BLEND);
		drawing->SetRenderTarget(color_composition);
		color->SetColorMod(sdl_color);
		drawing->RenderTextureFullscreen(color->SDLTexture(), nullptr);
		drawing->RenderTextureFullscreen(color_overlay->SDLTexture(), nullptr);

		auto new_color = (Button *)(new Button(color_composition, nullptr))
			->SetSize(Vec2i(42, 42))
			->SetAlign(Align::DONT, Align::CENTER)
			->SetName("Color");

		new_color->SetCallback([canvas, sdl_color, tool_selector]()
							   {
								   canvas->SetDrawColor(sdl_color);
								   canvas->SetTool(TOOL_PENCIL);
								   tool_selector->SetFocus(tool_selector->pencil_tool);
								   tool_selector->pencil_tool->GenerateBrushesColor(sdl_color);
								   canvas->SetCustomCursor(tool_selector->pencil_tool->GetSelectedCursor());
							   });
		colors.push_back(new_color);
	}

	// More colors button
	auto more_colors_button = (Button *)(new Button(sTexturePlus.GetTexture(), sTexturePlusPressed.GetTexture()))
		->SetCallback([]()
					  { std::wcout << "More Colors\n"; })
		->SetSize(Vec2i(42, 42))
		->SetAlign(Align::DONT, Align::CENTER)
		->SetName("MoreColors");
	colors.push_back(more_colors_button);

	auto more_colors = (new TextElement())
		->UpdateText(CommonUI::sFontSmaller.GetFont()->TTFFont(),"more colors", { 150, 150, 150, 255 })
		->SetAlign(Align::DONT, Align::CENTER)
		->SetName("MoreColorsText");

	colors.push_back(more_colors);

	// Color selector
	this->SetColor(255, 255, 255, 50);
	this->SetDraw(DRAW_RECT);
	this->SetSize(Vec2i(0, 50));
	this->SetFullyOccupy(true, false);
	this->SetFlex(Flex::WIDTH, 4);
	this->SetName("ColorSelector");
	this->AddChildren(colors);
}
}
