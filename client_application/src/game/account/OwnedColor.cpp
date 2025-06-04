//
// Created by Matq on 04/06/2025.
//

#include "OwnedColor.h"

OwnedColor::OwnedColor()
{
	color_id = 0;
	sdl_color.a = 255;
}

OwnedColor::~OwnedColor()
{

}

OwnedColor *OwnedColor::ParseFromJson(const json& color_data)
{
	color_id = color_data.value("color_id", color_id);
	sdl_color.r = color_data.value("r", sdl_color.r);
	sdl_color.g = color_data.value("g", sdl_color.g);
	sdl_color.b = color_data.value("b", sdl_color.b);
	return this;
}