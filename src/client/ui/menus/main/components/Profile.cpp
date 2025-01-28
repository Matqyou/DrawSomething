//
// Created by Matq on 26/01/2025.
//

#include "Profile.h"
#include "../../../components/element/TextElement.h"
#include "../../../CommonUI.h"

namespace Main {
static LinkTexture sTextureProfileBase("main_menu.profile.base");

Profile::Profile()
 : Frame(Vec2i(-25, 100),
         Vec2i(377, 532),
         Vec2i(405, 560),
         Vec2i(-14, -14),
         sTextureProfileBase.GetSDLTexture()) {
    auto assets = Assets::Get();

    TextureData* random_cat = assets->GetTexture(Strings::FString("profile_pictures.cat%d", 1 + rand() % 12));
    auto profile_picture = (new Frame(Vec2i(0, 0),
                                      Vec2i(167, 167),
                                      VisualTexture(random_cat->SDLTexture())))
        ->SetName("ProfilePicture");

    // Profile username
    auto profile_username = (new TextElement(Vec2i(0, 0)))
        ->UpdateText(CommonUI::sFontSmallerBold.GetTTFFont(), "Username S.", { 255, 0, 0, 255 })
        ->SetAlign(Align::CENTER, Align::DONT);

    // Profile email
    auto profile_email = (new TextElement(Vec2i(0, 0)))
        ->UpdateText(CommonUI::sFontSmallerBold.GetTTFFont(), "email@domain.com", { 150, 150, 150, 255 })
        ->SetAlign(Align::CENTER, Align::DONT);

    auto profile_info = (new Frame(Vec2i(0, 0),
                                   Vec2i(0, 0),
                                   DONT_DRAW))
        ->SetFlex(Flex::HEIGHT)
        ->SetAdaptive(false, true)
        ->SetOccupy(true, false)
        ->SetName("ProfileInfo")
        ->AddChildren({ profile_username, profile_email });

    auto profile_frame = (new Frame(Vec2i(10, 68),
                                    Vec2i(356, 0),
                                    DONT_DRAW))
        ->SetFlex(Flex::WIDTH)
        ->SetAdaptive(false, true)
        ->SetName("ProfileFrame")
        ->AddChildren({ profile_picture, profile_info });

    SetAlign(Align::RIGHT, Align::DONT);
    SetName("Profile");
    AddChildren({ profile_frame });
}

Profile::~Profile() {

}
}
