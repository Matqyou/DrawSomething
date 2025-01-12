//
// Created by Matq on 24/12/2024.
//

#pragma once

#include "../components/element/Frame.h"

class GuessingMenu : public Frame {
public:
    static PreloadTexture game_header_turn;
    static PreloadTexture game_header_picture;
    static PreloadTexture game_header_background;

    static PreloadTexture game_palette_background;
    static PreloadTexture game_palette_bomba;

    static PreloadTexture game_guessing_bar;
    static PreloadTexture game_letter_guess;
    static PreloadTexture game_letter_normal;
    static PreloadTexture game_letter_slot;
    static PreloadTexture game_letter_slot_background;
    static PreloadTexture game_finger;

    static Texture game_letter_slot_;

    static LinkFont sFontDefault;
    static LinkFont sFontDefaultBiggest;
    static LinkFont sFontDefaultBigger;
    static LinkFont sFontDefaultSmaller;

public:
    GuessingMenu();

    // Ticking
    void HandleEvent(SDL_Event& sdl_event, EventContext& event_summary) override;

};
