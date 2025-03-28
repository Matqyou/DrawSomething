//
// Created by Matq on 26/01/2025.
//

#include "Games.h"
#include "../../../../components/element/Button.h"
#include "game/Game.h"

namespace Main {
static LinkTexture sTextureGamesHeader("main_menu.games.header");
static LinkTexture sTextureGamesCreate("main_menu.games.create");
static LinkTexture sTextureGamesCreatePressed("main_menu.games.create_pressed");

Games::Games(ConfirmationScreen* confirmation_screen, LoadingScreen* loading_screen)
    : Frame(Vec2i(25, 100),
            Vec2i(0, 0),
            DONT_DRAW) {
    confirmation_screen_ = confirmation_screen;
    loading_screen_ = loading_screen;

    // Games title
    auto games_title = (new Frame(Vec2i(0, 0),
                                  Vec2i(560, 60),
                                  Vec2i(590, 75),
                                  Vec2i(-15, -15),
                                  sTextureGamesHeader.GetSDLTexture()))
        ->SetName("Title");

    // Temp game info
    game_info_example1 = GameInfo::GetExample1();
    game_info_example2 = GameInfo::GetExample2();

    // Games overview
    auto games_content = new Main::Game(this);
    auto games_content2 = new Main::Game(this);
    auto games_content3 = new Main::Game(this);

    games_content->UpdateInfo(&game_info_example1);
    games_content2->UpdateInfo(&game_info_example2);

    games = (Frame*)(new Frame())
        ->SetFlex(Flex::HEIGHT)
        ->SetAdaptive(true, true)
        ->SetName("Games")
        ->AddChildren({ games_content, games_content2, games_content3 });
    SortGames();

    // Games create
    auto games_create = (Button*)(new Button(Vec2i(0, 0),
                                             Vec2i(560, 60),
                                             VisualTexture(sTextureGamesCreate.GetSDLTexture(),
                                                           Vec2d(1.0516473731077471, 1.2396694214876034),
                                                           Vec2d(-0.024555461473327687, 0.0)),
                                             VisualTexture(sTextureGamesCreatePressed.GetSDLTexture(),
                                                           Vec2d(1.0516473731077471, 1.2396694214876034),
                                                           Vec2d(-0.024555461473327687, 0.0))))
        ->SetName("Create");
    games_create->SetCallback([this]() {
        this->games->AddChildren({ new Main::Game(this) });
        this->SortGames();
        this->parent->Refresh();
    });

    SetFlex(Flex::HEIGHT);
    SetAdaptive(true, true);
    SetName("GamesTab");
    AddChildren({ games_title, games, games_create });
}

Games::~Games() {

}

void Games::SortGames() {
    std::sort(games->children.begin(), games->children.end(), [](Element* a, Element* b) {
        auto A = dynamic_cast<Game*>(a);
        auto B = dynamic_cast<Game*>(b);

        if (A->GetGameInfo() == nullptr) return false;
        if (B->GetGameInfo() == nullptr) return true;

        return A->GetGameInfo()->game_turn > B->GetGameInfo()->game_turn;
    });
}
}
