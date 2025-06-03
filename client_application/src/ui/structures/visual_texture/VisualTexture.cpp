//
// Created by Matq on 24/01/2025.
//

#include "VisualTexture.h"
#include "core/app/Assets.h"

#define CHECK_SDL_TEXTURE(sdl_texture, message) \
    if ((sdl_texture) == nullptr) { \
        std::wcout << Strings::FStringColorsW(L"[Texture] &c" message L"\n"); \
        return this; \
    }

Texture::Texture(SDL_Texture *sdl_texture)
{
	this->sdl_texture = sdl_texture;
	SDL_GetTextureSize(sdl_texture, &sdl_texture_size.x, &sdl_texture_size.y); // might error on nullptr
	this->flagged_for_automatic_deletion = false;
	this->uses_hitbox = false;
}

Texture::~Texture()
{
	if (sdl_texture != nullptr)
	{
		SDL_DestroyTexture(sdl_texture);
		sdl_texture = nullptr;
	}
}

Texture *Texture::CopyTexture(SDL_TextureAccess texture_access) const
{
	Texture* new_texture;
	if (uses_hitbox)
		new_texture = new VisualTexture(*(VisualTexture*)(this));
	else
		new_texture = new Texture(*this);

	new_texture->sdl_texture = Assets::Get()->CopySDLTexture(sdl_texture, texture_access);
	return new_texture;
}

Texture *Texture::FlagForAutomaticDeletion()
{
	if (!flagged_for_automatic_deletion)
	{
		flagged_for_automatic_deletion = true;
		AssetsClass::AutomaticallyDeleteTexture(this);
	}
	return this;
}

Texture *Texture::SetSDLTexture(SDL_Texture *sdl_texture)
{
	this->sdl_texture = sdl_texture;
	SDL_GetTextureSize(sdl_texture, &sdl_texture_size.x, &sdl_texture_size.y);
	return this;
}

Texture *Texture::SetBlendMode(SDL_BlendMode blend_mode)
{
#ifndef NDEBUG
	CHECK_SDL_TEXTURE(sdl_texture, "&cSetBlendMode(), sdl_texture was nullptr!")
#endif

	SDL_SetTextureBlendMode(sdl_texture, blend_mode);
	return this;
}

Texture *Texture::SetColorMod(Uint8 r, Uint8 g, Uint8 b)
{
#ifndef NDEBUG
	CHECK_SDL_TEXTURE(sdl_texture, "&cSetColorMod(), sdl_texture '%s' was nullptr!")
#endif

	SDL_SetTextureColorMod(sdl_texture, r, g, b);
	return this;
}

Texture *Texture::SetColorMod(SDL_Color color)
{
#ifndef NDEBUG
	CHECK_SDL_TEXTURE(sdl_texture, "&cSetColorMod(), sdl_texture '%s' was nullptr!")
#endif

	SDL_SetTextureColorMod(sdl_texture, color.r, color.g, color.b);
	return this;
}

Texture *Texture::SetColorModFloat(float r, float g, float b)
{
#ifndef NDEBUG
	CHECK_SDL_TEXTURE(sdl_texture, "&cSetColorModFloat(), sdl_texture '%s' was nullptr!")
#endif

	SDL_SetTextureColorModFloat(sdl_texture, r, g, b);
	return this;
}

Texture *Texture::SetColorModFloat(SDL_FColor color)
{
#ifndef NDEBUG
	CHECK_SDL_TEXTURE(sdl_texture, "&cSetColorModFloat(), sdl_texture '%s' was nullptr!")
#endif

	SDL_SetTextureColorModFloat(sdl_texture, color.r, color.g, color.b);
	return this;
}

Texture *Texture::SetAlphaMod(int alpha)
{
#ifndef NDEBUG
	CHECK_SDL_TEXTURE(sdl_texture, "&cSetAlphaMode(), sdl_texture '%s' was nullptr!")
#endif

	SDL_SetTextureAlphaMod(sdl_texture, alpha);
	return this;
}

Texture *Texture::SetScaleMode(SDL_ScaleMode scale_mode)
{
#ifndef NDEBUG
	CHECK_SDL_TEXTURE(sdl_texture, "&cSetScaleMode(), sdl_texture '%s' was nullptr!")
#endif

	SDL_SetTextureScaleMode(sdl_texture, scale_mode);
	return this;
}

VisualTexture::VisualTexture(SDL_Texture *sdl_texture, const Rect4f& physical_hitbox)
	: Texture(sdl_texture)
{
	if (this->sdl_texture)
	{
		this->translation_ratios = {
			physical_hitbox.x / sdl_texture_size.x,
			physical_hitbox.y / sdl_texture_size.y,
			sdl_texture_size.x / physical_hitbox.w,
			sdl_texture_size.y / physical_hitbox.h,
		};
	}
	else
	{ this->translation_ratios = { 1.0, 1.0, 1.0, 1.0 }; }

	this->physical_hitbox = physical_hitbox;
	this->uses_hitbox = true;
}

VisualTexture::~VisualTexture()
{

}

Rect4f VisualTexture::GetResultingPlacement(const Rect4f& physical_placement) const
{
	return { physical_placement.x - physical_placement.w * translation_ratios.x,
			 physical_placement.y - physical_placement.h * translation_ratios.y,
			 physical_placement.w * translation_ratios.w,
			 physical_placement.h * translation_ratios.h };
};

VisualTextureInstance::VisualTextureInstance(Texture *texture)
{
	this->texture = texture;
	this->physical_placement = Rect4f(0.0f, 0.0f, 64.0f, 64.0f);
	this->resulting_placement = Rect4f(0.0f, 0.0f, 64.0f, 64.0f);
}

VisualTextureInstance::VisualTextureInstance(Texture *texture, const Rect4f& initial_placement)
{
	this->texture = texture;
	this->physical_placement = initial_placement;
	this->resulting_placement = initial_placement; //
}

VisualTextureInstance::~VisualTextureInstance()
{

}

void VisualTextureInstance::ChangeTexture(Texture *new_texture)
{
	this->texture = new_texture;

	UpdateResultingPlacement();
}

void VisualTextureInstance::UpdateResultingPlacement()
{
	if (texture == nullptr) return;

	if (texture->UsesHitbox())
	{
		auto *visual_texture = (VisualTexture *)texture;
		this->resulting_placement = visual_texture->GetResultingPlacement(physical_placement);
	}
	else
	{ this->resulting_placement = physical_placement; } // little sussy and maybe expensive
}

void VisualTextureInstance::UpdateWithNewPlacement(const SDL_FRect& new_placement)
{
	this->physical_placement = Rect4f(new_placement.x, new_placement.y, new_placement.w, new_placement.h);

	UpdateResultingPlacement();
}
