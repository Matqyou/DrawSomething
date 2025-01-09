//
// Created by Matq on 24/12/2024.
//

#include "MainMenu.h"
#include "../../core/Application.h"
#include "../components/element/Button.h"

LoadedTexture MainMenu::texture_header("header");

LoadedTexture MainMenu::texture_game_header("main_menu.games.header");
LoadedTexture MainMenu::texture_game_content("main_menu.games.game");
LoadedTexture MainMenu::texture_game_create("main_menu.games.create");

LoadedTexture MainMenu::texture_profile_base("main_menu.profile.base");

LoadedTexture MainMenu::texture_friends_header("main_menu.friends.header");
LoadedTexture MainMenu::texture_friends_link("main_menu.friends.button");

MainMenu::MainMenu()
    : Frame(Vec2i(0, 0), Vec2i(0, 0), DONT_DRAW) {
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
        ->SetFlex(FLEX_HEIGHT)
        ->SetAdaptive(true, true)
        ->SetName("Games")
        ->SetChildren({ games_title, games_content, games_content2, games_create });

    // Profile window
    auto profile = (new Frame(Vec2i(-25, 100),
                              Vec2i(377, 532),
                              Vec2i(405, 560),
                              Vec2i(-14, -14),
                              texture_profile_base.GetTexture()))
        ->SetFlex(FLEX_HEIGHT)
        ->SetAlign(ALIGN_RIGHT, DONT_ALIGN)
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
        ->SetFlex(FLEX_HEIGHT)
        ->SetAdaptive(true, true)
        ->SetName("Friends")
        ->SetChildren({ friends_title, friends_link });

    // Compose into the main frame
    std::initializer_list<Element*> game_elements = { header, games, profile, friends };

    SetChildren(game_elements)->SetName("MainMenu");
    SetSize(Vec2i(0, 0),
            Application::Get()->GetResolution(),
            Application::Get()->GetResolution());
}

void MainMenu::HandleEvent(SDL_Event& sdl_event, EventContext& event_summary) {
    switch (sdl_event.type) {
        case SDL_WINDOWEVENT: {
            if (sdl_event.window.event == SDL_WINDOWEVENT_RESIZED) {
                SetSize(Vec2i(0, 0),
                        Application::Get()->GetResolution(),
                        Application::Get()->GetResolution());
            }

            break;
        }
    }

    HandleEventChildren(sdl_event, event_summary);
}