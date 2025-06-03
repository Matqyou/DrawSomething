//
// Created by Matq on 21/01/2025.
//

#pragma once

#include "../../../../components/element/Frame.h"
#include "Letter.h"

namespace Ingame {
class GuessingBar;
class Letters : public Frame {
private:
    std::vector<Letter*> all_letters;
    Frame* top_row;
    Frame* bottom_row;
    std::string guess_word;

    GuessingBar* guessing_bar_;

public:
    explicit Letters(GuessingBar* guessing_bar);
    ~Letters();

    // Manipulation
    bool RandomizeWord(const std::string& word);
    void BlowUp();

    // Ticking
    void HandleEvent(const SDL_Event& sdl_event, EventContext& event_summary) override;

};
}
