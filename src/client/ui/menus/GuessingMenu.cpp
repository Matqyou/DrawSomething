//
// Created by Matq on 24/12/2024.
//

#include "GuessingMenu.h"
#include "../../core/Application.h"
#include "../components/element/Button.h"

LoadedTexture GuessingMenu::game_header_turn("game.header.turn");
LoadedTexture GuessingMenu::game_header_picture("game.header.picture");
LoadedTexture GuessingMenu::game_header_background("game.header.background");

LoadedTexture GuessingMenu::game_palette_background("game.letter_palette.background");
LoadedTexture GuessingMenu::game_palette_bomba("game.letter_palette.bomba");

LoadedTexture GuessingMenu::game_guessing_bar("game.guessing_bar");
LoadedTexture GuessingMenu::game_letter_guess("game.letter.guess");
LoadedTexture GuessingMenu::game_letter_normal("game.letter.normal");
LoadedTexture GuessingMenu::game_letter_slot("game.letter.slot");
LoadedTexture GuessingMenu::game_letter_slot_background("game.letter.slot_background");
LoadedTexture GuessingMenu::game_finger("game.finger");

Texture GuessingMenu::game_letter_slot_ = Texture(nullptr, "Letter Slot Composition");

LoadedFont GuessingMenu::sDefaultFont("fredoka.bold", 26);
LoadedFont GuessingMenu::sDefaultFontBiggest("fredoka.bold", 46);
LoadedFont GuessingMenu::sDefaultFontBigger("fredoka.bold", 36);
LoadedFont GuessingMenu::sDefaultFontSmaller("fredoka.medium", 18);

