//
// Created by Matq on 13/01/2025.
//

#include "ToolSelector.h"

#include "../../../../../core/Application.h"
#include "../../../../CommonUI.h"
#include "BrushSizeButton.h"

namespace Ingame
{
static LinkTexture sTextureOutline("game.tool_selector.outline", CommonUI::sCallbackScaleNearest);
static LinkTexture sTexturePencil("game.tool_selector.pencil", CommonUI::sCallbackScaleNearest);
static LinkTexture sTextureEraser("game.tool_selector.eraser", CommonUI::sCallbackScaleNearest);
static LinkTexture sTextureTrash("game.tool_selector.trash", CommonUI::sCallbackScaleNearest);
static LinkTexture sTextureGear("game.tool_selector.gear", CommonUI::sCallbackScaleNearest);
static LinkTexture sTextureDoneButton("game.tool_selector.done_button");

auto static sPregeneratePencilButton = [](AssetsClass *assets) -> Texture *
{
	auto drawing = assets->GetDrawing();
	auto texture_button = sTextureOutline.GetTexture();
	Vec2f button_size = texture_button->GetOriginalSize();
	auto texture_pencil = sTexturePencil.GetTexture();
	Texture *pencil_button = assets->CreateTexture(SDL_PIXELFORMAT_RGBA8888,
												   SDL_TEXTUREACCESS_TARGET,
												   (int)button_size.x,
												   (int)button_size.y)
		->SetScaleMode(SDL_SCALEMODE_NEAREST)
		->SetBlendMode(SDL_BLENDMODE_BLEND)
		->FlagForAutomaticDeletion();
	SDL_SetTextureScaleMode(pencil_button->SDLTexture(), SDL_SCALEMODE_NEAREST);
	drawing->SetRenderTarget(pencil_button);
	drawing->RenderTextureFullscreen(texture_pencil->SDLTexture(), nullptr);
	drawing->RenderTextureFullscreen(texture_button->SDLTexture(), nullptr);
	return pencil_button;
};
auto static sPregenerateEraserButton = [](AssetsClass *assets) -> Texture *
{
	auto drawing = assets->GetDrawing();
	auto texture_button = sTextureOutline.GetTexture();
	Vec2f button_size = texture_button->GetOriginalSize();
	auto texture_eraser = sTextureEraser.GetTexture();
	Texture *eraser_button = assets->CreateTexture(SDL_PIXELFORMAT_RGBA8888,
												   SDL_TEXTUREACCESS_TARGET,
												   (int)button_size.x,
												   (int)button_size.y)
		->SetScaleMode(SDL_SCALEMODE_NEAREST)
		->SetBlendMode(SDL_BLENDMODE_BLEND)
		->FlagForAutomaticDeletion();
	drawing->SetRenderTarget(eraser_button);
	drawing->RenderTextureFullscreen(texture_eraser->SDLTexture(), nullptr);
	drawing->RenderTextureFullscreen(texture_button->SDLTexture(), nullptr);
	return eraser_button;
};
auto static sPregenerateDoneButton = [](AssetsClass *assets) -> Texture *
{
	auto drawing = assets->GetDrawing();
	auto button_base = sTextureDoneButton.GetTexture();
	Vec2f button_size = button_base->GetOriginalSize();
	auto button_text = assets->RenderTextBlended(assets->GetFont("fredoka.big")->TTFFont(),
												 "D O N E",
												 { 230, 230, 230, 255 });

	SDL_FRect text_rect = Rectangles::CenterRelative(button_text->GetOriginalSize() * 1.5, button_size);

	Texture *done_button = assets->CreateTexture(SDL_PIXELFORMAT_RGBA8888,
												 SDL_TEXTUREACCESS_TARGET,
												 (int)button_size.x,
												 (int)button_size.y)
		->SetBlendMode(SDL_BLENDMODE_BLEND)
		->SetScaleMode(SDL_SCALEMODE_NEAREST)
		->FlagForAutomaticDeletion();
	drawing->SetRenderTarget(done_button);
	drawing->RenderTextureFullscreen(button_base->SDLTexture(), nullptr);
	drawing->RenderTexture(button_text->SDLTexture(), nullptr, text_rect);
	return done_button;
};

static PregenerateTexture sTexturePencilButton("game.tool_selector.pencil_button", sPregeneratePencilButton);
static PregenerateTexture sTextureEraserButton("game.tool_selector.eraser_button", sPregenerateEraserButton);
static PregenerateTexture sTextureDoneButton_("game.tool_selector.done_button_", sPregenerateDoneButton);

ToolSelector::ToolSelector(Canvas *canvas)
	: Frame()
{
	// Tools
	std::vector<Element *> tools;

	pencil_tool = (DrawingToolButton *)(new DrawingToolButton(canvas, this))
		->SetRelative(Vec2i(50, 0))
		->SetTexture(sTexturePencilButton.GetTexture())
		->SetName("PencilTool");
	eraser_tool = (DrawingToolButton *)(new DrawingToolButton(canvas, this))
		->SetRelative(Vec2i(150, 0))
		->SetTexture(sTextureEraserButton.GetTexture())
		->SetName("EraserTool");
	trash_button = (Button *)(new Button(sTextureTrash.GetTexture(), nullptr))
		->SetRelative(Vec2i(250, 0))
		->SetSize(Vec2i(54, 54))
		->SetAlign(Align::DONT, Align::CENTER)
		->SetName("ClearCanvas");

	tools.push_back(pencil_tool);
	tools.push_back(eraser_tool);
	tools.push_back(trash_button);

	canvas->SetCustomCursor(pencil_tool->GetSelectedCursor());

	done_button = (Button *)(new Button(sTextureDoneButton_.GetTexture(), nullptr))
		->SetRelative(Vec2i(350, 0))
		->SetSize(Vec2i(192, 50))
		->SetAlign(Align::DONT, Align::CENTER)
		->SetName("DoneButton");
	tools.push_back(done_button);

	auto settings_button = (new Button(sTextureGear.GetTexture(), nullptr))
		->SetRelative(Vec2i(592, 0))
		->SetSize(Vec2i(54, 54))
		->SetAlign(Align::DONT, Align::CENTER)
		->SetName("Settings");
	tools.push_back(settings_button);

	// Callbacks
	trash_button->SetCallback([canvas]()
							  { canvas->ClearCanvas(); });
	pencil_tool->SetCallback([this, canvas]()
							 {
								 eraser_tool->HideBrushSizeButtons();
								 canvas->SetTool(TOOL_PENCIL);
								 if (!pencil_tool->has_focus)
								 {
									 SetFocus(pencil_tool);
									 canvas->SetCustomCursor(pencil_tool->GetSelectedCursor());
								 }
								 else
								 {
									 pencil_tool->ToggleShowBrushSizeButtons();
//            Refresh();
								 }
							 });
	eraser_tool->SetCallback([this, canvas]()
							 {
								 pencil_tool->HideBrushSizeButtons();
								 canvas->SetTool(TOOL_ERASER);
								 if (!eraser_tool->has_focus)
								 {
									 SetFocus(eraser_tool);
									 canvas->SetCustomCursor(eraser_tool->GetSelectedCursor());
								 }
								 else
								 {
									 eraser_tool->ToggleShowBrushSizeButtons();
//            Refresh();
								 }
							 });

	// Tool selector
	SetColor(255, 255, 255, 50);
	SetDraw(DRAW_RECT);
	SetSize(Vec2i(0, 74));
	SetFullyOccupy(true, false);
	SetName("ToolSelector");
	AddChildren(tools);
	SetFocus(pencil_tool);
}

void ToolSelector::SetDoneCallback(Callback new_callback)
{
	done_button->SetCallback(std::move(new_callback));
}
}
