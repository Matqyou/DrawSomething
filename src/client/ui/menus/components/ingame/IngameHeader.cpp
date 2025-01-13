//
// Created by Matq on 13/01/2025.
//

#include "IngameHeader.h"
#include "../../../../core/Application.h"

PreloadTexture IngameHeader::game_header_turn("game.header.turn");
PreloadTexture IngameHeader::game_header_picture("game.header.picture");
PreloadTexture IngameHeader::game_header_background("game.header.background");

LinkFont IngameHeader::sFontDefaultBigger("fredoka.big");
LinkFont IngameHeader::sFontDefaultSmaller("fredoka.small");

IngameHeader::IngameHeader()
    : Frame(Vec2i(0, 0), Vec2i(0, 73), DONT_DRAW) {
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
    auto picture = (new Frame(Vec2i(3, 3), Vec2i(60, 60), game_header_picture.GetTexture()))
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
    auto round_info_frame = (new Frame(Vec2i(3, 0), Vec2i(50, 30), DONT_DRAW))
        ->SetFlex(FLEX_HEIGHT)
        ->SetAlign(DONT_ALIGN, ALIGN_CENTER)
        ->SetAdaptive(true, true)
        ->SetName("RoundInfoText")
        ->AddChildren({ text1, text2 });

    // Game info
    auto round_info = (new Frame(Vec2i(0, 0), Vec2i(0, 73), game_header_background.GetTexture()))
        ->SetFlex(FLEX_WIDTH, 7)
        ->SetOccupy(true, false)
        ->SetName("RoundInfo")
        ->AddChildren({ picture, round_info_frame });

    // Header
    SetFullyOccupy(true, false);
    SetFlex(FLEX_WIDTH);
    SetName("Header", false);
    AddChildren({ turn_display, round_info });
}