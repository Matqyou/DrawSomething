//
// Created by Matq on 12/01/2025.
//

#include "Panel.h"
#include "../../../core/Application.h"
#include "../../components/element/Button.h"

PreloadTexture Panel::game_guessing_bar("game.guessing_bar");
PreloadTexture Panel::game_letter_normal("game.letter.normal");
PreloadTexture Panel::game_palette_background("game.letter_palette.background");
PreloadTexture Panel::game_palette_bomba("game.letter_palette.bomba");

Texture Panel::game_letter_slot_ = Texture(nullptr, "Letter Slot Composition");

LinkFont Panel::sFontDefaultBiggest("fredoka.biggest");

Panel::Panel()
 : Frame(Vec2i(0, 0), Vec2i(0, 0), DONT_DRAW) {
    auto drawing = Application::Get()->GetDrawing();

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

    SetFullyOccupy(true, false);
    SetAdaptive(false, true);
    SetFlex(FLEX_HEIGHT);
    SetName("Panel");
    AddChildren({ guessing_bar, letters_bar });
}