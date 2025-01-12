//
// Created by Matq on 12/01/2025.
//

#include "IngameMenu.h"
#include "../../core/Application.h"

PreloadTexture IngameMenu::game_header_turn("game.header.turn");
PreloadTexture IngameMenu::game_header_picture("game.header.picture");
PreloadTexture IngameMenu::game_header_background("game.header.background");

LinkFont IngameMenu::sFontDefault("default");
LinkFont IngameMenu::sFontDefaultBiggest("fredoka.biggest");
LinkFont IngameMenu::sFontDefaultBigger("fredoka.big");
LinkFont IngameMenu::sFontDefaultSmaller("fredoka.small");

IngameMenu::IngameMenu()
    : FullscreenMenu() {
    auto drawing = Application::Get()->GetDrawing();

    // Turn title text
    Texture* turn_title_render;
    {
        SDL_Surface* sdl_surface =
            TTF_RenderText_Blended(sFontDefaultSmaller.GetFont()->TTFFont(),
                                   "turn",
                                   strlen("turn"),
                                   { 255, 255, 255, 255 });
        turn_title_render = Assets::Get()->TextureFromSurface(sdl_surface)
            ->FlagForAutomaticDeletion();
        SDL_DestroySurface(sdl_surface);
    }
    auto turn_title = (new Frame(Vec2i(0, 4),
                                 Vec2i(turn_title_render->GetWidth(), turn_title_render->GetHeight()),
                                 turn_title_render))
        ->SetName("TurnTitle", false);

    // Turn number text
    Texture* turn_number_render;
    {
        SDL_Surface* sdl_surface =
            TTF_RenderText_Blended(sFontDefaultBigger.GetFont()->TTFFont(), "6", strlen("6"), { 255, 255, 255, 255 });
        turn_number_render = Assets::Get()->TextureFromSurface(sdl_surface)
            ->FlagForAutomaticDeletion();
        SDL_DestroySurface(sdl_surface);
    }
    auto turn_number = (new Frame(Vec2i(0, -4),
                                  Vec2i(turn_number_render->GetWidth(), turn_number_render->GetHeight()),
                                  turn_number_render))
        ->SetAlign(ALIGN_CENTER, DONT_ALIGN)
        ->SetName("TurnNumber", false);

    // Turn text frame
    auto turn_info_frame = (new Frame(Vec2i(0, 0), Vec2i(50, 30), DONT_DRAW))
        ->SetFlex(FLEX_HEIGHT)
        ->SetAlign(ALIGN_CENTER, ALIGN_CENTER)
        ->SetAdaptive(true, true)
        ->SetName("TurnInfo", false)
        ->AddChildren({ turn_title, turn_number });

    // Turn display
    auto turn_display = (new Frame(Vec2i(0, 0), Vec2i(73, 73), game_header_turn.GetTexture()))
        ->SetName("Turn", false)
        ->AddChildren({ turn_info_frame });

    // Profile picutre
    auto picture = (new Frame(Vec2i(0, 0), Vec2i(68, 73), game_header_picture.GetTexture()))
        ->SetName("Picture");

    // Game info text1
    Texture* game_info_text1;
    {
        SDL_Surface* sdl_surface =
            TTF_RenderText_Blended(sFontDefaultSmaller.GetFont()->TTFFont(),
                                   "You are guessing",
                                   strlen("You are guessing"),
                                   { 0, 0, 0, 255 });
        game_info_text1 = Assets::Get()->TextureFromSurface(sdl_surface)
            ->FlagForAutomaticDeletion();
        SDL_DestroySurface(sdl_surface);
    }
    auto text1 =
        (new Frame(Vec2i(0, 0), Vec2i(game_info_text1->GetWidth(), game_info_text1->GetHeight()), game_info_text1))
            ->SetName("GuessingText", false);

    // Game info text2
    Texture* game_info_text2;
    {
        SDL_Surface* sdl_surface =
            TTF_RenderText_Blended(sFontDefaultSmaller.GetFont()->TTFFont(),
                                   "Name S.'s drawing.",
                                   strlen("Name S.'s drawing."),
                                   { 0, 0, 0, 255 });
        game_info_text2 = Assets::Get()->TextureFromSurface(sdl_surface);
        SDL_DestroySurface(sdl_surface);
    }
    auto text2 =
        (new Frame(Vec2i(0, 0), Vec2i(game_info_text2->GetWidth(), game_info_text2->GetHeight()), game_info_text2))
            ->SetName("DrawingText", false);

    // Game info text frame
    auto round_info_frame = (new Frame(Vec2i(10, 0), Vec2i(50, 30), DONT_DRAW))
        ->SetFlex(FLEX_HEIGHT)
        ->SetAlign(ALIGN_LEFT, ALIGN_CENTER)
        ->SetAdaptive(true, true)
        ->SetName("RoundInfoText")
        ->AddChildren({ text1, text2 });

    // Game info
    auto round_info = (new Frame(Vec2i(0, 0), Vec2i(0, 73), game_header_background.GetTexture()))
        ->SetOccupy(true, false)
        ->SetName("RoundInfo")
        ->AddChildren({ round_info_frame });

    // Header
    header = (Frame*)(new Frame(Vec2i(0, 0), Vec2i(0, 73), DONT_DRAW))
        ->SetFullyOccupy(true, false)
        ->SetFlex(FLEX_WIDTH)
        ->SetName("Header", false)
        ->AddChildren({ turn_display, picture, round_info });

    // Canvas
    canvas = (Canvas*)(new Canvas(Vec2i(0, 0),
                                  Vec2i(0, 0),
                                  DRAW_RECT))
//        ->SetCallback([this]() {
//            this->AddChildren({ });
//            this->Refresh();
//        })
        ->SetFullyOccupy(true, false)
        ->SetOccupy(false, true)
        ->SetColor(255, 255, 255, 255)
        ->SetName("Canvas", false);

    panel = new Panel();
    color_selector = (Frame*)(new Frame(Vec2i(0, 0), Vec2i(0, 50), DONT_DRAW))
        ->SetFullyOccupy(true, false)
        ->SetFlex(FLEX_WIDTH)
        ->SetName("ColorSelector", false);
    brush_selector = (Frame*)(new Frame(Vec2i(0, 0), Vec2i(0, 50), DONT_DRAW))
        ->SetFullyOccupy(true, false)
        ->SetFlex(FLEX_WIDTH)
        ->SetName("BrushSelector", false);

    SetColor(230, 230, 230, 255);
    SetFlex(FLEX_HEIGHT);
    AddChildren({ header, canvas })->SetName("GuessingMenu", false);
    Refresh();

    PrepareDraw();
}

void IngameMenu::PrepareGuess() {
    SetChildren({ header, canvas });
    Refresh();
    canvas->SetIntro(INTRO_GUESS);
    canvas->SetCallback([this]() {
        this->SetChildren({ header, canvas, panel });
        this->Refresh();
    });
}

void IngameMenu::PrepareWatch() {
    SetChildren({ header, canvas });
    Refresh();
    canvas->SetIntro(INTRO_WATCH);
    canvas->SetCallback([this]() {
        this->SetChildren({ header, canvas, panel });
        this->Refresh();
    });
}

void IngameMenu::PrepareDraw() {
    SetChildren({ header, canvas });
    Refresh();
    canvas->SetIntro(INTRO_DRAW);
    canvas->SetCallback([this]() {
        this->SetChildren({ header, color_selector, canvas, brush_selector });
        this->Refresh();
    });
}
