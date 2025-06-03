//
// Created by Matq on 23/05/2025.
//

#include "RenderPresets.h"
#include "core/app/Assets.h"
#include "core/app/Drawing.h"

Texture *RenderPresets::ColorButton(Texture *base,
									SDL_Color base_color,
									Texture *overlay,
									Texture *centered_texture,
									bool grayed_out)
{
	if (!base)
		return nullptr;

	auto assets = Assets::Get();
	auto drawing = assets->GetDrawing();
	auto original_render_target = SDL_GetRenderTarget(drawing->Renderer());

	auto base_copy = base->CopyTexture(SDL_TEXTUREACCESS_TARGET)
		->SetColorMod(base_color);
	auto result = base->CopyTexture(SDL_TEXTUREACCESS_TARGET);
	drawing->SetRenderTarget(result);
	drawing->RenderTextureFullscreen(base_copy->SDLTexture(), nullptr);
	delete base_copy;
	if (overlay)
		drawing->RenderTextureFullscreen(overlay->SDLTexture());
	if (centered_texture && centered_texture->UsesHitbox())
	{
		auto text_texture_visual = (VisualTexture *)centered_texture;

		const auto& base_size = base->GetOriginalSize();
		const auto& text_size = Vec2f(text_texture_visual->GetHitbox().w, text_texture_visual->GetHitbox().h);

		SDL_FRect text_destination = Rectangles::CenterRelative(text_size, base_size);
		drawing->RenderTexture(centered_texture->SDLTexture(), nullptr, text_destination);
	}
	if (grayed_out)
	{
		result->SetColorMod(200, 200, 200);

		auto old_result = result;
		result = old_result->CopyTexture(SDL_TEXTUREACCESS_TARGET);
		delete old_result;
	}
	drawing->SetRenderTargetSDL(original_render_target);

	return result;
}

Texture *RenderPresets::ColorButton(Texture *base, SDL_Color base_color,
									Texture *overlay, SDL_Color overlay_color,
									Texture *centered_texture, bool grayed_out)
{
	if (!base)
		return nullptr;

	auto assets = Assets::Get();
	auto drawing = assets->GetDrawing();
	auto original_render_target = SDL_GetRenderTarget(drawing->Renderer());

	auto base_copy = base->CopyTexture(SDL_TEXTUREACCESS_TARGET)
		->SetColorMod(base_color);
	auto result = base->CopyTexture(SDL_TEXTUREACCESS_TARGET);
	drawing->SetRenderTarget(result);
	drawing->RenderTextureFullscreen(base_copy->SDLTexture(), nullptr);
	delete base_copy;
	if (overlay)
	{
		auto overlay_copy = overlay->CopyTexture(SDL_TEXTUREACCESS_TARGET)
			->SetColorMod(overlay_color);
		drawing->RenderTextureFullscreen(overlay_copy->SDLTexture());
		delete overlay_copy;
	}
	if (centered_texture && centered_texture->UsesHitbox())
	{
		auto text_texture_visual = (VisualTexture *)centered_texture;

		const auto& base_size = base->GetOriginalSize();
		const auto& text_size = Vec2f(text_texture_visual->GetHitbox().w, text_texture_visual->GetHitbox().h);

		SDL_FRect text_destination = Rectangles::CenterRelative(text_size, base_size);
		drawing->RenderTexture(centered_texture->SDLTexture(), nullptr, text_destination);
	}
	if (grayed_out)
	{
		result->SetColorMod(200, 200, 200);

		auto old_result = result;
		result = old_result->CopyTexture(SDL_TEXTUREACCESS_TARGET);
		delete old_result;
	}
	drawing->SetRenderTargetSDL(original_render_target);

	return result;
}