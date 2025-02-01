//
// Created by Matq on 26/01/2025.
//

#include "Game.h"
#include "../../../../../structures/window_texture/WindowTexture.h"
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
static TextureData* sGenerateUndefinedButton(AssetsClass* assets) {
    return sGenerateButton(assets,
                           CommonUI::sFontDefault.GetTTFFont(),
                           "NaN",
                           { 255, 255, 255, 255 },
                           Vec2f(100, 45),
                           { 220, 0, 220, 255 },
                           15,
                           15,
                           15,
                           15);
}
static TextureData* sGenerateUndefinedButtonPressed(AssetsClass* assets) {
    return sGenerateButton(assets,
                           CommonUI::sFontDefault.GetTTFFont(),
                           "NaN",
                           { 255, 255, 255, 255 },
                           Vec2f(100, 45),
                           { 180, 0, 180, 255 },
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
static PregenerateTexture sTextureUndefinedButton("main_menu.undefined_button", sGenerateUndefinedButton);
static PregenerateTexture
    sTextureUndefinedButtonPressed("main_menu.undefined_button_pressed", sGenerateUndefinedButtonPressed);
static PregenerateTexture sTextureTurnFrame("main_menu.turn_frame", sGenerateTurnFrame);
static PregenerateTexture sTextureStatsButton("main_menu.stats_button", sGenerateStatsButton);
static PregenerateTexture sTextureStatsButtonPressed("main_menu.stats_button_pressed", sGenerateStatsButtonPressed);
static PregenerateTexture sTextureCloseButton("main_menu.close_button", sGenerateCloseButton);
static PregenerateTexture sTextureCloseButtonPressed("main_menu.close_button_pressed", sGenerateCloseButtonPressed);

LinkTexture Game::sTextureDefaultProfilePicture("icons.icon");

void Game::SetTurnNumber(int turn) {
    turn_number->UpdateText(CommonUI::sFontBigger.GetTTFFont(),
                            Strings::FString("%d", turn).c_str(),
                            { 255, 255, 255, 255 });
}
void Game::SetProfilePicture(TextureData* texture_data) {
    TextureData* profile_picture_ = texture_data != nullptr ? texture_data : sTextureDefaultProfilePicture.GetTexture();
    profile_picture->SetVisualTexture(VisualTexture(profile_picture_->SDLTexture()));
    profile_picture->UpdateVisualTexture(); //
}
void Game::SetUsername(const char* username) {
    picture_text->UpdateText(CommonUI::sFontSmallerBold.GetTTFFont(),
                             username,
                             { 255, 81, 145, 255 });
}
void Game::SetState(GameInfo::GameState game_state) {
    const char* game_state_text = game_state == GameInfo::GameState::YOUR_MOVE ? "ready" : "waiting";
    const char* description_text = game_state == GameInfo::GameState::YOUR_MOVE ? "your move" : "waiting their move";
    state_text->UpdateText(CommonUI::sFontDefault.GetTTFFont(), game_state_text, { 150, 150, 150, 255 });
    state_description->UpdateText(CommonUI::sFontSmaller.GetTTFFont(), description_text, { 150, 150, 150, 255 });

    SDL_Texture* sdl_texture = (game_state == GameInfo::GameState::YOUR_MOVE ?
        sTexturePlayButton : sTextureDeleteButton).GetSDLTexture();
    SDL_Texture* sdl_texture_pressed = (game_state == GameInfo::GameState::YOUR_MOVE ?
        sTexturePlayButtonPressed : sTextureDeleteButtonPressed).GetSDLTexture();
    game_button->SetVisualTexture(VisualTexture(sdl_texture));
    game_button->SetPressedVisualTexture(VisualTexture(sdl_texture_pressed));

        if (game_state == GameInfo::GameState::YOUR_MOVE) {
        game_button->SetCallback([this]() {
            LoadingScreen* loading_screen = games_->loading_screen_;
            loading_screen->StartShowing();
        });
    } else {
        game_button->SetCallback([this]() {
            ConfirmationScreen* confirmation_screen = games_->confirmation_screen_;
            confirmation_screen->Prompt("You want to delete this game?");
//            this->SetEnabled(false);
//            this->FlagToDestroy();
//
//            this->parent->parent->parent->Refresh();
        });
    }
}

Game::Game(Games* games)
    : Frame(Vec2i(0, 0),
            Vec2i(560, 0),
            DONT_DRAW) {
    games_ = games;
    game_info_ = nullptr;

    auto assets = Assets::Get();

    statistics = new Statistics();

    auto turn_text = (new TextElement(Vec2i(0, 5)))
        ->UpdateText(CommonUI::sFontDefault.GetTTFFont(), "turn", { 255, 255, 255, 255 })
        ->SetAlign(Align::CENTER, Align::DONT)
        ->SetName("TurnTitle");

    turn_number = (TextElement*)(new TextElement(Vec2i(0, -5)))
        ->UpdateText(CommonUI::sFontBigger.GetTTFFont(), "NaN", { 255, 255, 255, 255 })
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

    profile_picture = (Frame*)(new Frame(Vec2i(0, 0),
                                         Vec2i(66, 66),
                                         VisualTexture(assets->GetTexture("icons.icon")->SDLTexture())))
        ->SetName("ProfilePicture");

    picture_text = (TextElement*)(new TextElement(Vec2i(3, 0)))
        ->UpdateText(CommonUI::sFontSmallerBold.GetTTFFont(), "NaN", { 255, 81, 145, 255 })
        ->SetName("PictureName");

    auto picture_frame = (new Frame(Vec2i(0, 0),
                                    Vec2i(0, 0),
                                    DONT_DRAW))
        ->SetAlign(Align::DONT, Align::CENTER)
        ->SetFlex(Flex::HEIGHT)
        ->SetAdaptive(true, true)
        ->SetName("PictureFrame")
        ->AddChildren({ profile_picture, picture_text });

    state_text = (TextElement*)(new TextElement(Vec2i(0, 0)))
        ->UpdateText(CommonUI::sFontDefault.GetTTFFont(), "state", { 150, 150, 150, 255 })
        ->SetAlign(Align::CENTER, Align::DONT)
        ->SetName("StateText");

    state_description = (TextElement*)(new TextElement(Vec2i(0, 0)))
        ->UpdateText(CommonUI::sFontSmaller.GetTTFFont(), "description", { 150, 150, 150, 255 })
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

    game_button = (Button*)(new Button(Vec2i(-10, 0),
                                       Vec2i(100, 45),
                                       VisualTexture(sTextureUndefinedButton.GetSDLTexture()),
                                       VisualTexture(sTextureUndefinedButtonPressed.GetSDLTexture())))
        ->SetAlign(Align::DONT, Align::CENTER)
        ->SetName("PlayButton");

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

    SetFlex(Flex::HEIGHT);
    SetAdaptive(false, true);
    SetName("GameContent");
    AddChildren({ game_frame, statistics });
}

Game::~Game() {

}

void Game::UpdateInfo(GameInfo* game_info) {
    game_info_ = game_info;
    if (game_info != nullptr) {
        SetTurnNumber(game_info->game_turn);
        SetProfilePicture(game_info->players[1].profile_picture);
        SetUsername(game_info->players[1].username.c_str());
        SetState(game_info->game_state);

        statistics->UpdateInfo(game_info);
    }
}
}