GuessingMenu::GuessingMenu()
    : Frame(Vec2i(0, 0), Vec2i(0, 0), DONT_DRAW) {
    auto drawing = Application::Get()->GetDrawing();

    // Turn title text
    Texture* turn_title_render;
    {
        SDL_Surface* sdl_surface =
            TTF_RenderText_Blended(sDefaultFontSmaller.GetFont()->TTFFont(),
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
        ->SetName("TurnTitle");

    // Turn number text
    Texture* turn_number_render;
    {
        SDL_Surface* sdl_surface =
            TTF_RenderText_Blended(sDefaultFontBigger.GetFont()->TTFFont(), "6", strlen("6"), { 255, 255, 255, 255 });
        turn_number_render = Assets::Get()->TextureFromSurface(sdl_surface)
            ->FlagForAutomaticDeletion();
        SDL_DestroySurface(sdl_surface);
    }
    auto turn_number = (new Frame(Vec2i(0, -4),
                                  Vec2i(turn_number_render->GetWidth(), turn_number_render->GetHeight()),
                                  turn_number_render))
        ->SetAlign(ALIGN_CENTER, DONT_ALIGN)
        ->SetName("TurnNumber");

    // Turn text frame
    auto turn_info_frame = (new Frame(Vec2i(0, 0), Vec2i(50, 30), DONT_DRAW))
        ->SetFlex(FLEX_HEIGHT)
        ->SetAlign(ALIGN_CENTER, ALIGN_CENTER)
        ->SetAdaptive(true, true)
        ->SetName("Text")
        ->SetChildren({ turn_title, turn_number });

    // Turn display
    auto turn_display = (new Frame(Vec2i(0, 0), Vec2i(73, 73), game_header_turn.GetTexture()))
        ->SetName("Turn")
        ->SetChildren({ turn_info_frame });

    // Profile picutre
    auto picture = (new Frame(Vec2i(0, 0), Vec2i(68, 73), game_header_picture.GetTexture()))
        ->SetName("Picture");

    // Game info text1
    Texture* game_info_text1;
    {
        SDL_Surface* sdl_surface =
            TTF_RenderText_Blended(sDefaultFontSmaller.GetFont()->TTFFont(),
                                   "You are guessing",
                                   strlen("You are guessing"),
                                   { 0, 0, 0, 255 });
        game_info_text1 = Assets::Get()->TextureFromSurface(sdl_surface)
            ->FlagForAutomaticDeletion();
        SDL_DestroySurface(sdl_surface);
    }
    auto text1 =
        (new Frame(Vec2i(0, 0), Vec2i(game_info_text1->GetWidth(), game_info_text1->GetHeight()), game_info_text1))
            ->SetName("Text1");

    // Game info text2
    Texture* game_info_text2;
    {
        SDL_Surface* sdl_surface =
            TTF_RenderText_Blended(sDefaultFontSmaller.GetFont()->TTFFont(),
                                   "Name S.'s drawing.",
                                   strlen("Name S.'s drawing."),
                                   { 0, 0, 0, 255 });
        game_info_text2 = Assets::Get()->TextureFromSurface(sdl_surface);
        SDL_DestroySurface(sdl_surface);
    }
    auto text2 =
        (new Frame(Vec2i(0, 0), Vec2i(game_info_text2->GetWidth(), game_info_text2->GetHeight()), game_info_text2))
            ->SetName("Text2");

    // Game info text frame
    auto round_info_frame = (new Frame(Vec2i(10, 0), Vec2i(50, 30), DONT_DRAW))
        ->SetFlex(FLEX_HEIGHT)
        ->SetAlign(ALIGN_LEFT, ALIGN_CENTER)
        ->SetAdaptive(true, true)
        ->SetName("Text")
        ->SetChildren({ text1, text2 });

    // Game info
    auto round_info = (new Frame(Vec2i(0, 0), Vec2i(0, 73), game_header_background.GetTexture()))
        ->SetOccupy(true, false)
        ->SetName("Info")
        ->SetChildren({ round_info_frame });

    // Header
    auto header = (new Frame(Vec2i(0, 0), Vec2i(0, 73), DONT_DRAW))
        ->SetFullyOccupy(true, false)
        ->SetFlex(FLEX_WIDTH)
        ->SetName("Header")
        ->SetChildren({ turn_display, picture, round_info });

    // Finger text
    Texture* finger_render;
    {
        SDL_Surface* sdl_surface =
            TTF_RenderText_Blended(sDefaultFontBiggest.GetFont()->TTFFont(),
                                   "TAP TO GUESS.",
                                   strlen("TAP TO GUESS."),
                                   { 136, 136, 136, 255 });
        finger_render = Assets::Get()->TextureFromSurface(sdl_surface);
        SDL_DestroySurface(sdl_surface);
    }
    auto finger_text =
        (new Frame(Vec2i(0, 0), Vec2i(finger_render->GetSize()), finger_render))
            ->SetAlign(ALIGN_CENTER, DONT_ALIGN)
            ->SetName("Text2");

    // The finger
    auto finger = (new Frame(Vec2i(0, 0),
                             Vec2i(Rectangles::ScaleToBounds(game_finger.GetTexture()->GetSize(),
                                                             Vec2f(400.0f, 400.0f))),
                             game_finger.GetTexture()))
        ->SetAlign(ALIGN_CENTER, DONT_ALIGN)
        ->SetName("Finger");

    // Finger frame
    auto finger_frame = (new Frame(Vec2i(0, 0),
                                   Vec2i(0, 0),
                                   DONT_DRAW))
        ->SetFlex(FLEX_HEIGHT, 10)
        ->SetAdaptive(true, true)
        ->SetAlign(ALIGN_CENTER, ALIGN_CENTER)
        ->SetName("FingerBox")
        ->SetChildren({ finger_text, finger });

    // Canvas
    auto canvas = (new Frame(Vec2i(0, 0),
                             Vec2i(0, 0),
                             DRAW_RECT))
        ->SetFullyOccupy(true, false)
        ->SetOccupy(false, true)
        ->SetColor(255, 255, 255, 255)
        ->SetName("Canvas")
        ->SetChildren({ finger_frame });

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
        ->SetChildren(guessing_letters);

    // Guessing bar
    auto guessing_bar = (new Frame(Vec2i(0, 0),
                                   Vec2i(0, 77),
                                   game_guessing_bar.GetTexture()))
        ->SetFullyOccupy(true, false)
        ->SetFlexInvolved(true, false)
        ->SetAlign(DONT_ALIGN, ALIGN_ABOVE_TOP)
        ->SetName("Guessing")
        ->SetChildren({ guessing_frame });

    // Top row letters
    std::vector<Element*> top_row_letters;
    for (int i = 0; i < 6; i++) {
        auto letter_character = Strings::RandomUppercaseLetter();
        Texture* character_render;
        {
            SDL_Surface* sdl_surface =
                TTF_RenderText_Blended(sDefaultFontBiggest.GetFont()->TTFFont(),
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
        ->SetChildren(top_row_letters);

    // Bottom row letters
    std::vector<Element*> bottom_row_letters;
    for (int i = 0; i < 6; i++) {
        auto new_letter = (new Button(Vec2i(0, 0),
                                      Vec2i(66, 66),
                                      game_letter_normal.GetTexture()))
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
        ->SetChildren(bottom_row_letters);

    // Rows frame
    auto rows_frame = (new Frame(Vec2i(0, 0),
                                 Vec2i(0, 0),
                                 DONT_DRAW))
        ->SetAlign(ALIGN_CENTER, ALIGN_CENTER)
        ->SetAdaptive(true, true)
        ->SetFlex(FLEX_HEIGHT, 5)
        ->SetName("AllRows")
        ->SetChildren({ top_row, bottom_row });

    // Rows side
    auto rows = (new Frame(Vec2i(0, 0),
                           Vec2i(0, 0),
                           DONT_DRAW))
        ->SetOccupy(true, false)
        ->SetFullyOccupy(false, true)
        ->SetName("Rows")
        ->SetChildren({ rows_frame });

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
        ->SetChildren({ bomba });

    // Letters bar
    auto letters_bar = (new Frame(Vec2i(0, 0),
                                  Vec2i(0, 165),
                                  game_palette_background.GetTexture()))
        ->SetFullyOccupy(true, false)
        ->SetFlex(FLEX_WIDTH)
        ->SetName("Bottom")
        ->SetChildren({ rows, bomba_frame });

    // Bottom panel
    auto panel = (new Frame(Vec2i(0, 0), Vec2i(0, 0), DONT_DRAW))
        ->SetFullyOccupy(true, false)
        ->SetAdaptive(false, true)
        ->SetFlex(FLEX_HEIGHT)
        ->SetName("Panel")
        ->SetChildren({ guessing_bar, letters_bar });

    std::initializer_list<Element*> game_elements = {
        header,
        canvas,
        panel,
    };

    SetFlex(FLEX_HEIGHT);
    SetChildren(game_elements)->SetName("GuessingMenu", false);
    SetSize(Vec2i(0, 0),
            Application::Get()->GetResolution(),
            Application::Get()->GetResolution());
    SetSize(Vec2i(0, 0),
            Application::Get()->GetResolution(),
            Application::Get()->GetResolution());
}

void GuessingMenu::HandleEvent(SDL_Event& sdl_event, EventContext& event_summary) {
    switch (sdl_event.type) {
        case SDL_EVENT_WINDOW_RESIZED: {
            SetSize(Vec2i(0, 0),
                    Application::Get()->GetResolution(),
                    Application::Get()->GetResolution());

            break;
        }
    }

    HandleEventChildren(sdl_event, event_summary);
}