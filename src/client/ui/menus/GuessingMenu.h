//
// Created by Matq on 24/12/2024.
//

#pragma once

#include "../components/element/Frame.h"

class GuessingMenu : public Frame {
public:
    static LoadedTexture game_header_turn;
    static LoadedTexture game_header_picture;
    static LoadedTexture game_header_background;

    static LoadedTexture game_palette_background;
    static LoadedTexture game_palette_bomba;

    static LoadedTexture game_guessing_bar;
    static LoadedTexture game_letter_guess;
    static LoadedTexture game_letter_normal;
    static LoadedTexture game_letter_slot;
    static LoadedTexture game_letter_slot_background;

    static Texture game_letter_slot_;

    static LoadedFont sDefaultFont;
    static LoadedFont sDefaultFontBigger;
    static LoadedFont sDefaultFontSmaller;

public:
    GuessingMenu();

    // Ticking
    void HandleEvent(SDL_Event& sdl_event, EventContext& event_summary) override;

};
