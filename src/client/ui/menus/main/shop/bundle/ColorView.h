//
// Created by Matq on 21/05/2025.
//

#pragma once

#include "client/ui/components/element/Frame.h"

class ColorView : public Frame
{
private:
	int color_id;
	int rarity;
	SDL_Color color;

	Texture* view_texture;

	// Manipulating
	void UpdateTexture();

public:
	ColorView(int color_id, int rarity, Uint8 r, Uint8 g, Uint8 b);
	~ColorView() override;

	// Options
//	ColorView *UpdateInfo(int color_id, int rarity, Uint8 r, Uint8 g, Uint8 b);

};
