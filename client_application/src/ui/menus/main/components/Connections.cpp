//
// Created by Matq on 26/01/2025.
//

#include "Connections.h"
#include "ui/components/element/Button.h"

namespace Main {
static LinkTexture sTextureConnectionsHeader("main_menu.friends.header");
static LinkTexture sTextureConnectionsDiscord("main_menu.friends.button");

Connections::Connections()
    : Frame() {
    // Friends title
    auto friends_title = (new Frame())
        ->SetSize(Vec2i(560, 60))
        ->SetTexture(sTextureConnectionsHeader.GetTexture())
        ->SetDraw(DRAW_TEXTURE)
        ->SetName("Title");

    // Games create
    auto friends_link = (new Button(sTextureConnectionsDiscord.GetTexture(),
//                                                  Vec2d(1.0516473731077471, 1.2396694214876034),
//                                                  Vec2d(-0.024555461473327687, 0.0)),
                                    nullptr))
        ->SetSize(Vec2i(560, 60))
        ->SetName("Link");

    // Friends window
    this->SetRelative(Vec2i(25, -25));
    this->SetAlign(Align::DONT, Align::BOTTOM);
    this->SetFlex(Flex::HEIGHT);
    this->SetAdaptive(true, true);
    this->SetName("Friends");
    this->AddChildren({ friends_title, friends_link });
}

Connections::~Connections() {

}
}
