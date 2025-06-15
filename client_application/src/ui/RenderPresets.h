//
// Created by Matq on 23/05/2025.
//

#pragma once

#include "ui/structures/visual_texture/VisualTexture.h"
#include <string>
#include <vector>

class RenderPresets
{
public:
	static Texture *ColorButton(Texture* base, SDL_Color base_color,
								Texture* overlay,
								Texture* centered_texture = nullptr, bool grayed_out = false);
	static Texture *ColorButton(Texture* base, SDL_Color base_color,
								Texture* overlay, SDL_Color overlay_color,
								Texture* centered_texture = nullptr, bool grayed_out = false);

	static Texture *Window(const std::string& texture_key,
						   float left_scaling, float right_scaling, float top_scaling, float bottom_scaling,
						   const Vec2f& size, SDL_Color color);

	static Texture *Composition(std::initializer_list<Texture*> textures, std::initializer_list<SDL_Color> colors);
};
