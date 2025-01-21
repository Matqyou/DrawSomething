//
// Created by Matq on 24/12/2024.
//

#include "MainMenu.h"
#include "../../../core/Application.h"
#include "../../components/element/Button.h"

static auto sCallbackHeader = [](Texture* texture) {
    texture->SetColorMod(100, 190, 255);
};

PreloadTexture MainMenu::texture_header("header", sCallbackHeader);

PreloadTexture MainMenu::texture_game_header("main_menu.games.header");
PreloadTexture MainMenu::texture_game_content("main_menu.games.game");
PreloadTexture MainMenu::texture_game_create("main_menu.games.create");

PreloadTexture MainMenu::texture_profile_base("main_menu.profile.base");

PreloadTexture MainMenu::texture_friends_header("main_menu.friends.header");
PreloadTexture MainMenu::texture_friends_link("main_menu.friends.button");

MainMenu::MainMenu()
    : FullscreenMenu() {
    // Header
    auto header = (new Frame(Vec2i(0, 0),
                             Vec2i(0, 58),
                             Vec2i(0, 80),
                             Vec2i(0, 0),
                             texture_header.GetTexture()))
        ->SetFullyOccupy(true, false)
        ->SetName("Header");

    // Games title
    auto games_title = (new Frame(Vec2i(0, 0),
                                  Vec2i(560, 60),
                                  Vec2i(590, 75),
                                  Vec2i(-15, -15),
                                  texture_game_header.GetTexture()))
        ->SetName("Title");

    // Games overview
    auto games_content = (new Frame(Vec2i(0, 0),
                                    Vec2i(560, 102),
                                    Vec2i(590, 102),
                                    Vec2i(-15, 0),
                                    texture_game_content.GetTexture()))
        ->SetName("Content");
    auto games_content2 = (new Frame(Vec2i(0, 0),
                                     Vec2i(560, 102),
                                     Vec2i(590, 102),
                                     Vec2i(-15, 0),
                                     texture_game_content.GetTexture()))
        ->SetName("Content");

    // Games create
    auto games_create = (new Button(Vec2i(0, 0),
                                    Vec2i(560, 60),
                                    Vec2i(590, 75),
                                    Vec2i(-15, 0),
                                    texture_game_create.GetTexture()))
        ->SetName("Create");

    // Games window
    auto games = (new Frame(Vec2i(25, 100),
                            Vec2i(0, 0),
                            DONT_DRAW))
        ->SetFlex(Flex::HEIGHT)
        ->SetAdaptive(true, true)
        ->SetName("Games")
        ->AddChildren({ games_title, games_content, games_content2, games_create });

    // Profile window
    auto profile = (new Frame(Vec2i(-25, 100),
                              Vec2i(377, 532),
                              Vec2i(405, 560),
                              Vec2i(-14, -14),
                              texture_profile_base.GetTexture()))
        ->SetFlex(Flex::HEIGHT)
        ->SetAlign(Align::RIGHT, Align::DONT)
        ->SetName("Profile");

    // Friends title
    auto friends_title = (new Frame(Vec2i(0, 0),
                                    Vec2i(560, 60),
                                    Vec2i(590, 75),
                                    Vec2i(-15, -15),
                                    texture_friends_header.GetTexture()))
        ->SetName("Title");

    // Games create
    auto friends_link = (new Button(Vec2i(0, 0),
                                    Vec2i(560, 60),
                                    Vec2i(590, 75),
                                    Vec2i(-15, 0),
                                    texture_friends_link.GetTexture()))
        ->SetName("Link");

    // Friends window
    auto friends = (new Frame(Vec2i(25, 450),
                              Vec2i(0, 0),
                              DONT_DRAW))
        ->SetFlex(Flex::HEIGHT)
        ->SetAdaptive(true, true)
        ->SetName("Friends")
        ->AddChildren({ friends_title, friends_link });

    // Compose into the main frame
    std::initializer_list<Element*> game_elements = { header, games, profile, friends };

    AddChildren(game_elements)->SetName("MainMenu");
    SetColor(94, 152, 224, 255);
    UpdateElement(Vec2i(0, 0),
                  Application::Get()->GetResolution(),
                  Application::Get()->GetResolution());
}
