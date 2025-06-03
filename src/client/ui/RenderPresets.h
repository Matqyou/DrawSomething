//
// Created by Matq on 23/05/2025.
//

#pragma once

#include "client/ui/structures/visual_texture/VisualTexture.h"

class RenderPresets
{
public:
	static Texture *ColorButton(Texture* base, SDL_Color base_color,
								Texture* overlay,
								Texture* centered_texture = nullptr, bool grayed_out = false);
	static Texture *ColorButton(Texture* base, SDL_Color base_color,
								Texture* overlay, SDL_Color overlay_color,
								Texture* centered_texture = nullptr, bool grayed_out = false);

};
