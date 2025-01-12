//
// Created by Matq on 24/12/2024.
//

#include "GuessingMenu.h"
#include "../../core/Application.h"
#include "../components/element/Button.h"
#include "../components/Canvas.h"

PreloadTexture GuessingMenu::game_header_turn("game.header.turn");
PreloadTexture GuessingMenu::game_header_picture("game.header.picture");
PreloadTexture GuessingMenu::game_header_background("game.header.background");

PreloadTexture GuessingMenu::game_palette_background("game.letter_palette.background");
PreloadTexture GuessingMenu::game_palette_bomba("game.letter_palette.bomba");

PreloadTexture GuessingMenu::game_guessing_bar("game.guessing_bar");
PreloadTexture GuessingMenu::game_letter_guess("game.letter.guess");
PreloadTexture GuessingMenu::game_letter_normal("game.letter.normal");
PreloadTexture GuessingMenu::game_letter_slot("game.letter.slot");
PreloadTexture GuessingMenu::game_letter_slot_background("game.letter.slot_background");

Texture GuessingMenu::game_letter_slot_ = Texture(nullptr, "Letter Slot Composition");

LinkFont GuessingMenu::sFontDefault("default");
LinkFont GuessingMenu::sFontDefaultBiggest("fredoka.biggest");
LinkFont GuessingMenu::sFontDefaultBigger("fredoka.big");
LinkFont GuessingMenu::sFontDefaultSmaller("fredoka.small");

