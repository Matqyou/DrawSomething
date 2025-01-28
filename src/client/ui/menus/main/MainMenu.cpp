//
// Created by Matq on 24/12/2024.
//

#include "MainMenu.h"
#include "../../../core/Application.h"
#include "components/Profile.h"
#include "components/games/Games.h"
#include "components/Connections.h"

MainMenu::MainMenu()
    : FullscreenMenu() {
    name = L"MainMenu";

    auto settings = new Main::Settings();
    auto header = new Main::Header(settings);
    auto games = new Main::Games();
    auto profile = new Main::Profile();
    auto friends = new Main::Connections();

    SetColor(94, 152, 224, 255);
    SetName("MainMenu");
    AddChildren({ header, games, profile, friends, settings });
    RefreshMenu();
}
