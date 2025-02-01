//
// Created by Matq on 24/12/2024.
//

#include "MainMenu.h"
#include "../confirmation/ConfirmationScreen.h"
#include "../loading/LoadingScreen.h"
#include "components/games/Games.h"
#include "components/Connections.h"
#include "components/Profile.h"

MainMenu::MainMenu()
    : FullscreenMenu() {
    name = L"MainMenu";

    auto loading_screen = new LoadingScreen();
    auto confirmation = new ConfirmationScreen();
    auto settings = new Main::Settings();
    auto header = new Main::Header(settings);
    auto games = new Main::Games(confirmation, loading_screen);
    auto profile = new Main::Profile();
    auto friends = new Main::Connections();

    SetColor(94, 152, 224, 255);
    SetName("MainMenu");
    AddChildren({ header, games, profile, friends, loading_screen, confirmation, settings });

    confirmation->Prompt("You want to delete this game?");

    RefreshMenu();
}
