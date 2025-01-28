//
// Created by Matq on 26/01/2025.
//

#include "Game.h"
#include "../../../../../structures/window_texture/WindowTexture.h"
#include "../../../../../components/element/TextElement.h"
#include "../../../../../components/element/Button.h"
#include "../../../../../CommonUI.h"
#include "../Games.h"
#include "Statistics.h"

namespace Main {

static TextureData* sGenerateButton(AssetsClass* assets,
                                    TTF_Font* font,
                                    const char* text,
                                    SDL_Color text_color,
                                    const Vec2f& button_size,
                                    SDL_Color button_color,
                                    float left_scaling,
                                    float right_scaling,
                                    float top_scaling,
                                    float bottom_scaling) {
    auto drawing = assets->GetDrawing();

    assets->GetTexture("window.sheet4")
        ->SetScaleMode(SDL_SCALEMODE_NEAREST);

    Sheet sheet;
    WindowTexture window_texture(&sheet, left_scaling, right_scaling, top_scaling, bottom_scaling);
    sheet.Generate(assets, assets->GetTexture("window.sheet4"));
    window_texture.Generate(assets, button_size);
    TextureData* generated = window_texture.GetGenerated();
    generated->SetColorMod(button_color);

    auto button_size_ = window_texture.GetGenerated()->GetSize();
    auto button_texture = assets->RenderTextBlended(font,
                                                    text, text_color);
    auto play_button = assets->CreateTexture(SDL_PIXELFORMAT_RGBA8888,
                                             SDL_TEXTUREACCESS_TARGET,
                                             (int)button_size_.x,
                                             (int)button_size_.y)
        ->SetScaleMode(SDL_SCALEMODE_NEAREST)
        ->FlagForAutomaticDeletion();
    drawing->SetRenderTarget(play_button);
    drawing->RenderTextureFullscreen(generated->SDLTexture(), nullptr);
    auto text_rect = Rectangles::CenterRelative(button_texture->GetSize(), button_size_);
    drawing->RenderTexture(button_texture->SDLTexture(), nullptr, text_rect);
    delete button_texture;

    return play_button;
}
static TextureData* sGeneratePlayButton(AssetsClass* assets) {
    return sGenerateButton(assets,
                           CommonUI::sFontDefault.GetTTFFont(),
                           "Play",
                           { 255, 255, 255, 255 },
                           Vec2f(100, 45),
                           { 0, 200, 0, 255 },
                           15,
                           15,
                           15,
                           15);
}
static TextureData* sGeneratePlayButtonPressed(AssetsClass* assets) {
    return sGenerateButton(assets,
                           CommonUI::sFontDefault.GetTTFFont(),
                           "Play",
                           { 255, 255, 255, 255 },
                           Vec2f(100, 45),
                           { 0, 180, 0, 255 },
                           15,
                           15,
                           15,
                           15);
}
static TextureData* sGenerateDeleteButton(AssetsClass* assets) {
    return sGenerateButton(assets,
                           CommonUI::sFontDefault.GetTTFFont(),
                           "Delete",
                           { 255, 255, 255, 255 },
                           Vec2f(100, 45),
                           { 220, 0, 0, 255 },
                           15,
                           15,
                           15,
                           15);
}
static TextureData* sGenerateDeleteButtonPressed(AssetsClass* assets) {
    return sGenerateButton(assets,
                           CommonUI::sFontDefault.GetTTFFont(),
                           "Delete",
                           { 255, 255, 255, 255 },
                           Vec2f(100, 45),
                           { 180, 0, 0, 255 },
                           15,
                           15,
                           15,
                           15);
}
static TextureData* sGenerateTurnFrame(AssetsClass* assets) {
    return sGenerateButton(assets,
                           CommonUI::sFontDefault.GetTTFFont(),
                           "",
                           { 0, 0, 0, 0 },
                           Vec2f(90, 90),
                           { 104, 195, 235, 255 },
                           10,
                           10,
                           10,
                           10);
}
static TextureData* sGenerateStatsButton(AssetsClass* assets) {
    return sGenerateButton(assets,
                           CommonUI::sFontSmallerBold.GetTTFFont(),
                           "Stats",
                           { 69, 19, 0, 255 },
                           Vec2f(60, 20),
                           { 255, 144, 71, 255 },
                           7,
                           7,
                           7,
                           7);
}
static TextureData* sGenerateStatsButtonPressed(AssetsClass* assets) {
    return sGenerateButton(assets,
                           CommonUI::sFontSmallerBold.GetTTFFont(),
                           "Stats",
                           { 69, 19, 0, 255 },
                           Vec2f(60, 20),
                           { 188, 104, 52, 255 },
                           7,
                           7,
                           7,
                           7);
}
static TextureData* sGenerateCloseButton(AssetsClass* assets) {
    return sGenerateButton(assets,
                           CommonUI::sFontSmallerBold.GetTTFFont(),
                           "Close",
                           { 69, 19, 0, 255 },
                           Vec2f(60, 20),
                           { 255, 144, 71, 255 },
                           7,
                           7,
                           7,
                           7);
}
static TextureData* sGenerateCloseButtonPressed(AssetsClass* assets) {
    return sGenerateButton(assets,
                           CommonUI::sFontSmallerBold.GetTTFFont(),
                           "Close",
                           { 69, 19, 0, 255 },
                           Vec2f(60, 20),
                           { 188, 104, 52, 255 },
                           7,
                           7,
                           7,
                           7);
}

static LinkTexture sTextureGamesContent("main_menu.games.game");

static PregenerateTexture sTexturePlayButton("main_menu.play_button", sGeneratePlayButton);
static PregenerateTexture sTexturePlayButtonPressed("main_menu.play_button_pressed", sGeneratePlayButtonPressed);
static PregenerateTexture sTextureDeleteButton("main_menu.delete_button", sGenerateDeleteButton);
static PregenerateTexture sTextureDeleteButtonPressed("main_menu.delete_button_pressed", sGenerateDeleteButtonPressed);
static PregenerateTexture sTextureTurnFrame("main_menu.turn_frame", sGenerateTurnFrame);
static PregenerateTexture sTextureStatsButton("main_menu.stats_button", sGenerateStatsButton);
static PregenerateTexture sTextureStatsButtonPressed("main_menu.stats_button_pressed", sGenerateStatsButtonPressed);
static PregenerateTexture sTextureCloseButton("main_menu.close_button", sGenerateCloseButton);
static PregenerateTexture sTextureCloseButtonPressed("main_menu.close_button_pressed", sGenerateCloseButtonPressed);

Game::Game()
    : Frame(Vec2i(0, 0),
            Vec2i(560, 0),
            DONT_DRAW) {
    auto assets = Assets::Get();
    auto pick_random = rand() % 2; // 0 - play, 1 - delete

    statistics = new Statistics();
    turn_number_ = -1;

    auto turn_text = (new TextElement(Vec2i(0, 5)))
        ->UpdateText(CommonUI::sFontDefault.GetTTFFont(), "turn", { 255, 255, 255, 255 })
        ->SetAlign(Align::CENTER, Align::DONT)
        ->SetName("TurnTitle");

    turn_number = (TextElement*)(new TextElement(Vec2i(0, -5)))
        ->UpdateText(CommonUI::sFontBigger.GetTTFFont(), "-1", { 255, 255, 255, 255 })
        ->SetAlign(Align::CENTER, Align::DONT)
        ->SetName("TurnNumber");

    auto stats = (Button*)(new Button(Vec2i(0, -5),
                                      Vec2i(60, 20),
                                      VisualTexture(sTextureStatsButton.GetSDLTexture()),
                                      VisualTexture(sTextureStatsButtonPressed.GetSDLTexture())))
        ->SetAlign(Align::CENTER, Align::DONT)
        ->SetName("StatsButton");
    stats->SetCallback([this, stats]() {
        this->statistics->SetEnabled(!this->statistics->enabled);
        if (this->statistics->enabled) {
            stats->SetVisualTexture(VisualTexture(sTextureCloseButton.GetSDLTexture()));
            stats->SetPressedVisualTexture(VisualTexture(sTextureCloseButtonPressed.GetSDLTexture()));
        } else {
            stats->SetVisualTexture(VisualTexture(sTextureStatsButton.GetSDLTexture()));
            stats->SetPressedVisualTexture(VisualTexture(sTextureStatsButtonPressed.GetSDLTexture()));
        }

        this->parent->parent->parent->Refresh();
    });

    auto turn_frame = (new Frame(Vec2i(0, -3),
                                 Vec2i(0, 0),
                                 DONT_DRAW))
        ->SetAlign(Align::CENTER, Align::CENTER)
        ->SetFlex(Flex::HEIGHT)
        ->SetAdaptive(true, true)
        ->SetName("TurnFrame")
        ->AddChildren({ turn_text, turn_number, stats });

    auto turn = (new Frame(Vec2i(0, 0),
                           Vec2i(70, 90),
                           VisualTexture(sTextureTurnFrame.GetSDLTexture())))
        ->SetAlign(Align::DONT, Align::CENTER)
        ->SetName("Turn")
        ->AddChildren({ turn_frame });

    auto random_cat = assets->GetTexture(Strings::FString("profile_pictures.cat%d", 1 + rand() % 12));
    auto profile_picture = (new Frame(Vec2i(0, 0), Vec2i(66, 66), VisualTexture(random_cat->SDLTexture())))
        ->SetName("ProfilePicture");

    const char* usernames[10] = {
        "Ethan S.",
        "Sophia S.",
        "Lucas S.",
        "Mia S.",
        "Oliver S.",
        "Amelia S.",
        "Liam S.",
        "Isabella S.",
        "Noah S.",
        "Charlotte S.",
    };
    auto random_username = usernames[std::rand() % 10];
    auto picture_text = (new TextElement(Vec2i(3, 0)))
        ->UpdateText(CommonUI::sFontSmallerBold.GetTTFFont(), random_username, { 255, 81, 145, 255 })
        ->SetName("PictureName");

    auto picture_frame = (new Frame(Vec2i(0, 0),
                                    Vec2i(0, 0),
                                    DONT_DRAW))
        ->SetAlign(Align::DONT, Align::CENTER)
        ->SetFlex(Flex::HEIGHT)
        ->SetAdaptive(true, true)
        ->SetName("PictureFrame")
        ->AddChildren({ profile_picture, picture_text });

    auto text = pick_random == 0 ? "ready" : "waiting";
    auto state_text = (new TextElement(Vec2i(0, 0)))
        ->UpdateText(CommonUI::sFontDefault.GetTTFFont(), text, { 150, 150, 150, 255 })
        ->SetAlign(Align::CENTER, Align::DONT)
        ->SetName("StateText");

    auto description = pick_random == 0 ? "your move" : "waiting their move";
    auto state_description = (new TextElement(Vec2i(0, 0)))
        ->UpdateText(CommonUI::sFontSmaller.GetTTFFont(), description, { 150, 150, 150, 255 })
        ->SetAlign(Align::CENTER, Align::DONT)
        ->SetName("StateDescription");

    auto state_frame = (new Frame(Vec2i(0, 0),
                                  Vec2i(0, 0),
                                  DONT_DRAW))
        ->SetAlign(Align::DONT, Align::CENTER)
        ->SetFullyOccupy(false, true)
        ->SetOccupy(true, false)
        ->SetFlex(Flex::HEIGHT)
        ->SetAdaptive(false, true)
        ->SetName("StateFrame")
        ->AddChildren({ state_text, state_description });

    SDL_Texture* sdl_texture = (pick_random == 0 ? sTexturePlayButton : sTextureDeleteButton).GetSDLTexture();
    SDL_Texture* sdl_texture_pressed =
        (pick_random == 0 ? sTexturePlayButtonPressed : sTextureDeleteButtonPressed).GetSDLTexture();
    auto game_button = (Button*)(new Button(Vec2i(-10, 0),
                                            Vec2i(100, 45),
                                            VisualTexture(sdl_texture),
                                            VisualTexture(sdl_texture_pressed)))
        ->SetAlign(Align::DONT, Align::CENTER)
        ->SetName("PlayButton");
    game_button->SetCallback([this]() {
        this->SetEnabled(false);
        this->FlagToDestroy();

        this->parent->parent->parent->Refresh();
    });

    auto info_frame = (new Frame(Vec2i(5, 0),
                                 Vec2i(0, 102),
                                 DONT_DRAW))
        ->SetFlex(Flex::WIDTH, 5)
        ->SetFullyOccupy(true, false)
        ->SetName("InfoFrame")
        ->AddChildren({ turn, picture_frame, state_frame, game_button });

    auto game_frame = (new Frame(Vec2i(0, 0),
                                 Vec2i(560, 102),
                                 VisualTexture(sTextureGamesContent.GetSDLTexture(),
                                               Vec2d(1.0516473731077471, 1.0),
                                               Vec2d(-0.024555461473327687, 0.0))))
        ->SetName("GameFrame")
        ->AddChildren({ info_frame });

    SetTurnNumber(std::rand() % 70);

    SetFlex(Flex::HEIGHT);
    SetAdaptive(false, true);
    SetName("GameContent");
    AddChildren({ game_frame, statistics });
}

Game::~Game() {

}

void Game::SetTurnNumber(int number) {
    turn_number_ = number;
    turn_number->UpdateText(CommonUI::sFontBigger.GetTTFFont(),
                            Strings::FString("%d", number).c_str(),
                            { 255, 255, 255, 255 });
}
}
