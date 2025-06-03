//
// Created by Matq on 26/01/2025.
//

#include "Header.h"

namespace Picking {
static PregenerateTexture sTextureHeader("picking_menu_header", [](AssetsClass *assets) -> Texture *
{
	auto header_texture = assets->GetTexture("header");
	return header_texture->CopyTexture(SDL_TEXTUREACCESS_TARGET)
		->SetColorMod(216, 112, 43);
});
static LinkTexture sTextureCard("picking.card");

Header::Header() :
    Frame() {

    this->SetSize(Vec2i(0, 58));
    this->SetTexture(sTextureHeader.GetTexture()->SetColorMod(216, 112, 43));
    this->SetDraw(DRAW_TEXTURE);
    this->SetFullyOccupy(true, false);
    this->SetName("Header");
}

Header::~Header() {

}
}
