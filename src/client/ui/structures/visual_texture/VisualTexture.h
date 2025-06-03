//
// Created by Matq on 24/01/2025.
//

#pragma once

#include "../../../core/Rectangles.h"
#include <SDL3/SDL_render.h>

class Texture
{
protected:
	SDL_Texture *sdl_texture;
	Vec2f sdl_texture_size;
	bool flagged_for_automatic_deletion;

	bool uses_hitbox; // if is VisualTexture subclass

public:
	explicit Texture(SDL_Texture *sdl_texture = nullptr);
	virtual ~Texture();

	// Getting
	[[nodiscard]] SDL_Texture *SDLTexture() const { return sdl_texture; }
	[[nodiscard]] const Vec2f& GetOriginalSize() const { return sdl_texture_size; }
	[[nodiscard]] bool UsesHitbox() const { return uses_hitbox; }

	// Generating
	[[nodiscard]] Texture *CopyTexture(SDL_TextureAccess texture_access) const;

	// Manipulating
	Texture *FlagForAutomaticDeletion();
	Texture *SetSDLTexture(SDL_Texture *sdl_texture);
	Texture *SetBlendMode(SDL_BlendMode blend_mode);
	Texture *SetColorMod(Uint8 r, Uint8 g, Uint8 b);
	Texture *SetColorMod(SDL_Color color);
	Texture *SetColorModFloat(float r, float g, float b);
	Texture *SetColorModFloat(SDL_FColor color);
	Texture *SetAlphaMod(int alpha);
	Texture *SetScaleMode(SDL_ScaleMode scale_mode);

};

class VisualTexture : public Texture
{
private:
	Rect4f physical_hitbox;
	Rect4f translation_ratios;

public:
	VisualTexture(SDL_Texture *sdl_texture, const Rect4f& physical_hitbox);
	~VisualTexture() override;

	// Getting
	[[nodiscard]] const Rect4f& GetHitbox() const { return physical_hitbox; }
	[[nodiscard]] Rect4f GetResultingPlacement(const Rect4f& physical_placement) const;

};

class VisualTextureInstance
{
private:
	Texture *texture;
	Rect4f physical_placement;
	Rect4f resulting_placement;

public:
	explicit VisualTextureInstance(Texture *texture);
	VisualTextureInstance(Texture *texture, const Rect4f& initial_placement);
	~VisualTextureInstance();

	// Getting
	[[nodiscard]] Texture *GetTexture() const { return texture; }
	[[nodiscard]] const Rect4f& GetResultingRect() const { return resulting_placement; }
	[[nodiscard]] SDL_FRect GetResultingFRect() const
	{
		return { resulting_placement.x,
				 resulting_placement.y,
				 resulting_placement.w,
				 resulting_placement.h };
	}

	// Manipulating
	void ChangeTexture(Texture *new_texture);
	void UpdateResultingPlacement();
	void UpdateWithNewPlacement(const SDL_FRect& new_placement);

};
