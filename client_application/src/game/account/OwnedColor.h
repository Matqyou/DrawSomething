//
// Created by Matq on 04/06/2025.
//

#pragma once

#include <SDL3/SDL_pixels.h>
#include "network/NetworkClient.h"

class OwnedColor
{
private:
	int color_id;
	SDL_Color sdl_color;

public:
	OwnedColor();
	~OwnedColor();

	// Sense
	[[nodiscard]] int GetColorID() const { return color_id; };
	[[nodiscard]] SDL_Color GetColor() const { return sdl_color; };

	// Manipulating
	OwnedColor *ParseFromJson(const json& color_data);

};