GuessingMenu::GuessingMenu()
    : Frame(Vec2i(0, 0), Vec2i(0, 0), DONT_DRAW) {
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
    auto header = (new Frame(Vec2i(0, 0), Vec2i(0, 73), DONT_DRAW))
        ->SetFullyOccupy(true, false)
        ->SetFlex(FLEX_WIDTH)
        ->SetName("Header", false)
        ->AddChildren({ turn_display, picture, round_info });

    // Guessing letters
    std::vector<Element*> guessing_letters;
    for (int i = 0; i < 4; i++) {
        auto new_letter = (new Frame(Vec2i(0, 0),
                                     Vec2i(66, 66),
                                     &game_letter_slot_))
            ->SetAlign(DONT_ALIGN, ALIGN_CENTER)
            ->SetName("Letter");

        guessing_letters.push_back(new_letter);
    }

    // Guessing letter frame
    auto guessing_frame = (new Frame(Vec2i(0, 0),
                                     Vec2i(0, 0),
                                     DONT_DRAW))
        ->SetAlign(ALIGN_CENTER, ALIGN_CENTER)
        ->SetAdaptive(true, true)
        ->SetFlex(FLEX_WIDTH, 10)
        ->AddChildren(guessing_letters);

    // Guessing bar
    auto guessing_bar = (new Frame(Vec2i(0, 0),
                                   Vec2i(0, 77),
                                   game_guessing_bar.GetTexture()))
        ->SetFullyOccupy(true, false)
        ->SetFlexInvolved(true, false)
        ->SetAlign(DONT_ALIGN, ALIGN_ABOVE_TOP)
        ->SetName("Guessing")
        ->AddChildren({ guessing_frame });

    // Top row letters
    std::vector<Element*> top_row_letters;
    for (int i = 0; i < 6; i++) {
        auto letter_character = Strings::RandomUppercaseLetter();
        Texture* character_render;
        {
            SDL_Surface* sdl_surface =
                TTF_RenderText_Blended(sFontDefaultBiggest.GetFont()->TTFFont(),
                                       letter_character,
                                       strlen(letter_character),
                                       { 255, 255, 255, 255 });
            character_render = Assets::Get()->TextureFromSurface(sdl_surface);
            SDL_DestroySurface(sdl_surface);
        }
        auto size = game_letter_normal.GetTexture()->GetSize();
        SDL_FRect character_rect = {
            (float)(size.x / 2 - character_render->GetWidth()),
            (float)(size.y / 2 - character_render->GetHeight()),
            (float)(character_render->GetWidth() * 2),
            (float)(character_render->GetHeight() * 2),
        };
        SDL_FRect character_rect_higher = character_rect;
        character_rect_higher.y -= 4;
        auto letter = (new Texture(SDL_CreateTexture(drawing->Renderer(),
                                                     SDL_PIXELFORMAT_RGBA8888,
                                                     SDL_TEXTUREACCESS_TARGET,
                                                     game_letter_normal.GetTexture()->GetWidth(),
                                                     game_letter_normal.GetTexture()->GetHeight())))
            ->FlagForAutomaticDeletion();
        letter->SetBlendMode(SDL_BLENDMODE_BLEND);
        drawing->SetRenderTarget(letter);
        drawing->RenderTextureFullscreen(game_letter_normal.GetTexture()->SDLTexture(), nullptr);
        character_render->SetColorMod(41, 77, 104);
        drawing->RenderTexture(character_render->SDLTexture(), nullptr, character_rect_higher);
        character_render->SetColorMod(255, 255, 255);
        drawing->RenderTexture(character_render->SDLTexture(), nullptr, character_rect);
        drawing->SetRenderTarget(nullptr);
        delete character_render;

        auto new_letter = (new Button(Vec2i(0, 0),
                                      Vec2i(66, 66),
                                      letter))
            ->SetAlign(DONT_ALIGN, ALIGN_CENTER)
            ->SetName("Letter");

        top_row_letters.push_back(new_letter);
    }

    // Top row
    auto top_row = (new Frame(Vec2i(0, 0),
                              Vec2i(66, 66),
                              DONT_DRAW))
        ->SetAlign(ALIGN_CENTER, DONT_ALIGN)
        ->SetFlex(FLEX_WIDTH, 5)
        ->SetAdaptive(true, true)
        ->SetName("TopRow")
        ->AddChildren(top_row_letters);

    // Bottom row letters
    std::vector<Element*> bottom_row_letters;
    for (int i = 0; i < 6; i++) {
        auto letter_character = Strings::RandomUppercaseLetter();
        Texture* character_render;
        {
            SDL_Surface* sdl_surface =
                TTF_RenderText_Blended(sFontDefaultBiggest.GetFont()->TTFFont(),
                                       letter_character,
                                       strlen(letter_character),
                                       { 255, 255, 255, 255 });
            character_render = Assets::Get()->TextureFromSurface(sdl_surface);
            SDL_DestroySurface(sdl_surface);
        }
        auto size = game_letter_normal.GetTexture()->GetSize();
        SDL_FRect character_rect = {
            (float)(size.x / 2 - character_render->GetWidth()),
            (float)(size.y / 2 - character_render->GetHeight()),
            (float)(character_render->GetWidth() * 2),
            (float)(character_render->GetHeight() * 2),
        };
        SDL_FRect character_rect_higher = character_rect;
        character_rect_higher.y -= 4;
        auto letter = (new Texture(SDL_CreateTexture(drawing->Renderer(),
                                                     SDL_PIXELFORMAT_RGBA8888,
                                                     SDL_TEXTUREACCESS_TARGET,
                                                     game_letter_normal.GetTexture()->GetWidth(),
                                                     game_letter_normal.GetTexture()->GetHeight())))
            ->FlagForAutomaticDeletion();
        letter->SetBlendMode(SDL_BLENDMODE_BLEND);
        drawing->SetRenderTarget(letter);
        drawing->RenderTextureFullscreen(game_letter_normal.GetTexture()->SDLTexture(), nullptr);
        character_render->SetColorMod(41, 77, 104);
        drawing->RenderTexture(character_render->SDLTexture(), nullptr, character_rect_higher);
        character_render->SetColorMod(255, 255, 255);
        drawing->RenderTexture(character_render->SDLTexture(), nullptr, character_rect);
        drawing->SetRenderTarget(nullptr);
        delete character_render;

        auto new_letter = (new Button(Vec2i(0, 0),
                                      Vec2i(66, 66),
                                      letter))
            ->SetAlign(DONT_ALIGN, ALIGN_CENTER)
            ->SetName("Letter");

        bottom_row_letters.push_back(new_letter);
    }

    // Bottom row
    auto bottom_row = (new Frame(Vec2i(0, 0),
                                 Vec2i(66, 66),
                                 DONT_DRAW))
        ->SetAlign(ALIGN_CENTER, DONT_ALIGN)
        ->SetFlex(FLEX_WIDTH, 5)
        ->SetAdaptive(true, true)
        ->SetName("BottomRow")
        ->AddChildren(bottom_row_letters);

    // Rows frame
    auto rows_frame = (new Frame(Vec2i(0, 0),
                                 Vec2i(0, 0),
                                 DONT_DRAW))
        ->SetAlign(ALIGN_CENTER, ALIGN_CENTER)
        ->SetAdaptive(true, true)
        ->SetFlex(FLEX_HEIGHT, 5)
        ->SetName("AllRows")
        ->AddChildren({ top_row, bottom_row });

    // Rows side
    auto rows = (new Frame(Vec2i(0, 0),
                           Vec2i(0, 0),
                           DONT_DRAW))
        ->SetOccupy(true, false)
        ->SetFullyOccupy(false, true)
        ->SetName("Rows")
        ->AddChildren({ rows_frame });

    // Bomba
    auto bomba = (new Button(Vec2i(0, 0),
                             Vec2i(98, 147),
                             game_palette_bomba.GetTexture()))
        ->SetCallback([]() { std::wcout << L"BOMBA" << std::endl; })
        ->SetAlign(ALIGN_CENTER, ALIGN_CENTER)
        ->SetName("Bomba");

    // Bomba side
    auto bomba_frame = (new Frame(Vec2i(0, 0),
                                  Vec2i(114, 0),
                                  DONT_DRAW))
        ->SetFullyOccupy(false, true)
        ->SetFlex(FLEX_WIDTH)
        ->SetName("Bomba")
        ->AddChildren({ bomba });

    // Letters bar
    auto letters_bar = (new Frame(Vec2i(0, 0),
                                  Vec2i(0, 165),
                                  game_palette_background.GetTexture()))
        ->SetFullyOccupy(true, false)
        ->SetFlex(FLEX_WIDTH)
        ->SetName("Bottom")
        ->AddChildren({ rows, bomba_frame });

    // Bottom panel
    auto panel = (new Frame(Vec2i(0, 0), Vec2i(0, 0), DONT_DRAW))
        ->SetFullyOccupy(true, false)
        ->SetAdaptive(false, true)
        ->SetFlex(FLEX_HEIGHT)
        ->SetName("Panel")
        ->AddChildren({ guessing_bar, letters_bar });

    // Canvas
    Canvas* canvas;
    canvas = (Canvas*)(new Canvas(Vec2i(0, 0),
                                  Vec2i(0, 0),
                                  DRAW_RECT))
        ->SetCallback([this, panel]() {
            this->AddChildren({ panel });
            this->Refresh();
        })
        ->SetFullyOccupy(true, false)
        ->SetOccupy(false, true)
        ->SetColor(255, 255, 255, 255)
        ->SetName("Canvas", false);

    SetFlex(FLEX_HEIGHT);
    AddChildren({ header, canvas })->SetName("GuessingMenu", false);
    UpdateElement(Vec2i(0, 0),
                  Application::Get()->GetResolution(),
                  Application::Get()->GetResolution());
}

void GuessingMenu::HandleEvent(SDL_Event& sdl_event, EventContext& event_summary) {
    switch (sdl_event.type) {
        case SDL_EVENT_WINDOW_RESIZED: {
            UpdateElement(Vec2i(0, 0),
                          Application::Get()->GetResolution(),
                          Application::Get()->GetResolution());

            break;
        }
    }

    HandleEventChildren(sdl_event, event_summary);
}