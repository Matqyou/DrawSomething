//
// Created by Matq on 26/01/2025.
//

#include "Connections.h"
#include "../../../components/element/Button.h"

namespace Main {
static LinkTexture sTextureConnectionsHeader("main_menu.friends.header");
static LinkTexture sTextureConnectionsDiscord("main_menu.friends.button");

Connections::Connections()
    : Frame(Vec2i(25, -25),
            Vec2i(0, 0),
            DONT_DRAW) {
    // Friends title
    auto friends_title = (new Frame(Vec2i(0, 0),
                                    Vec2i(560, 60),
                                    Vec2i(590, 75),
                                    Vec2i(-15, -15),
                                    sTextureConnectionsHeader.GetSDLTexture()))
        ->SetName("Title");

    // Games create
    auto friends_link = (new Button(Vec2i(0, 0),
                                    Vec2i(560, 60),
                                    VisualTexture(sTextureConnectionsDiscord.GetSDLTexture(),
                                                  Vec2d(1.0516473731077471, 1.2396694214876034),
                                                  Vec2d(-0.024555461473327687, 0.0)),
                                    VisualTexture(nullptr, Vec2d(1.0, 1.0), Vec2d(0.0, 0.0))))
        ->SetName("Link");

    // Friends window
    SetAlign(Align::DONT, Align::BOTTOM);
    SetFlex(Flex::HEIGHT);
    SetAdaptive(true, true);
    SetName("Friends");
    AddChildren({ friends_title, friends_link });
}

Connections::~Connections() {

}
}
