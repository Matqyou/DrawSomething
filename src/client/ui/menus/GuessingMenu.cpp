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

Texture GuessingMenu::game_letter_slot_ = Texture(nullptr, "Letter Slot Composition");

LoadedFont GuessingMenu::sDefaultFont("fredoka.bold", 26);
LoadedFont GuessingMenu::sDefaultFontBigger("fredoka.bold", 36);
LoadedFont GuessingMenu::sDefaultFontSmaller("fredoka.medium", 18);

GuessingMenu::GuessingMenu()
    : Frame(Vec2i(0, 0), Vec2i(0, 0), DONT_DRAW) {
    auto drawing = Application::Get()->GetDrawing();

    // Turn title text
    SDL_Surface* turn_title_surface =
        TTF_RenderText_Blended(sDefaultFontSmaller.GetFont()->TTFFont(), "turn", { 255, 255, 255, 255 });
    Texture* turn_title_render = Assets::Get()->TextureFromSurface(turn_title_surface)
        ->FlagForAutomaticDeletion();
    SDL_FreeSurface(turn_title_surface);
    auto turn_title = (new Frame(Vec2i(0, 4), turn_title_render->GetSize(), turn_title_render))
        ->SetName("TurnTitle");

    // Turn number text
    SDL_Surface* turn_surface =
        TTF_RenderText_Blended(sDefaultFontBigger.GetFont()->TTFFont(), "6", { 255, 255, 255, 255 });
    Texture* turn_number_render = Assets::Get()->TextureFromSurface(turn_surface)
        ->FlagForAutomaticDeletion();
    SDL_FreeSurface(turn_surface);
    auto turn_number = (new Frame(Vec2i(0, -4), turn_number_render->GetSize(), turn_number_render))
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
    SDL_Surface* surface =
        TTF_RenderText_Blended(sDefaultFontSmaller.GetFont()->TTFFont(), "You are guessing", { 0, 0, 0, 255 });
    Texture* line_render = Assets::Get()->TextureFromSurface(surface)
        ->FlagForAutomaticDeletion();
    SDL_FreeSurface(surface);
    auto text1 = (new Frame(Vec2i(0, 0), line_render->GetSize(), line_render))
        ->SetName("Text1");

    // Game info text2
    SDL_Surface* surface2 =
        TTF_RenderText_Blended(sDefaultFontSmaller.GetFont()->TTFFont(), "Name S.'s drawing.", { 0, 0, 0, 255 });
    Texture* line_render2 = Assets::Get()->TextureFromSurface(surface2);
    SDL_FreeSurface(surface2);
    auto text2 = (new Frame(Vec2i(0, 0), line_render2->GetSize(), line_render2))
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

    auto canvas = (new Frame(Vec2i(0, 0),
                             Vec2i(0, 0),
                             DRAW_RECT))
        ->SetFullyOccupy(true, false)
        ->SetOccupy(false, true)
        ->SetColor(255, 255, 255, 255);

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
        auto letter_character = "A";
//        SDL_Surface* character_surface =
//            TTF_RenderText_Blended(sDefaultFontBigger.GetFont()->TTFFont(),
//                                   letter_character,
//                                   { 255, 255, 255, 255 });
//        std::wcout << Strings::FStringColorsW(L"&eError: %s", SDL_GetError()) << std::endl;
//        Texture* character_render = Assets::Get()->TextureFromSurface(surface);
//        SDL_FreeSurface(character_surface);
//        SDL_Rect character_rect = {
//            33 - character_render->GetWidth() / 2,
//            33 - character_render->GetHeight() / 2,
//            character_render->GetWidth(),
//            character_render->GetHeight(),
//        };
//        auto letter = (new Texture(SDL_CreateTexture(drawing->Renderer(),
//                                                     SDL_PIXELFORMAT_RGBA8888,
//                                                     SDL_TEXTUREACCESS_TARGET,
//                                                     game_letter_normal.GetTexture()->GetWidth(),
//                                                     game_letter_normal.GetTexture()->GetHeight())))
//            ->FlagForAutomaticDeletion();
//        letter->SetBlendMode(SDL_BLENDMODE_BLEND);
//        drawing->SetRenderTarget(letter);
//        drawing->RenderTextureFullscreen(game_letter_normal.GetTexture()->SDLTexture(), nullptr);
//        drawing->RenderTexture(character_render->SDLTexture(), nullptr, character_rect);
//        drawing->SetRenderTarget(nullptr);
//        delete character_render;
//
//        auto new_letter = (new Button(Vec2i(0, 0),
//                                      Vec2i(66, 66),
//                                      letter))
//            ->SetAlign(DONT_ALIGN, ALIGN_CENTER)
//            ->SetName("Letter");
//
//        top_row_letters.push_back(new_letter);
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