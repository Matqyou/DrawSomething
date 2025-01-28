//
// Created by Matq on 26/01/2025.
//

#include "Header.h"

namespace Picking {
static void sHeaderRecoloring(TextureData* header) {
    header->SetColorMod(100, 190, 255);
}

static LinkTexture sTextureHeader("header", sHeaderRecoloring);

Header::Header() :
    Frame(Vec2i(0, 0),
          Vec2i(0, 80),
          VisualTexture(sTextureHeader.GetSDLTexture())) {

    SetFullyOccupy(true, false);
    SetName("Header");
}

Header::~Header() {

}
}
